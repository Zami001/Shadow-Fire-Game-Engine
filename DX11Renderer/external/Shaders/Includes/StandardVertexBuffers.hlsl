#include <Test.hlsl>

#ifndef _STANDARDBUFFERS
#define _STANDARDBUFFERS

cbuffer ModelViewProjectionConstantBuffer : register(b0) {
    matrix mvp;
};

cbuffer BoneBuffer : register(b1) {
    matrix Bones[256];
};

#endif