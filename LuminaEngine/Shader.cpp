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

	D3D11_SO_DECLARATION_ENTRY pVDecl[] =
	{
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "TEXCOORD", 0, 0, 3, 0 },
		{ 0, "TEXCOORD", 1, 0, 2, 0 },
		{ 0, "TEXCOORD", 2, 0, 1, 0 },
		{ 0, "TEXCOORD", 3, 0, 1, 0 },
	};

	UINT strides = 0;

	switch (type)
	{
	case Vert:
		dev->CreateVertexShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &vert);
		break;
	case Pixel:
		dev->CreatePixelShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &pix);
		break;
	case Geometry:
		dev->CreateGeometryShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &geo);
		break;
	case GeometrySO:
		dev->CreateGeometryShaderWithStreamOutput(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), pVDecl, 5, 0, 0, 0, 0, &geo);
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

void Shader::BindShader(ShaderType type, ID3D11DeviceContext* devCon)
{
	switch (type)
	{
	case Vert:
		//if (vert)
			devCon->VSSetShader(vert, NULL, 0);
		break;
	case Pixel:
		//if (pix)
			devCon->PSSetShader(pix, NULL, 0);
		break;
	case Geometry:
		//if (geo)
			devCon->GSSetShader(geo, NULL, 0);
		break;
	case Compute:
		//if (comp)
			devCon->CSSetShader(comp, NULL, 0);
		break;
	case Domain:
		//if (dom)
			devCon->DSSetShader(dom, NULL, 0);
		break;
	}
}

void Shader::BindShader(ID3D11DeviceContext* devCon)
{
	devCon->VSSetShader(vert, NULL, 0);
	devCon->PSSetShader(pix, NULL, 0);
	devCon->GSSetShader(geo, NULL, 0);
	devCon->CSSetShader(comp, NULL, 0);
	devCon->DSSetShader(dom, NULL, 0);
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

void Shader::UnbindPixelShader(ID3D11DeviceContext* devCon)
{
	devCon->PSSetShader(0, 0, 0);
}

NS_END