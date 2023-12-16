#include <DX11Mesh.h>
#include <DX11Pipeline.h>
#include <DX11Material.h>
#include <DX11Shader.h>
#include <Texture/DX11Texture2D.h>

DX11Mesh::DX11Mesh() : Mesh() {
	// Create bone matrix buffer
	CD3D11_BUFFER_DESC cbDesc(
		sizeof(Matrix4x4) * 256,
		D3D11_BIND_CONSTANT_BUFFER
	);

	DX11Pipeline::device->CreateBuffer(
		&cbDesc,
		nullptr,
		BoneConstantBuffer.GetAddressOf()
	);
}

void DX11Mesh::Render(const Matrix4x4& mvp, Material* mat) {
	PrepareRender(mvp, mat);
	SubmitRender();
}

void DX11Mesh::RenderSkeletal(const Matrix4x4& mvp, Material* mat, const SkeletalAnimation& Animation, float Time) {
	PrepareRender(mvp, mat);
	UpdateBoneBuffer(Animation, Time);
	SubmitRender();
}

VertexBuffer& DX11Mesh::GetVertexBuffer() {
	return VertBuffer;
}

void DX11Mesh::PrepareRender(const Matrix4x4 mvp, Material* mat) {
	DX11Material* material = reinterpret_cast<DX11Material*>(mat);

	// update mvp matrix buffer
	DX11Pipeline::context->UpdateSubresource(
		material->ConstantBuffer.Get(),
		0,
		nullptr,
		&mvp,
		0,
		0
	);

	// set vertex buffer
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
	HRESULT result = DX11Pipeline::device->CreateInputLayout(
		VertBuffer.GetLayoutDescription(),
		VertBuffer.GetDescriptors().size(),
		reinterpret_cast<DX11Shader*>(material->vertex.Get())->CompiledShader->GetBufferPointer(),
		reinterpret_cast<DX11Shader*>(material->vertex.Get())->CompiledShader->GetBufferSize(),
		&InputLayout
	);

	DX11Pipeline::context->IASetInputLayout(InputLayout.Get());

	VertBuffer.Bind();

	for (int i = 0; i < material->Textures.size(); ++i) {
		DX11Pipeline::context->PSSetShaderResources(i, 1, reinterpret_cast<DX11Texture2D*>(material->Textures[i].Get())->SRV.GetAddressOf());
	}

	DX11Pipeline::context->VSSetShader(reinterpret_cast<DX11Shader*>(material->vertex.Get())->VertexShader.Get(), nullptr, 0);
	DX11Pipeline::context->VSSetConstantBuffers(0, 1, material->ConstantBuffer.GetAddressOf());
	DX11Pipeline::context->PSSetShader(reinterpret_cast<DX11Shader*>(material->fragment.Get())->PixelShader.Get(), nullptr, 0);
}

void DX11Mesh::SubmitRender() {
	DX11Pipeline::context->DrawIndexed(VertBuffer.GetNumIndices(), 0, 0);
}

void DX11Mesh::UpdateBoneBuffer(const SkeletalAnimation& Animation, float Time) {
	// update bone buffer
	if (GetSkeleton().Get()) {
		Matrix4x4* bonesBuffer = new Matrix4x4[256];
		std::vector<Matrix4x4> AnimTransforms;
		Animation.Sample(Time, AnimTransforms);

		for (const Skeleton::Bone& bone : GetSkeleton()->GetRootBone()) {
			const Skeleton::Bone* ParentBone = &bone;
			Matrix4x4 FullTransform;

			while (ParentBone) {
				FullTransform = AnimTransforms[ParentBone->BoneIndex] * FullTransform;
				ParentBone = ParentBone->Parent;
			}

			bonesBuffer[bone.BoneIndex] = FullTransform * bone.InverseBind;
		}

		DX11Pipeline::context->UpdateSubresource(
			BoneConstantBuffer.Get(),
			0,
			nullptr,
			bonesBuffer,
			0,
			0);

		delete[] bonesBuffer;

		DX11Pipeline::context->VSSetConstantBuffers(1, 1, BoneConstantBuffer.GetAddressOf());
	}
}
