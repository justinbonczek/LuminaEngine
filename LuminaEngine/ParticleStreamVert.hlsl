struct Particle
{
	float3 initialPosition : POSITION;
	float3 initialVelocity : TEXCOORD0;
	float2 size			   : TEXCOORD1;
	float  age			   : TEXCOORD2;
	uint   type			   : TEXCOORD3;
};

Particle main(Particle input)
{
	return input;
}