#include <Skeleton.h>
#include <Math/Vector.h>
#include <Logging/Log.h>

Skeleton::Bone* Skeleton::Bone::FindBone(const std::string& findName) {
	if (Name == findName) {
		return this;
	}

	for (int i = 0; i < ChildBones.size(); ++i) {
		Bone* found = ChildBones[i].FindBone(findName);
		if (found) {
			return found;
		}
	}

	return nullptr;
}

size_t Skeleton::Bone::BoneTreeCount() const {
	size_t count = 1;

	for (int i = 0; i < ChildBones.size(); ++i) {
		count += ChildBones[i].BoneTreeCount();
	}

	return count;
}

Skeleton::Bone::BoneIterator& Skeleton::Bone::BoneIterator::operator++() {
	while (RefStack.size() > 0 && ++RefStack.top().ChildIndex >= RefStack.top().BoneRef->ChildBones.size()) {
		RefStack.pop();
	}

	if (RefStack.size() > 0) {
		RefStack.push({ &RefStack.top().BoneRef->ChildBones[RefStack.top().ChildIndex], -1});
	}

	return *this;
}

Skeleton::Bone::ConstBoneIterator& Skeleton::Bone::ConstBoneIterator::operator++() {
	while (RefStack.size() > 0 && ++RefStack.top().ChildIndex >= RefStack.top().BoneRef->ChildBones.size()) {
		RefStack.pop();
	}

	if (RefStack.size() > 0) {
		RefStack.push({ &RefStack.top().BoneRef->ChildBones[RefStack.top().ChildIndex], -1 });
	}

	return *this;
}

void Skeleton::GenerateDebugMesh(VertexBuffer& Buffer) {
	struct VertexData {
		Vector3 Position;
		VertexBuffer::IndexType BoneIndices[4] = { 0, 0, 0, 0 };
		float BoneWeights[4] = { 1, 0, 0, 0 };
	};

	Buffer.SetDescriptors({
		VertexBuffer::VertexDescriptor::Position,
		VertexBuffer::VertexDescriptor::Bone_Index,
		VertexBuffer::VertexDescriptor::Bone_Weight
	});

	Buffer.SetTopology(VertexBuffer::Topology::Line);

	std::vector<Vector3> Positions;
	CreatePositionTree(Positions, Matrix4x4(), RootBone);

	std::vector<const Bone*> allBones;
	for (Bone& bone : RootBone) {
		allBones.push_back(&bone);
	}

	std::vector<VertexData> data;
	data.resize(NumBones);

	for (int i = 0; i < NumBones; ++i) {
		data[i].Position = Positions[i];
		data[i].BoneIndices[0] = allBones[i]->BoneIndex;
	}
	
	Buffer.SetVertexData(data.data(), data.size());
	
	std::vector<VertexBuffer::IndexType> Indices;
	for (Bone& bone : RootBone) {
		if (bone == RootBone) continue;
		for (int i = 0; i < bone.ChildBones.size(); ++i) {
			Indices.emplace_back(bone.BoneIndex);
			Indices.emplace_back(bone.ChildBones[i].BoneIndex);
		}
	}

	Buffer.SetIndices(Indices.data(), Indices.size());
}

void Skeleton::GenerateAnimatedDebugMesh(VertexBuffer& Buffer, float Time) {
	struct VertexData {
		Vector3 Position;
		VertexBuffer::IndexType BoneIndices[4] = { 0, 0, 0, 0 };
		float BoneWeights[4] = { 1, 0, 0, 0 };
	};

	Buffer.SetDescriptors({
		VertexBuffer::VertexDescriptor::Position,
		VertexBuffer::VertexDescriptor::Bone_Index,
		VertexBuffer::VertexDescriptor::Bone_Weight
	});

	Buffer.SetTopology(VertexBuffer::Topology::Line);

	std::vector<Vector3> Positions;
	CreateAnimatedPositionTree(Positions, Matrix4x4(), RootBone, Time);

	std::vector<const Bone*> allBones;
	for (Bone& bone : RootBone) {
		allBones.push_back(&bone);
	}

	std::vector<VertexData> data;
	data.resize(NumBones);

	for (int i = 0; i < NumBones; ++i) {
		data[i].Position = Positions[i];
		data[i].BoneIndices[0] = allBones[i]->BoneIndex;
	}

	Buffer.SetVertexData(data.data(), data.size());

	std::vector<VertexBuffer::IndexType> Indices;
	for (Bone& bone : RootBone) {
		//if (bone == RootBone) continue;
		for (int i = 0; i < bone.ChildBones.size(); ++i) {
			Indices.emplace_back(bone.BoneIndex);
			Indices.emplace_back(bone.ChildBones[i].BoneIndex);
		}
	}

	Buffer.SetIndices(Indices.data(), Indices.size());
}

void Skeleton::SetupSkeleton(Bone& NewRootBone) {
	RootBone = NewRootBone;
	RootBone.Parent = nullptr;

	int count = 0;
	for (auto& bone : RootBone) {
		bone.BoneIndex = count++;
		
		for (int i = 0; i < bone.ChildBones.size(); ++i) {
			bone.ChildBones[i].Parent = &bone;
		}
	}

	Matrix4x4 outRot;
	Vector3 outTranslation, outScale;
	NewRootBone.Transformation.ExtractTransformComponents(outTranslation, outScale, outRot);
	SF_LOG(LogTest, Log, "%s", outScale.ToString().c_str())

	NumBones = count;
}

void Skeleton::CreatePositionTree(std::vector<Vector3>& Positions, Matrix4x4 CurrentMatrix, const Bone& bone) const {
	// todo: change to use a bone iterator since this function doesn't need hierarchy data
	if (bone.BoneIndex >= Positions.size()) {
		Positions.resize(NumBones);
	}
	
	Vector4 pos = { 0, 0, 0, 1 };
	pos = pos; //bone.InverseBind.Inverse()

	const Bone* ParentBone = &bone;
	Matrix4x4 FullTransform;
	while (ParentBone) {
		FullTransform = FullTransform * ParentBone->Transformation.Inverse();
		ParentBone = ParentBone->Parent;
	}

	Matrix4x4 NewMatrix = CurrentMatrix * bone.Transformation.Transpose();
	pos = FullTransform.Inverse() * pos;

	SF_LOG(Testing, Log, "Position: X %f, Y %f, Z %f", pos.x / pos.w, pos.y / pos.w, pos.z / pos.w)

	Positions[bone.BoneIndex] = Vector3(pos.x, pos.y, pos.z) / pos.w;
	
	for (int i = 0; i < bone.ChildBones.size(); ++i) {
		CreatePositionTree(Positions, NewMatrix, bone.ChildBones[i]);
	}
}

void Skeleton::CreateAnimatedPositionTree(std::vector<Vector3>& Positions, const Matrix4x4& CurrentMatrix, const Bone& bone, float Time) {
	// todo: change to use a bone iterator since this function doesn't need hierarchy data
	std::vector<Matrix4x4> AnimTransforms;
	Animation->Sample(Time, AnimTransforms);

	if (bone.BoneIndex >= Positions.size()) {
		Positions.resize(NumBones);
	}

	const Bone* ParentBone = &bone;
	Matrix4x4 FullTransform;
	while (ParentBone) {
		FullTransform = FullTransform * AnimTransforms[ParentBone->BoneIndex].Inverse();
		ParentBone = ParentBone->Parent;
	}

	Vector4 pos = FullTransform.Inverse() * (Vector4(0, 0, 0, 1));
	Matrix4x4 NewMatrix = CurrentMatrix * bone.Transformation;


	//pos = NewMatrix *  * bone.InverseBind * pos;

	Positions[bone.BoneIndex] = Vector3(pos.x, pos.y, pos.z) / pos.w;

	for (int i = 0; i < bone.ChildBones.size(); ++i) {
		CreateAnimatedPositionTree(Positions, NewMatrix, bone.ChildBones[i], Time);
	}
}