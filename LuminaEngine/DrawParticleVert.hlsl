cbuffer particles : register(b4)
{
	float3 emitPosition;
	float3 emitDirection;
	float3 acceleration;
}

struct Particle
{
	float3 initialPosition : POSITION;
	float3 initialVelocity : TEXCOORD0;
	float2 size			   : TEXCOORD1;
	float age			   : TEXCOORD2;
	uint type			   : TEXCOORD3;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
	float3 worldpos : POSITION0;
	float2 size		: TEXCOORD1;
	float4 color	: COLOR;
	uint   type		: TEXCOORD3;
};

VertexOutput main(Particle input)
{
	VertexOutput o;

	o.position = float4(0, 0, 0, 0.0);

	float t = input.age;

	o.worldpos = input.initialPosition + t * input.initialVelocity + 0.5f * t * t * acceleration;

	float alpha = 1.0 - smoothstep(0.0, 1.0, t / 1.0);
	o.color = float4(1.0, 1.0, 1.0, alpha);

	o.type = input.type;

	return o;
}