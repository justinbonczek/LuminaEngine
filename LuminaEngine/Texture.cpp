#include "Texture.hpp"

#include <WICTextureLoader.h>

NS_BEGIN

Texture2D::Texture2D(wchar_t* filepath, GraphicsDevice* graphicsDevice)
{
	CreateWICTextureFromFile(graphicsDevice->getDevice(), filepath, 0, &srv);
	if (!srv)
	{
		printf("Failed to load texture: "); printf((char *)filepath); printf("\n");
	}
}

Texture2D::~Texture2D()
{
	DELETECOM(srv);
}

void Texture2D::BindTexture(GraphicsDevice* graphicsDevice)
{
	if (srv)
	{
		graphicsDevice->getDeviceContext()->VSSetShaderResources(0, 1, &srv);
		graphicsDevice->getDeviceContext()->PSSetShaderResources(0, 1, &srv);
	}
}

void Texture2D::BindTexture(UINT index, GraphicsDevice* graphicsDevice)
{
	if (srv)
	{
		graphicsDevice->getDeviceContext()->VSSetShaderResources(index, 1, &srv);
		graphicsDevice->getDeviceContext()->PSSetShaderResources(index, 1, &srv);
	}
}

NS_END