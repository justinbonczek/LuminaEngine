cbuffer perFrame : register(b0)
{
	matrix view;
	matrix projection;
	float3 eyePos;
	float elapsedTime;
};

struct GSInput
{
	float4 position : SV_POSITION;
	float3 worldpos : POSITION0;
	float2 size		: TEXCOORD1;
	float4 color	: COLOR;
	uint   type		: TEXCOORD3;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
};

[maxvertexcount(4)]
void main(point GSInput input[1], inout TriangleStream<GSOutput> o)
{
	if (input[0].type != 1)
	{
		float3 look = normalize(eyePos - input[0].worldpos);
		float3 right = normalize(cross(float3(0, 1, 0), look));
		float3 up = cross(look, right);

		float halfW = 0.5f * input[0].size;
		float halfH = 0.5f * input[0].size;

		// New Vertex positions
		float4 v[4];

		v[0] = float4(input[0].worldpos + halfW * right - halfH * up, 1.0);
		v[1] = float4(input[0].worldpos + halfW * right + halfH * up, 1.0);
		v[2] = float4(input[0].worldpos - halfW * right - halfH * up, 1.0);
		v[3] = float4(input[0].worldpos - halfW * right + halfH * up, 1.0);

		// New Vertex UVs
		float2 quadUVs[4] = {
			float2(1, 1),
			float2(1, 0),
			float2(0, 1),
			float2(0, 0)
		};

		GSOutput vertex;

		[unroll]
		for (int i = 0; i < 4; i++)
		{
			vertex.position = mul(v[i], mul(view, projection));
			vertex.color = input[0].color;
			vertex.uv = quadUVs[i];

			o.Append(vertex);
		}
	}
}