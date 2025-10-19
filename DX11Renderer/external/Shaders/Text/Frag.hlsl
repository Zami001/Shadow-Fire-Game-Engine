#include <TextMSDF.hlsl>

Texture2D diffuseTex : register(t0);
SamplerState TexSampler : register(s0);

struct PS_INPUT {
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange() {
    //vec2 unitRange = vec2(pxRange) / vec2(textureSize(msdf, 0));
    //vec2 screenTexSize = vec2(1.0) / fwidth(texCoord);
    //return max(0.5 * dot(unitRange, screenTexSize), 1.0);
    return 4;
}

float4 main(PS_INPUT In) : SV_TARGET {
    //float4 col = diffuseTex.Sample(TexSampler, float2(In.UV.x, In.UV.y));
    //return float4(0, 1, 0, 1);
    
    float4 sample = diffuseTex.Sample(TexSampler, In.UV);
    //return float4(sample.rgb, 1);
    //return sample.aaaa;
    
    float3 msd = diffuseTex.Sample(TexSampler, In.UV).rgb;
    //return sample;
    //return float4(1, 1, 1, 1);
    //return smoothstep(float4(msd, 1), float4(1, 1, 1, 1), 0.5);
    
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange() * (sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    
    float4 col = smoothstep(float4(0, 0, 0, sample.a), float4(1, 1, 1, sample.a), opacity);
    //return opacity;
    return col * sample.a;
}