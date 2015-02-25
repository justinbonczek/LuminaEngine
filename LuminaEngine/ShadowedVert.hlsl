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
	int tileU;
	int tileV;
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

cbuffer shadows : register(b3)
{
	matrix sView[7];
	matrix sProj[7];
	int numSM;
	float resolution;
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
	float4 position		: SV_POSITION;
	float3 worldpos		: POSITION0;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float4 shadowpos[7]	: TEXCOORD1;
};

VertexOutput main(VertexInput input)
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

	// Calculate projected texture position for the shadows
	for (int i = 0; i < numSM; i++)
	{
		matrix shadowTransform = mul(mul(world, sView[i]), sProj[i]);
		o.shadowpos[i] = mul(float4(input.position, 1.0), shadowTransform);
		o.shadowpos[i].xy = o.shadowpos[i].xy * 0.5 + 0.5;
		o.shadowpos[i].y = o.shadowpos[i].y * -1;
	}
	return o;
}