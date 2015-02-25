#include "Material.hpp"
#include <WICTextureLoader.h>
#include "Window.hpp"

NS_BEGIN
Material::Material(wchar_t* filepath, ID3D11SamplerState* sampler, Window& window) :
sampler(sampler)
{
	tileUV[0] = 1;
	tileUV[1] = 1;
	CreateWICTextureFromFile(
		window.Device(),
		filepath,
		0,
		&srv);
	shader = new Shader();
}

Material::Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, Window& window)
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
	window.Device()->CreateSamplerState(&wsd, &sampler);

	shader = new Shader();
	shader->LoadShader(vertfilepath, Vert, window.Device());
	shader->LoadShader(pixelfilepath, Pixel, window.Device());
}

Material::Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, ID3D11SamplerState* _sampler, Window& window)
{
	tileUV[0] = 1;
	tileUV[1] = 1;
	shader = new Shader();
	shader->LoadShader(vertfilepath, Vert, window.Device());
	shader->LoadShader(pixelfilepath, Pixel, window.Device());
	sampler = _sampler;
}

Material::~Material()
{
	delete shader;
	delete lightMat;
	
	DELETECOM(srv);
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
	shader->SetShader(Vert, devCon);
	shader->SetShader(Pixel, devCon);
	shader->SetShader(Geometry, devCon);
	shader->SetShader(Compute, devCon);
	shader->SetShader(Domain, devCon);
}

void Material::BindSampler(ID3D11DeviceContext* devCon)
{
	if (sampler)
	{
		devCon->VSSetSamplers(0, 1, &sampler);
		devCon->PSSetSamplers(0, 1, &sampler);
	}
}

void Material::LoadTexture(wchar_t* texturefilepath, ID3D11Device* dev)
{
	CreateWICTextureFromFile(dev, texturefilepath, 0, &srv);
}

void Material::LoadNormal(wchar_t* texturefilepath, ID3D11Device* dev)
{
	CreateWICTextureFromFile(dev, texturefilepath, 0, &normalSrv);
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