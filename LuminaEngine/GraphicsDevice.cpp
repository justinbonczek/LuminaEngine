#include "GraphicsDevice.hpp"

NS_BEGIN

GraphicsDevice::GraphicsDevice()
{
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
}

// TODO: Get Graphics info from a file
bool GraphicsDevice::Initialize(const Window& window)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = window.getWindowWidth();
	scd.BufferDesc.Height = window.getWindowHeight();
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window.getWindowHandle();
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.SampleDesc.Count = 4;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	featureLevel = D3D_FEATURE_LEVEL_11_0;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&dev,
		&featureLevel,
		&devCon);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"D3D11CreateDeviceAndSwapChain failed.", NULL, NULL);
		return false;
	}

	OnResize(window);
	return true;
}

void GraphicsDevice::OnResize(const Window& window)
{
	// Release old window elements
	DELETECOM(renderTargetView);
	DELETECOM(depthStencilView);
	DELETECOM(depthStencilBuffer);

	UINT windowWidth = window.getWindowWidth();
	UINT windowHeight = window.getWindowHeight();

	// Resize the swap chain
	swapChain->SetFullscreenState(window.getFullScreen(), NULL);
	swapChain->ResizeBuffers(
		1,
		windowWidth,
		windowHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		NULL);
	// Create a new back buffer and bind it as the new render target
	ID3D11Texture2D* backBuffer;
	swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	dev->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	DELETECOM(backBuffer);

	// Create a new Depth Stencil View and bind it
	D3D11_TEXTURE2D_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));
	dsd.Width = windowWidth;
	dsd.Height = windowHeight;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = NULL;
	dsd.MiscFlags = NULL;
	dsd.SampleDesc.Count = 4;

	dev->CreateTexture2D(&dsd, NULL, &depthStencilBuffer);
	dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	devCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Recreate the viewport and bind it
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)windowWidth;
	viewport.Height = (float)windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	devCon->RSSetViewports(1, &viewport);
}

void GraphicsDevice::Clear(XMFLOAT4 color)
{
	const float clearColor[4] = { color.x, color.y, color.z, color.w };
	devCon->ClearRenderTargetView(renderTargetView, clearColor);
	devCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsDevice::Clear(float r, float g, float b, float a)
{
	const float clearColor[4] = { r, g, b, a };
	devCon->ClearRenderTargetView(renderTargetView, clearColor);
	devCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsDevice::Clear(const float color[4])
{
	devCon->ClearRenderTargetView(renderTargetView, color);
	devCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsDevice::Display()
{
	swapChain->Present(0, 0);
}

void GraphicsDevice::BindBackBuffer()
{
	devCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void GraphicsDevice::SetShaderResource(UINT slot, ID3D11ShaderResourceView* srv)
{
	devCon->VSSetShaderResources(slot, 1, &srv);
	devCon->PSSetShaderResources(slot, 1, &srv);
}

void GraphicsDevice::SetDepthStencilState(DepthStencilState depthStencilState)
{
	depthStencilState.BindDepthStencilState(this);
}

ID3D11Device* GraphicsDevice::getDevice()
{
	return dev;
}

ID3D11DeviceContext* GraphicsDevice::getDeviceContext()
{
	return devCon;
}

float GraphicsDevice::getAspectRatio()
{
	return (float)viewport.Width / viewport.Height;
}

NS_END