#include "Texture.hpp"

#include <WICTextureLoader.h>

NS_BEGIN

Texture2D::Texture2D(wchar_t* filepath, ID3D11Device* dev)
{
	CreateWICTextureFromFile(dev, filepath, 0, &srv);
	if (!srv)
	{
		printf("Failed to load texture: "); printf((char *)filepath); printf("\n");
	}
}

Texture2D::~Texture2D()
{
	DELETECOM(srv);
}

void Texture2D::BindTexture(ID3D11DeviceContext* devCon)
{
	if (srv)
	{
		devCon->VSSetShaderResources(0, 1, &srv);
		devCon->PSSetShaderResources(0, 1, &srv);
	}
}

void Texture2D::BindTexture(UINT index, ID3D11DeviceContext* devCon)
{
	if (srv)
	{
		devCon->VSSetShaderResources(index, 1, &srv);
		devCon->PSSetShaderResources(index, 1, &srv);
	}
}

NS_END