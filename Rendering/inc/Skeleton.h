#pragma once

#include <Rendering.h>
#include <vector>
#include <Math/Matrix4x4.h>
#include <string>
#include <VertexBuffer.h>
#include <Math/Vector.h>
#include <stack>
#include <SkeletalAnimation.h>
#include <MinimalCore.h>

class RENDERING_API Skeleton {
	friend class MeshImporter;

public:
	struct RENDERING_API Bone {
		struct RENDERING_API BoneIterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type	= std::ptrdiff_t;
			using value_type		= Bone;
			using pointer			= value_type*;
			using reference			= value_type&;

		private:
			struct IterRef {
				Bone* BoneRef;
				int ChildIndex;
			};

			std::stack<IterRef> RefStack;

		public:
			BoneIterator(Bone* bone) { if(bone) RefStack.push({ bone, -1 }); }

			Bone& operator*() const { return *RefStack.top().BoneRef; }
			Bone* operator->() { return RefStack.top().BoneRef; }

			BoneIterator& operator++();
			explicit operator bool() { return RefStack.size() > 0; }
			friend bool operator==(const BoneIterator& left, const BoneIterator& right) {
				if (left.RefStack.size() > 0) {
					return left.RefStack.size() == right.RefStack.size() && left.RefStack.top().BoneRef == right.RefStack.top().BoneRef;
				} else {
					return right.RefStack.size() == 0;
				}
			}

			friend bool operator!=(const BoneIterator& left, const BoneIterator& right) {
				return !(left == right);
			}
		};

		struct RENDERING_API ConstBoneIterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Bone;
			using pointer = const value_type*;
			using reference = const value_type&;

		private:
			struct IterRef {
				const Bone* BoneRef;
				int ChildIndex;
			};

			std::stack<IterRef> RefStack;

		public:
			ConstBoneIterator(const Bone* bone) { if (bone) RefStack.push({ bone, -1 }); }

			const Bone& operator*() const { return *RefStack.top().BoneRef; }
			const Bone* operator->() { return RefStack.top().BoneRef; }

			ConstBoneIterator& operator++();
			explicit operator bool() { return RefStack.size() > 0; }
			friend bool operator==(const ConstBoneIterator& left, const ConstBoneIterator& right) {
				if (left.RefStack.size() > 0) {
					return left.RefStack.size() == right.RefStack.size() && left.RefStack.top().BoneRef == right.RefStack.top().BoneRef;
				} else {
					return right.RefStack.size() == 0;
				}
			}

			friend bool operator!=(const ConstBoneIterator& left, const ConstBoneIterator& right) {
				return !(left == right);
			}
		};

		std::string Name;
		std::vector<Bone> ChildBones;
		Bone* Parent;
		uint32_t BoneIndex;
		Matrix4x4 InverseBind;
		Matrix4x4 Transformation;

		Bone* FindBone(const std::string& findName);
		size_t BoneTreeCount() const;

		friend bool operator==(const Bone& left, const Bone& right) { return &left == &right; }
		friend bool operator!=(const Bone& left, const Bone& right) { return &left != &right; }
		BoneIterator begin() { return BoneIterator(this); }
		BoneIterator end() { return BoneIterator(nullptr); }
		ConstBoneIterator begin() const { return ConstBoneIterator(this); }
		ConstBoneIterator end() const { return ConstBoneIterator(nullptr); }
	};

public:
	Skeleton() = default;
	virtual ~Skeleton() = default;

	void GenerateDebugMesh(VertexBuffer& Buffer);
	void GenerateAnimatedDebugMesh(VertexBuffer& Buffer, float Time);
	void SetupSkeleton(Bone& NewRootBone);

private:
	Bone RootBone;
	size_t NumBones;

	void CreatePositionTree(std::vector<Vector3>& Positions, Matrix4x4 CurrentMatrix, const Bone& bone) const;
	void CreateAnimatedPositionTree(std::vector<Vector3>& Positions, const Matrix4x4& CurrentMatrix, const Bone& bone, float Time);

public:
	SFSharedPtr<SkeletalAnimation> Animation;
	inline size_t GetNumBones() const { return NumBones; }
	inline const Bone& GetRootBone() { return RootBone; }
};