#include <Mesh.h>

void Mesh::SetSkeleton(SFSharedPtr<Skeleton> NewSkeleton) {
	SFSharedPtr<Skeleton> OldSkeleton = skeleton;
	skeleton = NewSkeleton;
	OnSkeletonAssigned(OldSkeleton, NewSkeleton);
}