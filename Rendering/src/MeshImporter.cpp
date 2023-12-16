#include <MeshImporter.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_set>

MeshImporter::MeshImporter(RenderPipeline& pipeline) : pipeline(pipeline) {
}

bool MeshImporter::ConvertData(ImportedMeshAsset& target, const char* data, size_t length) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFileFromMemory(data, length, 
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_PopulateArmatureData |
		aiProcess_LimitBoneWeights
	);

	if (!scene) {
		SF_LOG(Mesh Import, Error, "Failed to import mesh file: %s", importer.GetErrorString())
		return false;
	}

	if (scene->mNumMeshes == 0) {
		SF_LOG(Mesh Import, Error, "File does not contain any meshes")
		return false;
	}

	if (scene->mNumMeshes > 1) {
		SF_LOG(Mesh Import, Warning, "Currently unable to import multiple meshs from a single file. Num of meshes in file: %i", scene->mNumMeshes)
	}

	target.Meshes.push_back(pipeline.CreateMesh());

	std::vector<VertexBuffer::VertexDescriptor> descriptors;

	//Mesh::GlobalInverse = reinterpret_cast<Matrix4x4*>(&scene->mRootNode->mTransformation)->Transpose().Inverse();

	if (scene->mMeshes[0]->HasPositions()) {
		descriptors.push_back(VertexBuffer::VertexDescriptor::Position);
	}

	if (scene->mMeshes[0]->HasNormals()) {
		descriptors.push_back(VertexBuffer::VertexDescriptor::Normal);
	}

	if (scene->mMeshes[0]->HasVertexColors(0)) {
		descriptors.push_back(VertexBuffer::VertexDescriptor::Color_RGBA_Float);
	}

	if (scene->mMeshes[0]->HasTextureCoords(0)) {
		switch (scene->mMeshes[0]->mNumUVComponents[0]) {
			case 1:
				SF_LOG(Mesh Import, Warning, "Single channel texture coordinate found, treating it as a 2 channel (UV Coordinate)")

			case 2:
				descriptors.push_back(VertexBuffer::VertexDescriptor::UV_Coordinate);
				break;

			case 3:
				SF_LOG(Mesh Import, Warning, "Imported mesh uses UVW texture coordinates")
				descriptors.push_back(VertexBuffer::VertexDescriptor::UVW_Coordinate);
				break;
		}

		if (scene->mMeshes[0]->HasTextureCoords(1)) {
			SF_LOG(Mesh Importer, Warning, "Currently unable to import more than 1 UV channel")
		}
	}

	struct BoneWeight {
		VertexBuffer::BoneIndex BoneIndex;
		VertexBuffer::BoneWeight Weight;

		BoneWeight(VertexBuffer::BoneIndex index, VertexBuffer::BoneWeight weight) : BoneIndex(index), Weight(weight) {}
	};
	
	std::vector<BoneWeight>* BoneWeights = nullptr;
	SFSharedPtr<Skeleton> importedSkeleton = nullptr;
	std::unordered_set<std::string> BoneNames;

	if (scene->mMeshes[0]->HasBones()) {
		BoneWeights = new std::vector<BoneWeight>[scene->mMeshes[0]->mNumVertices];
		importedSkeleton = new Skeleton();
		target.Skeletons.emplace_back(importedSkeleton);

		// record bone names
		aiNode* RootBone = scene->mMeshes[0]->mBones[0]->mArmature; // ->mChildren[0];
		for (int i = 0; i < scene->mMeshes[0]->mNumBones; ++i) {
			BoneNames.emplace(scene->mMeshes[0]->mBones[i]->mName.C_Str());
		}

		// build skeleton hierarchy
		Skeleton::Bone NewSkeleton = ConvertSkeletalNode(*RootBone, BoneNames);

		// bind the skeleton, setting up bone indices
		importedSkeleton->SetupSkeleton(NewSkeleton);

		// assign inverse bind matrix from bone data
		for (auto& bone : importedSkeleton->RootBone) {
			for (int i = 0; i < scene->mMeshes[0]->mNumBones; ++i) {
				if (bone.Name == scene->mMeshes[0]->mBones[i]->mName.C_Str()) {
					auto matrix = reinterpret_cast<const Matrix4x4*>(&scene->mMeshes[0]->mBones[i]->mOffsetMatrix)->Transpose();

					bone.InverseBind = matrix;

					break;
				}
			}
		}

		descriptors.push_back(VertexBuffer::VertexDescriptor::Bone_Index);
		descriptors.push_back(VertexBuffer::VertexDescriptor::Bone_Weight);

		for (int i = 0; i < scene->mMeshes[0]->mNumBones; ++i) {
			for (int y = 0; y < scene->mMeshes[0]->mBones[i]->mNumWeights; ++y) {
				auto* bone = importedSkeleton->RootBone.FindBone(scene->mMeshes[0]->mBones[i]->mName.C_Str());
				BoneWeights[scene->mMeshes[0]->mBones[i]->mWeights[y].mVertexId].push_back({ static_cast<VertexBuffer::BoneIndex>(bone->BoneIndex), scene->mMeshes[0]->mBones[i]->mWeights[y].mWeight });
			}
		}

		target.Meshes[0]->SetSkeleton(importedSkeleton);
	}

	target.Meshes[0]->GetVertexBuffer().SetDescriptors(descriptors);

	std::vector<VertexBuffer::IndexType> indices;
	size_t badIndices = 0;
	for (size_t i = 0; i < scene->mMeshes[0]->mNumFaces; ++i) {
		if (scene->mMeshes[0]->mFaces[i].mNumIndices != 3) {
			badIndices++;
			continue;
		}

		indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[0]);
		indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[1]);
		indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[2]);
	}

	if (badIndices > 0) {
		SF_LOG(Mesh Import, Error, "Mesh contains %i faces that could not be imported correctly", badIndices)
	}
	target.Meshes[0]->GetVertexBuffer().SetIndices(indices.data(), indices.size());

	uint8_t* vertData = new uint8_t[scene->mMeshes[0]->mNumVertices * target.Meshes[0]->GetVertexBuffer().GetVertexStride()];
	size_t dataOffset = 0;
	size_t descriptorSize;
	for (size_t i = 0; i < scene->mMeshes[0]->mNumVertices; ++i) {
		for (int desc = 0; desc < descriptors.size(); ++desc) {
			descriptorSize = VertexBuffer::GetDescriptorSize(descriptors[desc]);

			switch (descriptors[desc]) {
				case VertexBuffer::VertexDescriptor::Position:
					memcpy(vertData + dataOffset, scene->mMeshes[0]->mVertices + i, descriptorSize);
					break;

				case VertexBuffer::VertexDescriptor::Normal:
					memcpy(vertData + dataOffset, scene->mMeshes[0]->mNormals + i, descriptorSize);
					break;

				case VertexBuffer::VertexDescriptor::Color_RGBA_Float:
					memcpy(vertData + dataOffset, scene->mMeshes[0]->mColors[0] + i, descriptorSize);
					break;

				case VertexBuffer::VertexDescriptor::Tangent:
					memcpy(vertData + dataOffset, scene->mMeshes[0]->mTangents + i, descriptorSize);
					break;

				case VertexBuffer::VertexDescriptor::UV_Coordinate:
					memcpy(vertData + dataOffset, scene->mMeshes[0]->mTextureCoords[0] + i, descriptorSize);

					break;

				case VertexBuffer::VertexDescriptor::UVW_Coordinate:
					memcpy(vertData + dataOffset, scene->mMeshes[0]->mTextureCoords + i, descriptorSize);
					break;

				case VertexBuffer::VertexDescriptor::Bone_Index:
					for (int y = 0; y < 4; ++y) {
						if (y >= BoneWeights[i].size()) {
							memset(vertData + dataOffset + (y * sizeof(VertexBuffer::BoneIndex)), 0, sizeof(VertexBuffer::BoneIndex));
						} else {
							memcpy(vertData + dataOffset + (y * sizeof(VertexBuffer::BoneIndex)), &BoneWeights[i][y].BoneIndex, sizeof(VertexBuffer::BoneIndex));
						}
					}
					break;

				case VertexBuffer::VertexDescriptor::Bone_Weight:
					for (int y = 0; y < 4; ++y) {
						if (y >= BoneWeights[i].size()) {
							memset(vertData + dataOffset + (y * sizeof(VertexBuffer::BoneWeight)), 0, sizeof(VertexBuffer::BoneWeight));
						} else {
							memcpy(vertData + dataOffset + (y * sizeof(VertexBuffer::BoneWeight)), &BoneWeights[i][y].Weight, sizeof(VertexBuffer::BoneWeight));
						}
					}
					break;
			}

			dataOffset += descriptorSize;
		}
	}

	if (scene->HasAnimations()) {
		for (int AnimNumber = 0; AnimNumber < scene->mNumAnimations; ++AnimNumber) {
			SFSharedPtr<SkeletalAnimation> Animation = new SkeletalAnimation();
			target.Animations.push_back(Animation);

			Animation->Duration = scene->mAnimations[AnimNumber]->mDuration;
			Animation->PlaySpeed = scene->mAnimations[AnimNumber]->mTicksPerSecond;

			for (Skeleton::Bone& bone : importedSkeleton->RootBone) {
				for (int channel = 0; channel < scene->mAnimations[AnimNumber]->mNumChannels; ++channel) {
					//std::cout << scene->mAnimations[AnimNumber]->mChannels[channel]->mNodeName.C_Str() << "\n";
					bool Unfound = true;
					for (Skeleton::Bone& bone : importedSkeleton->RootBone) {
						if (bone.Name == scene->mAnimations[AnimNumber]->mChannels[channel]->mNodeName.C_Str()) {
							Unfound = false;
						}
					}

					if (Unfound) {
						// todo: remove or move this warning, it is extremely porformance heavy to find
						SF_LOG(Mesh Importer, Warning, "Loaded animation contains a bone that is not recognised: %s", scene->mAnimations[AnimNumber]->mChannels[channel]->mNodeName.C_Str())
					}

					SkeletalAnimation::BoneKeys Keys;
					Keys.BoneName = scene->mAnimations[AnimNumber]->mChannels[channel]->mNodeName.C_Str();

					if (Keys.BoneName != bone.Name) {
						continue;
					}

					for (int key = 0; key < scene->mAnimations[AnimNumber]->mChannels[channel]->mNumRotationKeys; ++key) {
						auto& RotKey = scene->mAnimations[AnimNumber]->mChannels[channel]->mRotationKeys[key];
						Quaternion quat = { RotKey.mValue.x, RotKey.mValue.y, RotKey.mValue.z, RotKey.mValue.w };
						Keys.Rotations.emplace_back(RotKey.mTime, quat);
					}

					for (int key = 0; key < scene->mAnimations[AnimNumber]->mChannels[channel]->mNumPositionKeys; ++key) {
						auto& PosKey = scene->mAnimations[AnimNumber]->mChannels[channel]->mPositionKeys[key];
						Vector3 vec = { PosKey.mValue.x, PosKey.mValue.y, PosKey.mValue.z };
						Keys.Positions.emplace_back(PosKey.mTime, vec);
					}

					for (int key = 0; key < scene->mAnimations[AnimNumber]->mChannels[channel]->mNumScalingKeys; ++key) {
						auto& ScaleKey = scene->mAnimations[AnimNumber]->mChannels[channel]->mScalingKeys[key];
						Vector3 vec = { ScaleKey.mValue.x, ScaleKey.mValue.y, ScaleKey.mValue.z };

						// check if its the root bone
						if (scene->mAnimations[AnimNumber]->mChannels[channel]->mNodeName == scene->mMeshes[0]->mBones[0]->mArmature->mName) {
							Keys.Scales.emplace_back(ScaleKey.mTime, Vector3(1, 1, 1));
							SF_LOG(LogTest, Log, "Replacing bone scale animation of bone %s: %s with %s", scene->mMeshes[0]->mBones[0]->mNode->mName.C_Str(), vec.ToString().c_str(), Vector3(1, 1, 1).ToString().c_str())
						} else {
							Keys.Scales.emplace_back(ScaleKey.mTime, vec);
						}
					}

					Animation->Keys.push_back(Keys);
					break;
				}
			}
		}
	}

	target.Meshes[0]->GetVertexBuffer().SetVertexData(vertData, scene->mMeshes[0]->mNumVertices);
	delete[] vertData;
	delete[] BoneWeights;

	SF_LOG(Mesh Import, Log, "Mesh file imported: verts - %i, indices - %i", scene->mMeshes[0]->mNumVertices, indices.size())

	return true;
}

Skeleton::Bone MeshImporter::ConvertSkeletalNode(aiNode& Node, std::unordered_set<std::string>& BoneNames) {
	Skeleton::Bone bone;
	bone.Name = Node.mName.C_Str();
	bone.Transformation = {
		Node.mTransformation.a1, Node.mTransformation.b1, Node.mTransformation.c1, Node.mTransformation.d1,
		Node.mTransformation.a2, Node.mTransformation.b2, Node.mTransformation.c2, Node.mTransformation.d2,
		Node.mTransformation.a3, Node.mTransformation.b3, Node.mTransformation.c3, Node.mTransformation.d3,
		Node.mTransformation.a4, Node.mTransformation.b4, Node.mTransformation.c4, Node.mTransformation.d4,
	};

	for (int i = 0; i < Node.mNumChildren; ++i) {
		if (BoneNames.count(Node.mChildren[i]->mName.C_Str())) {
			bone.ChildBones.push_back(ConvertSkeletalNode(*Node.mChildren[i], BoneNames));
		}
	}

	return bone;
}