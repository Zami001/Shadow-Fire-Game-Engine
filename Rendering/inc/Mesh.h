#pragma once

#include <Rendering.h>
#include <Math/Matrix4x4.h>
#include <Material.h>
#include <VertexBuffer.h>
#include <Skeleton.h>

class RENDERING_API Mesh {
protected:
	Mesh() = default;

public:
	virtual ~Mesh() = default;

	virtual void Render(const Matrix4x4& mvp, Material* mat) = 0;
	virtual void RenderSkeletal(const Matrix4x4& mvp, Material* mat, const SkeletalAnimation& Animation, float Time) = 0;
	virtual VertexBuffer& GetVertexBuffer() = 0;

private:
	SFSharedPtr<Skeleton> skeleton = nullptr;

public:
	void SetSkeleton(SFSharedPtr<Skeleton> NewSkeleton);
	SFSharedPtr<Skeleton> GetSkeleton() { return skeleton; }

protected:
	virtual void OnSkeletonAssigned(SFSharedPtr<Skeleton> OldSkeleton, SFSharedPtr<Skeleton> NewSkeleton) {}
};