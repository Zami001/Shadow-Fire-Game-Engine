#pragma once

#include <DX11Renderer.h>
#include <Mesh.h>
#include <DX11Pipeline.h>
#include <DX11VertexBuffer.h>

class DX11_API DX11Mesh : public Mesh {
public:
	DX11Mesh();
	virtual ~DX11Mesh() = default;

	virtual void Render(const Matrix4x4& mvp, Material* mat) override;
	virtual void RenderSkeletal(const Matrix4x4& mvp, Material* mat, const SkeletalAnimation& Animation, float Time) override;
	virtual VertexBuffer& GetVertexBuffer() override;

private:
	DX11VertexBuffer VertBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> BoneConstantBuffer;

	void PrepareRender(const Matrix4x4 mvp, Material* mat);
	void SubmitRender();

	void UpdateBoneBuffer(const SkeletalAnimation& Animation, float Time);
};