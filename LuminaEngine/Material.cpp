#include "Material.hpp"
#include <WICTextureLoader.h>
#include "Window.hpp"

NS_BEGIN
Material::Material(ID3D11Device* dev) :
blendState(Transparency, dev)
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
	dev->CreateSamplerState(&wsd, &sampler);
}

Material::Material(BlendType blendType, ID3D11Device* dev):
blendState(blendType, dev)
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
	dev->CreateSamplerState(&wsd, &sampler);

	lightMat = new LightMaterial();
}

Material::Material(wchar_t* filepath, ID3D11SamplerState* sampler, GraphicsDevice* graphicsDevice) :
sampler(sampler),
blendState(Transparency, graphicsDevice->getDevice())
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

Material::Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, ID3D11Device* dev):
blendState(Transparency, dev)
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
	dev->CreateSamplerState(&wsd, &sampler);

	shader = new Shader();
	shader->LoadShader(vertfilepath, Vert, dev);
	shader->LoadShader(pixelfilepath, Pixel, dev);
}

Material::Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, ID3D11SamplerState* _sampler, GraphicsDevice* graphicsDevice) :
blendState(Transparency, graphicsDevice->getDevice())
{
	tileUV[0] = 1;
	tileUV[1] = 1;
	shader = new Shader();
	shader->LoadShader(vertfilepath, Vert, graphicsDevice->getDevice());
	shader->LoadShader(pixelfilepath, Pixel, graphicsDevice->getDevice());
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

void Material::BindSRV(ID3D11DeviceContext* devCon)
{
	if (srv)
	{
		devCon->VSSetShaderResources(0, 1, &srv);
		devCon->PSSetShaderResources(0, 1, &srv);
	}
	if (normalSrv)
	{
		devCon->VSSetShaderResources(1, 1, &normalSrv);
		devCon->PSSetShaderResources(1, 1, &normalSrv);
	}
}

void Material::BindSRV(UINT index, ID3D11DeviceContext* devCon)
{
	if (srv)
	{
		devCon->VSSetShaderResources(index, 1, &srv);
		devCon->PSSetShaderResources(index, 1, &srv);
	}
}

void Material::BindShader(ID3D11DeviceContext* devCon)
{
	shader->BindShader(Vert, devCon);
	shader->BindShader(Pixel, devCon);
	shader->BindShader(Geometry, devCon);
	shader->BindShader(Compute, devCon);
	shader->BindShader(Domain, devCon);
}

void Material::UnbindPixelShader(ID3D11DeviceContext* devCon)
{
	shader->UnbindPixelShader(devCon);
}

void Material::BindSampler(ID3D11DeviceContext* devCon)
{
	if (sampler)
	{
		devCon->VSSetSamplers(0, 1, &sampler);
		devCon->PSSetSamplers(0, 1, &sampler);
	}
}

void Material::BindBlendState(ID3D11DeviceContext* devCon)
{
	blendState.BindBlendState(devCon);
}

void Material::LoadTexture(wchar_t* texturefilepath, ID3D11Device* dev)
{
	CreateWICTextureFromFile(dev, texturefilepath, 0, &srv);
	if (!srv)
	{
		printf("Failed to load texture: "); printf((char *)texturefilepath); printf("\n");
	}
}

void Material::LoadNormal(wchar_t* texturefilepath, ID3D11Device* dev)
{
	CreateWICTextureFromFile(dev, texturefilepath, 0, &normalSrv);
	if (!srv)
	{
		printf("Failed to load texture: "); printf((char *)texturefilepath); printf("\n");
	}
}

void Material::SetShader(Shader* shader)
{
	this->shader = shader;
}

void Material::SetShader(wchar_t* filepath, ShaderType type, ID3D11Device* dev)
{
	shader->LoadShader(filepath, type, dev);
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