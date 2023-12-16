cbuffer ModelViewProjectionConstantBuffer : register(b0) {
	matrix mvp;
};

struct VS_INPUT {
	float3 vPos : POSITION;
};

struct VS_OUTPUT {
	float4 Position : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT Output;

    float4 pos = float4(input.vPos, 1.0f);
    Output.Position = mul(mvp, pos);;

    return Output;
}