cbuffer perFrame : register(b0)
{
	matrix view;
	matrix projection;
	float3 eyePos;
	float elapsedTime;
};

cbuffer particles : register(b4)
{
	float3 emitPosition;
	float pad0;
	float3 emitDirection;
	float pad1;
	float3 acceleration;
	float pad2;
}

struct Particle
{
	float3 initialPosition : POSITION;
	float3 initialVelocity : TEXCOORD0;
	float2 size			   : TEXCOORD1;
	float  age			   : TEXCOORD2;
	uint   type			   : TEXCOORD3;
};


[maxvertexcount(2)]
void main(point Particle input[1], inout PointStream<Particle> o)
{
	input[0].age += elapsedTime;

	if (input[0].type == 1)
	{
		if (input[0].age >= 0.00005)
		{
			Particle p;
			p.initialPosition = float4(emitPosition, 1.0);
			// TODO: NO HARDCODING
			p.initialVelocity = emitDirection;
			p.size = float2(2.0, 2.0);
			p.age = 0.0;
			p.type = 0;

			o.Append(p);

			input[0].age = 0.0;
		}
		o.Append(input[0]);
	}
	else
	{
		if (input[0].age <= 1.0f)
			o.Append(input[0]);
	}
}