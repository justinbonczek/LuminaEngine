#include "Window.hpp"

NS_BEGIN

Window* window = 0;
/// <summary>
/// Window message callback
/// </summary>
LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return window->MsgProc(hwnd, msg, wParam, lParam);
}

Window::Window(HINSTANCE hInstance, UINT windowWidth, UINT windowHeight, std::wstring windowTitle):
hInstance(hInstance),
windowWidth(windowWidth),
windowHeight(windowHeight),
windowTitle(windowTitle),
isFullScreen(false),
isOpen(false),
hWnd(0),
dev(0),
devCon(0),
swapChain(0),
depthStencilBuffer(0),
renderTargetView(0),
depthStencilView(0)
{
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	window = this; 
}

Window::~Window()
{
	DELETECOM(depthStencilView);
	DELETECOM(depthStencilBuffer);
	DELETECOM(renderTargetView);
	DELETECOM(swapChain);

	if (devCon)
		devCon->ClearState();
	DELETECOM(devCon);
	DELETECOM(dev);
}

Window::Window(const Window& window)
{
	// TODO: Overload the Window class copy constructor
}

Window& Window::operator=(const Window& window)
{
	return *this;
}

bool Window::Initialize(void)
{
	if (!InitializeWindow())
	{
		printf("Failed to initialize window.");
		return false;
	}
	if (!InitializeDirect3D())
	{
		printf("Failed to initialize Direct3D.");
		return false;
	}
}

void Window::Close(void)
{
	Window::~Window();
}

bool Window::InitializeWindow(void)
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"WindowClassMain";
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.cbWndExtra = NULL;
	wc.cbClsExtra = NULL;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, L"RegisterClass failed.", NULL, NULL);
		return false;
	}

	RECT wr = { 0, 0, windowWidth, windowHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindow(
		L"WindowClassMain",
		windowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"CreateWindow Failed.", NULL, NULL);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	isOpen = true;
}

bool Window::InitializeDirect3D(void)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = windowWidth;
	scd.BufferDesc.Height = windowHeight;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.SampleDesc.Count = 4;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	featureLevel = D3D_FEATURE_LEVEL_9_1;
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

	OnResize();
	return true;
}

void Window::Clear(XMFLOAT4 color)
{
	const float clearColor[4] = { color.x, color.y, color.z, color.w };
	devCon->ClearRenderTargetView(renderTargetView, clearColor);
	devCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Window::Clear(float r, float g, float b, float a)
{
	const float clearColor[4] = { r, g, b, a };
	devCon->ClearRenderTargetView(renderTargetView, clearColor);
	devCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Window::Clear(const float color[4])
{
	devCon->ClearRenderTargetView(renderTargetView, color);
	devCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Window::Display(void)
{
	swapChain->Present(0, 0);
}

LRESULT Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		windowWidth = LOWORD(lParam);
		windowHeight = HIWORD(lParam);
		if (dev)
		{
			if (wParam = SIZE_MINIMIZED){}
			else
			{
				OnResize();
			}
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		return 0;
	case WM_EXITSIZEMOVE:
		OnResize();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void Window::OnResize(void)
{
	// Release old window elements
	DELETECOM(renderTargetView);
	DELETECOM(depthStencilView);
	DELETECOM(depthStencilBuffer);

	// Resize the swap chain
	swapChain->SetFullscreenState(isFullScreen, NULL);
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

void Window::SetFullScreen(bool state)
{
	isFullScreen = state;
	OnResize();
}

void Window::SetResolution(UINT width, UINT height)
{
	windowWidth = width;
	windowHeight = height;
	OnResize();
}

void Window::SetRenderTarget()
{
	devCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	devCon->RSSetViewports(1, &viewport);
}

float Window::AspectRatio(void)const
{
	return (float)windowWidth / windowHeight;
}

bool Window::IsOpen(void)
{
	return isOpen;
}

ID3D11Device* Window::Device(void)
{
	return dev;
}

ID3D11DeviceContext* Window::DeviceContext(void)
{
	return devCon;
}

ID3D11RenderTargetView* Window::BackBuffer()
{
	return renderTargetView;
}

ID3D11DepthStencilView* Window::DepthStencilView()
{
	return depthStencilView;
}

NS_END