#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Config.hpp"

#define PI 3.13149265359f

NS_BEGIN

struct Vertex
{
	Vertex(){}
	Vertex(XMFLOAT3 position, XMFLOAT2 uv){ Position = position; UV = uv; }
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT2 UV;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;
};

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
};
NS_END

#endif