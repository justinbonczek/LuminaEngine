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

Texture2D _ShadowMap : register(t3);
SamplerState _Sampler : register(s0);
SamplerComparisonState _CmpSampler : register(s1);

float4 main(VertexOutput input) : SV_TARGET
{
	input.normal = normalize(input.normal);

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
		ComputeDirectionalLight(lightMat, dLight[i], input.normal, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	for (int i = 0; i < numPL; i++)
	{
		ComputePointLight(lightMat, pLight[i], input.worldpos, input.normal, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	for (int i = 0; i < numSL; i++)
	{
		ComputeSpotLight(lightMat, sLight[i], input.worldpos, input.normal, toEye, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	float percentLit = 0.0f;

	// Complete projection (if using perspective projection)
	for (int i = 0; i < numSM; i++)
	{
		input.shadowpos[i].xyz /= input.shadowpos[i].w;

		// Calculate distance between each pixel
		float dx = 1.0 / resolution;

		// PCF Filtering

		const float2 offsets[9] =
		{
			float2(-dx, -dx), float2(0.0f, -dx), float2(+dx, -dx),
			float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(+dx, 0.0f),
			float2(-dx, +dx), float2(0.0f, +dx), float2(+dx, +dx)
		};

		// Calculate Shadows
		float lightDepth = input.shadowpos[i].z;
		for (int j = 0; j < 9; j++)
		{
			float3 _sample = _ShadowMap.SampleCmpLevelZero(_CmpSampler, input.shadowpos[i].xy + offsets[j], lightDepth - 0.005);
			percentLit += _sample.r;
		}
	}
	
	percentLit /= (9.0 * numSM);

	// Calculate lit color based on lighting and shadow calculations
	float4 litColor = (ambient + diffuse) * percentLit * + spec * percentLit;
	//float4 litColor = ambient + diffuse + spec;

	// Pass through alpha values
	litColor.a = lightMat.diffuse.a;

	//return float4(litColor.r, 1.0, litColor.b, 1.0);
	return litColor;
}