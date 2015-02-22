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

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 worldpos		: POSITION0;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

Texture2D _Texture : register(t0);
Texture2D _NormalMap : register(t1);
SamplerState _Sampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	// Unpack normal
	float3 normalT = _NormalMap.Sample(_Sampler, input.uv);
	normalT = 2.0f * normalT - 1.0f;

	// Calculate TBN
	float3 N = input.normal;
	float3 T = normalize(input.tangent - dot(input.tangent, N) *N);
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);
	float3 bumpedNormal = mul(normalT, TBN);
	bumpedNormal = normalize(bumpedNormal);

	// Calculate relation to camera for specularity and camera based effects
	float distToEye = length(eyePos - input.worldpos);
	float3 toEye = normalize(eyePos - input.worldpos);

	// Create light values and set them to zero
	float4 ambient = float4(0, 0, 0, 0);
	float4 diffuse = float4(0, 0, 0, 0);
	float4 spec = float4(0, 0, 0, 0);

	float4 A, D, S;

	///
	// Lighting Calculations
	///
	for (int i = 0; i < numDL; i++)
	{
		ComputeDirectionalLight(lightMat, dLight[i], bumpedNormal, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	for (int i = 0; i < numPL; i++)
	{
		ComputePointLight(lightMat, pLight[i], input.worldpos, bumpedNormal, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	for (int i = 0; i < numSL; i++)
	{
		ComputeSpotLight(lightMat, sLight[i], input.worldpos, bumpedNormal, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	// Sample texture(s)
	float4 texColor = _Texture.Sample(_Sampler, float2(input.uv.x, input.uv.y));

	// Calculate lit color based on lighting and shadow calculations
	float4 litColor = texColor * (ambient + diffuse) + spec;

	// Pass through alpha value
	litColor.a = lightMat.diffuse.a;

	return litColor;
}