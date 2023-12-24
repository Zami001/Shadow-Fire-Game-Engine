#include <StandardVertexBuffers.hlsl>

struct VS_INPUT {
    float3 vPos : POSITION;
    float2 UV : TEXCOORD0;
};

struct VS_OUTPUT {
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT Output;

    Output.Position = float4(input.vPos, 1);
    Output.UV = input.UV;

    return Output;
}