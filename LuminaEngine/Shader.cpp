#include "Shader.hpp"
#include <d3dcompiler.h>

NS_BEGIN

Shader::Shader() :
vert(),
pix(),
geo(),
comp(),
dom()
{

}

Shader::~Shader()
{
	if (vert)
		DELETECOM(vert);
	if (pix)
		DELETECOM(pix);
	if (geo)
		DELETECOM(geo);
	if (comp)
		DELETECOM(comp);
	if (dom)
		DELETECOM(dom);
}

bool Shader::LoadShader(wchar_t* filepath, ShaderType type, ID3D11Device* dev)
{
	ID3DBlob* fileToBlob;
	HRESULT hr = D3DReadFileToBlob(filepath, &fileToBlob);

	if (hr != S_OK)
		return false;

	HRESULT temp;
	switch (type)
	{
	case Vert:
		temp = dev->CreateVertexShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &vert);
		break;
	case Pixel:
		dev->CreatePixelShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &pix);
		break;
	case Geometry:
		dev->CreateGeometryShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &geo);
		break;
	case Compute:
		dev->CreateComputeShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &comp);
		break;
	case Domain:
		dev->CreateDomainShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &dom);
		break;
	}

	return true;
}

void Shader::SetShader(ShaderType type, ID3D11DeviceContext* devCon)
{
	switch (type)
	{
	case Vert:
		if (vert)
			devCon->VSSetShader(vert, NULL, 0);
		break;
	case Pixel:
		if (pix)
			devCon->PSSetShader(pix, NULL, 0);
		break;
	case Geometry:
		if (geo)
			devCon->GSSetShader(geo, NULL, 0);
		break;
	case Compute:
		if (comp)
			devCon->CSSetShader(comp, NULL, 0);
		break;
	case Domain:
		if (dom)
			devCon->DSSetShader(dom, NULL, 0);
		break;
	}
}

bool Shader::CheckLoaded(ShaderType type)
{
	switch (type)
	{
	case Vert:
		if (vert)
			return true;
		break;
	case Pixel:
		if (pix)
			return true;
		break;
	case Geometry:
		if (geo)
			return true;
		break;
	case Compute:
		if (comp)
			return true;
		break;
	case Domain:
		if (dom)
			return true;
		break;
	}

	return false;
}

NS_END