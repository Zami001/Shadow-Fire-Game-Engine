Texture2D diffuseTex : register(t0);
SamplerState TexSampler : register(s0);

struct PS_INPUT {
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

float4 main(PS_INPUT In) : SV_TARGET {
	return diffuseTex.Sample(TexSampler, float2(In.UV.x, In.UV.y));
}