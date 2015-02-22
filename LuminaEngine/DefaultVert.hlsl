#include "Lighting.hlsli"

cbuffer perFrame : register(b0)
{
	matrix view;
	matrix projection;
	float3 eyePos;
	float elapsedTime;
};

cbuffer perObject : register(b1)
{
	matrix world;
	matrix worldInverseTranspose;
	LightMaterial lightMat;
}

cbuffer lights : register(b2)
{
	DirectionalLight dLight[1];
	PointLight pLight[3];
	SpotLight sLight[3];
	int numDL;
	int numPL;
	int numSL;
} 

struct VertexInput
{
	float3 position : POSITION;
	float4 color    : COLOR;
	float2 uv		: TEXCOORD0;
	float3 normal   : NORMAL;
	float4 tangent  : TANGENT;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
	float3 worldpos : POSITION0;
	float4 color    : COLOR;
	float2 uv		: TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
};

VertexOutput main( VertexInput input )
{
	VertexOutput o;

	// Calculate wvp matrix
	matrix worldViewProj = mul(mul(world, view), projection);

	// Apply wvp matrix to input coordinates to get screen coordinates
	o.position = mul(float4(input.position, 1.0), worldViewProj);

	o.worldpos = mul(float4(input.position, 1.0), world).xyz;

	// Normal/ Tangent calculation
	o.normal = mul(input.normal, (float3x3)worldInverseTranspose);
	o.tangent = mul(input.tangent, (float3x3)world);

	// Pass through values
	o.color = input.color;
	o.uv = input.uv;
	return o;	
}