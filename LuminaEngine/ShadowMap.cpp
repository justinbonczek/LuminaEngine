#include "ShadowMap.hpp"

NS_BEGIN

ShadowMap::ShadowMap(ID3D11Device* dev, UINT width, UINT height) :
dsv(0),
shadowMap(0),
width(width),
height(height)
{
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R24G8_TYPELESS;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	ID3D11Texture2D* depthMap = 0;
	dev->CreateTexture2D(&td, 0, &depthMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvd.Flags = 0;
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;

	dev->CreateDepthStencilView(depthMap, &dsvd, &dsv);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	dev->CreateShaderResourceView(depthMap, &srvd, &shadowMap);

	DELETECOM(depthMap);
}

ShadowMap::~ShadowMap()
{
	DELETECOM(shadowMap);
	DELETECOM(dsv);
}

ID3D11ShaderResourceView* ShadowMap::GetDepthMapSrv()
{
	return shadowMap;
}

void ShadowMap::SetSRVToShaders(ID3D11DeviceContext* devCon)
{
	devCon->VSSetShaderResources(3, 1, &shadowMap);
	devCon->PSSetShaderResources(3, 1, &shadowMap);
}

void ShadowMap::BindDSVAndSetNullRenderTarget(ID3D11DeviceContext* devCon)
{
	devCon->RSSetViewports(1, &viewport);

	ID3D11RenderTargetView* renderTargets[1] = { 0 };
	devCon->OMSetRenderTargets(1, renderTargets, dsv);

	devCon->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

NS_END