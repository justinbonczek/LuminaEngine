#include "Material.hpp"
#include <WICTextureLoader.h>
#include "Window.hpp"

NS_BEGIN
Material::Material(GraphicsDevice* graphicsDevice) :
blendState(Transparency, graphicsDevice)
{
	tileUV[0] = 1;
	tileUV[1] = 1;
	D3D11_SAMPLER_DESC wsd;
	ZeroMemory(&wsd, sizeof(D3D11_SAMPLER_DESC));
	wsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	wsd.MaxAnisotropy = 0;
	wsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	wsd.MinLOD = 0;
	wsd.MaxLOD = 0;
	wsd.MipLODBias = 0;
	graphicsDevice->getDevice()->CreateSamplerState(&wsd, &sampler);
}

Material::Material(BlendType blendType, GraphicsDevice* graphicsDevice) :
blendState(blendType, graphicsDevice)
{
	tileUV[0] = 1;
	tileUV[1] = 1;
	D3D11_SAMPLER_DESC wsd;
	ZeroMemory(&wsd, sizeof(D3D11_SAMPLER_DESC));
	wsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	wsd.MaxAnisotropy = 0;
	wsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	wsd.MinLOD = 0;
	wsd.MaxLOD = 0;
	wsd.MipLODBias = 0;
	graphicsDevice->getDevice()->CreateSamplerState(&wsd, &sampler);

	lightMat = new LightMaterial();
}

Material::Material(wchar_t* filepath, ID3D11SamplerState* sampler, GraphicsDevice* graphicsDevice) :
sampler(sampler),
blendState(Transparency, graphicsDevice)
{
	tileUV[0] = 1;
	tileUV[1] = 1;
	CreateWICTextureFromFile(
		graphicsDevice->getDevice(),
		filepath,
		0,
		&srv);
	shader = new Shader();
}

Material::Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, GraphicsDevice* graphicsDevice) :
blendState(Transparency, graphicsDevice)
{
	tileUV[0] = 1;
	tileUV[1] = 1;
	D3D11_SAMPLER_DESC wsd;
	ZeroMemory(&wsd, sizeof(D3D11_SAMPLER_DESC));
	wsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.Filter = D3D11_FILTER_ANISOTROPIC;
	wsd.MaxAnisotropy = 8;
	wsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	wsd.MinLOD = 0;
	wsd.MaxLOD = 0;
	wsd.MipLODBias = 0;
	graphicsDevice->getDevice()->CreateSamplerState(&wsd, &sampler);

	shader = new Shader();
	shader->LoadShader(vertfilepath, Vert, graphicsDevice);
	shader->LoadShader(pixelfilepath, Pixel, graphicsDevice);
}

Material::Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, ID3D11SamplerState* _sampler, GraphicsDevice* graphicsDevice) :
blendState(Transparency, graphicsDevice)
{
	tileUV[0] = 1;
	tileUV[1] = 1;
	shader = new Shader();
	shader->LoadShader(vertfilepath, Vert, graphicsDevice);
	shader->LoadShader(pixelfilepath, Pixel, graphicsDevice);
	sampler = _sampler;
}

Material::~Material()
{
	delete shader;
	delete lightMat;
	
	DELETECOM(srv);
	DELETECOM(normalSrv);
	DELETECOM(sampler);
}

void Material::BindSRV(GraphicsDevice* graphicsDevice)
{
	if (srv)
	{
		graphicsDevice->getDeviceContext()->VSSetShaderResources(0, 1, &srv);
		graphicsDevice->getDeviceContext()->PSSetShaderResources(0, 1, &srv);
	}
	if (normalSrv)
	{
		graphicsDevice->getDeviceContext()->VSSetShaderResources(1, 1, &normalSrv);
		graphicsDevice->getDeviceContext()->PSSetShaderResources(1, 1, &normalSrv);
	}
}

void Material::BindSRV(UINT index, GraphicsDevice* graphicsDevice)
{
	if (srv)
	{
		graphicsDevice->getDeviceContext()->VSSetShaderResources(index, 1, &srv);
		graphicsDevice->getDeviceContext()->PSSetShaderResources(index, 1, &srv);
	}
}

void Material::BindShader(GraphicsDevice* graphicsDevice)
{
	shader->BindShader(Vert, graphicsDevice);
	shader->BindShader(Pixel, graphicsDevice);
	shader->BindShader(Geometry, graphicsDevice);
	shader->BindShader(Compute, graphicsDevice);
	shader->BindShader(Domain, graphicsDevice);
}

void Material::UnbindPixelShader(GraphicsDevice* graphicsDevice)
{
	shader->UnbindPixelShader(graphicsDevice);
}

void Material::BindSampler(GraphicsDevice* graphicsDevice)
{
	if (sampler)
	{
		graphicsDevice->getDeviceContext()->VSSetSamplers(0, 1, &sampler);
		graphicsDevice->getDeviceContext()->PSSetSamplers(0, 1, &sampler);
	}
}

void Material::BindBlendState(GraphicsDevice* graphicsDevice)
{
	blendState.BindBlendState(graphicsDevice);
}

void Material::LoadTexture(wchar_t* texturefilepath, GraphicsDevice* graphicsDevice)
{
	CreateWICTextureFromFile(graphicsDevice->getDevice(), texturefilepath, 0, &srv);
	if (!srv)
	{
		printf("Failed to load texture: "); printf((char *)texturefilepath); printf("\n");
	}
}

void Material::LoadNormal(wchar_t* texturefilepath, GraphicsDevice* graphicsDevice)
{
	CreateWICTextureFromFile(graphicsDevice->getDevice(), texturefilepath, 0, &normalSrv);
	if (!srv)
	{
		printf("Failed to load texture: "); printf((char *)texturefilepath); printf("\n");
	}
}

void Material::SetShader(Shader* shader)
{
	this->shader = shader;
}

void Material::SetShader(wchar_t* filepath, ShaderType type, GraphicsDevice* graphicsDevice)
{
	shader->LoadShader(filepath, type, graphicsDevice);
}

void Material::SetLightMaterial(LightMaterial* _lightMat)
{
	lightMat = _lightMat;
}

LightMaterial Material::GetLightMaterial(void)
{
	return *lightMat;
}

void Material::SetTextureTileU(UINT val)
{
	tileUV[0] = val;
}

void Material::SetTextureTileV(UINT val)
{
	tileUV[1] = val;
}

void Material::SetTextureTileUV(UINT val)
{
	tileUV[0] = val;
	tileUV[1] = val;
}

void Material::SetTextureTileUV(UINT u, UINT v)
{
	tileUV[0] = u;
	tileUV[1] = v;
}

UINT Material::GetTextureTileU()
{
	return tileUV[0];
}

UINT Material::GetTextureTileV()
{
	return tileUV[1];
}

ID3D11SamplerState* Material::Sampler(void)
{
	return sampler;
}

ID3D11ShaderResourceView* Material::SRV(void)
{
	return srv;
}

NS_END