#include "ShadowMap.hpp"

NS_BEGIN

ShadowMap::ShadowMap(DirectionalLight* light, GraphicsDevice* graphicsDevice, UINT width, UINT height) :
dsv(0),
shadowMap(0),
width(width),
height(height),
light(light)
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
	graphicsDevice->getDevice()->CreateTexture2D(&td, 0, &depthMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvd.Flags = 0;
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;

	graphicsDevice->getDevice()->CreateDepthStencilView(depthMap, &dsvd, &dsv);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	graphicsDevice->getDevice()->CreateShaderResourceView(depthMap, &srvd, &shadowMap);

	DELETECOM(depthMap);
}

ShadowMap::ShadowMap(PointLight* light, GraphicsDevice* graphicsDevice, UINT width, UINT height) :
light(light)
{

}

ShadowMap::ShadowMap(SpotLight* light, GraphicsDevice* graphicsDevice, UINT width, UINT height) :
dsv(0),
shadowMap(0),
width(width),
height(height),
light(light)
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
	graphicsDevice->getDevice()->CreateTexture2D(&td, 0, &depthMap);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvd.Flags = 0;
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	
	graphicsDevice->getDevice()->CreateDepthStencilView(depthMap, &dsvd, &dsv);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = td.MipLevels;
	srvd.Texture2D.MostDetailedMip = 0;
	graphicsDevice->getDevice()->CreateShaderResourceView(depthMap, &srvd, &shadowMap);
	
	DELETECOM(depthMap);
}

ShadowMap::~ShadowMap()
{
	DELETECOM(shadowMap);
	DELETECOM(dsv);
}

void ShadowMap::SetSRVToShaders(GraphicsDevice* graphicsDevice)
{
	graphicsDevice->getDeviceContext()->VSSetShaderResources(3, 1, &shadowMap);
	graphicsDevice->getDeviceContext()->PSSetShaderResources(3, 1, &shadowMap);
}

void ShadowMap::BindDSVAndSetNullRenderTarget(GraphicsDevice* graphicsDevice)
{
	graphicsDevice->getDeviceContext()->RSSetViewports(1, &viewport);

	ID3D11RenderTargetView* renderTargets[1] = { 0 };
	graphicsDevice->getDeviceContext()->OMSetRenderTargets(1, renderTargets, dsv);

	graphicsDevice->getDeviceContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowMap::UpdateViewProjMatrix()
{
	if (DirectionalLight* d = dynamic_cast<DirectionalLight*>(light))
	{
		XMVECTOR lightDirection = XMVector3Normalize(XMLoadFloat3(&d->direction));
		XMVECTOR scaledDirection = lightDirection * 100;

		// Generate view and projection matrix
		XMStoreFloat4x4(&view, XMMatrixLookToLH(
			-scaledDirection,
			lightDirection,
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));

		XMStoreFloat4x4(&projection, XMMatrixOrthographicLH(100.0f, 100.0f, 0.1f, 200.0));
	}
	else if (PointLight* p = dynamic_cast<PointLight*>(light))
	{

	}
	else if (SpotLight* s = dynamic_cast<SpotLight*>(light))
	{
		XMVECTOR lightDirection = XMVector3Normalize(XMLoadFloat3(&s->direction));

		// Generate view and projection matrix
		XMStoreFloat4x4(&view, XMMatrixLookToLH(
			XMLoadFloat3(&s->position),
			lightDirection,
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));

		//XMStoreFloat4x4(&projection, XMMatrixPerspectiveFovLH(0.25f * PI, height / width, 0.1f, 100.0f));
		XMStoreFloat4x4(&projection, XMMatrixOrthographicLH(100.0f, 100.0f, 0.1f, 200.0));
	}
	else
	{
		printf("Attempted to update a Shadow Map without a light.");
	}
}

XMFLOAT4X4 ShadowMap::GetViewMatrix()
{
	return view;
}

XMFLOAT4X4 ShadowMap::GetViewMatrixTranspose()
{
	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, XMMatrixTranspose(XMLoadFloat4x4(&view)));
	return temp;
}

XMFLOAT4X4 ShadowMap::GetProjectionMatrix()
{
	return projection;
}

XMFLOAT4X4 ShadowMap::GetProjectionMatrixTranspose()
{
	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, XMMatrixTranspose(XMLoadFloat4x4(&projection)));
	return temp;
}

float ShadowMap::GetResolution()
{
	return (float)width;
}

ID3D11ShaderResourceView* ShadowMap::GetDepthMapSrv()
{
	return shadowMap;
}

NS_END