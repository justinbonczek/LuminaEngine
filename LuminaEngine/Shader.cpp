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

bool Shader::LoadShader(wchar_t* filepath, ShaderType type, GraphicsDevice* graphicsDevice)
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
		graphicsDevice->getDevice()->CreateVertexShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &vert);
		break;
	case Pixel:
		graphicsDevice->getDevice()->CreatePixelShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &pix);
		break;
	case Geometry:
		graphicsDevice->getDevice()->CreateGeometryShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &geo);
		break;
	case GeometrySO:
		graphicsDevice->getDevice()->CreateGeometryShaderWithStreamOutput(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), pVDecl, 5, 0, 0, 0, 0, &geo);
		break;
	case Compute:
		graphicsDevice->getDevice()->CreateComputeShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &comp);
		break;
	case Domain:
		graphicsDevice->getDevice()->CreateDomainShader(fileToBlob->GetBufferPointer(), fileToBlob->GetBufferSize(), NULL, &dom);
		break;
	}

	return true;
}

void Shader::BindShader(ShaderType type, GraphicsDevice* graphicsDevice)
{
	switch (type)
	{
	case Vert:
		//if (vert)
			graphicsDevice->getDeviceContext()->VSSetShader(vert, NULL, 0);
		break;
	case Pixel:
		//if (pix)
			graphicsDevice->getDeviceContext()->PSSetShader(pix, NULL, 0);
		break;
	case Geometry:
		//if (geo)
			graphicsDevice->getDeviceContext()->GSSetShader(geo, NULL, 0);
		break;
	case Compute:
		//if (comp)
			graphicsDevice->getDeviceContext()->CSSetShader(comp, NULL, 0);
		break;
	case Domain:
		//if (dom)
			graphicsDevice->getDeviceContext()->DSSetShader(dom, NULL, 0);
		break;
	}
}

void Shader::BindShader(GraphicsDevice* graphicsDevice)
{
	graphicsDevice->getDeviceContext()->VSSetShader(vert, NULL, 0);
	graphicsDevice->getDeviceContext()->PSSetShader(pix, NULL, 0);
	graphicsDevice->getDeviceContext()->GSSetShader(geo, NULL, 0);
	graphicsDevice->getDeviceContext()->CSSetShader(comp, NULL, 0);
	graphicsDevice->getDeviceContext()->DSSetShader(dom, NULL, 0);
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

void Shader::UnbindPixelShader(GraphicsDevice* graphicsDevice)
{
	graphicsDevice->getDeviceContext()->PSSetShader(0, 0, 0);
}

NS_END