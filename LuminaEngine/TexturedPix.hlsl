#include "Lighting.hlsli"

cbuffer perObject : register(b1)
{
	matrix world;
	matrix worldInverseTranspose;
	LightMaterial lightMat;
	int tileX;
	int tileY;
	float pad[2];
}

struct PixelInput
{
	float4 position : SV_POSITION;
	float3 worldpos : POSITION0;
	float4 color    : COLOR;
	float2 uv		: TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
};

Texture2D _Texture : register(t0);
SamplerState _Sampler : register(s0);

float4 main(PixelInput input) : SV_TARGET
{
	return _Texture.Sample(_Sampler, float2(input.uv.x * tileX, input.uv.y * tileY));
}