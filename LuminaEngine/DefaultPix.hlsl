struct PixelInput
{
	float4 position : SV_POSITION;
	float3 worldpos : POSITION0;
	float4 color    : COLOR;
	float2 uv		: TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
};

float4 main(PixelInput pIn) : SV_TARGET
{
	return float4(0.7f, 0.7f, 0.7f, 1.0f);
}