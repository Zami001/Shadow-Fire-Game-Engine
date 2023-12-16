cbuffer ModelViewProjectionConstantBuffer : register(b0) {
	matrix mvp;
};

cbuffer BoneBuffer : register(b1) {
	matrix Bones[256];
};

struct VS_INPUT {
	float3 vPos : POSITION;
	float2 UV : TEXCOORD0;
	uint4 BoneIndex : BLENDINDICES;
	float4 BoneWeight : BLENDWEIGHT;
};

struct VS_OUTPUT {
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT Output;

    float4 pos = float4(input.vPos, 1.0f);
    Output.Color = float4((pos.x * 0.5) + 0.5, (pos.y * 0.5) + 0.5, (pos.z * 0.5) + 0.5, 1);
    float totalWeight = input.BoneWeight[0] + input.BoneWeight[1] + input.BoneWeight[2] + input.BoneWeight[3];
    matrix BlendedMatrix =
						Bones[input.BoneIndex[0]] * input.BoneWeight[0];
    BlendedMatrix += Bones[input.BoneIndex[1]] * input.BoneWeight[1];
    BlendedMatrix += Bones[input.BoneIndex[2]] * input.BoneWeight[2];
    BlendedMatrix += Bones[input.BoneIndex[3]] * input.BoneWeight[3];
    pos = mul(BlendedMatrix, pos);
    pos = pos / pos.w;
	
    pos = mul(mvp, pos);
    Output.Position = pos;

    Output.Color = float4(1, 1, 1, 1);
    Output.UV = input.UV;

    return Output;
}