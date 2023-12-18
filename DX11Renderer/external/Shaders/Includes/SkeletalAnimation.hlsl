#ifndef _SKELETALANIMATION
#define _SKELETALANIMATION

#include <StandardBuffers.hlsl>

float4 calculateVertexBonePosition(float4 position, uint4 BoneIndices, float4 BoneWeights) {
    matrix BlendedMatrix =
						Bones[BoneIndices[0]] * BoneWeights[0];
    BlendedMatrix +=    Bones[BoneIndices[1]] * BoneWeights[1];
    BlendedMatrix +=    Bones[BoneIndices[2]] * BoneWeights[2];
    BlendedMatrix +=    Bones[BoneIndices[3]] * BoneWeights[3];
    float4 outPos = mul(BlendedMatrix, position);
    outPos /= outPos.w;
    
    return outPos;
}

#endif