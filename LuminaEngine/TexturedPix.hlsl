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
	return _Texture.Sample(_Sampler, input.uv);
}