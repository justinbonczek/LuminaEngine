cbuffer particles : register(b4)
{
	float3 initialPosition;
	float size;
	float3 initialVelocity;
	float age;
	float3 acceleration;
	float life;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
};

Texture2D _Texture : register(t0);
SamplerState _Sampler : register(s0);

float4 main(PixelInput input) : SV_TARGET
{
	return _Texture.Sample(_Sampler, input.uv) * input.color;
}