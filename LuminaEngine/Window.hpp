#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Config.hpp"

NS_BEGIN

/// <summary>
/// Windows Application created for a DirectX 11 Application
/// </summary>
class Window
{
public:
	Window(HINSTANCE hInstance, UINT windowWidth = 800, UINT windowHeight = 600, std::wstring windowTitle = L"Windows Application");
	~Window();
	Window(const Window& window);
	Window& operator=(const Window& window);

	/// <summary>
	/// Handles Windows messages
	/// </summary>
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// Sets up a window and initializes Direct3D
	/// </summary>
	bool Initialize(void);

	/// <summary>
	/// Closes the window
	/// </summary>
	void Close(void);

	/// <summary>
	/// Handles the destruction and recreation of window elements when the window is resized
	/// </summary>
	void OnResize(void);

	/// <summary>
	/// Clears the back buffer to the given color or black by default
	/// </summary>
	void Clear(XMFLOAT4 color = XMFLOAT4(0.0, 0.0, 0.0, 1.0));
	void Clear(float r, float g, float b, float a);
	void Clear(const float color[4]);

	/// <summary>
	/// Updates and displays window
	/// </summary>
	void Display(void);

	/// <summary>
	/// Sets the window to fullscreen mode, or exits if false is supplied as a parameter
	/// </summary>
	void SetFullScreen(bool state = true);

	/// <summary>
	/// Sets the resolution of the window to the specified parameters
	/// </summary>
	void SetResolution(UINT width, UINT height);

	/// <summary>
	/// Returns whether or not the window is currently open
	/// </summary>
	bool IsOpen(void);

	/// <summary>
	/// Calculates the aspect ratio of a window
	/// </summary>
	float AspectRatio(void)const;

	/// <summary>
	/// Returns a pointer to the device
	/// </summary>
	ID3D11Device* Device(void);

	/// <summary>
	/// Returns a pointer to the device context
	/// </summary>
	ID3D11DeviceContext* DeviceContext(void);

	/// <summary>
	/// Returns a pointer to the back buffer
	/// </summary>
	ID3D11RenderTargetView* BackBuffer(void);

	/// <summary>
	/// Returns a pointer to the depth stencil view
	/// </summary>
	ID3D11DepthStencilView* DepthStencilView(void);
private:
	/// <summary>
	/// Handles window initialization in Initialize(void)
	/// </summary>
	bool InitializeWindow(void);

	/// <summary>
	/// Handles Direct3D initialization in Initialize(void)
	/// </summary>
	bool InitializeDirect3D(void);

	HINSTANCE hInstance;
	HWND	  hWnd;

	ID3D11Device*			dev;
	ID3D11DeviceContext*	devCon;
	IDXGISwapChain*			swapChain;
	ID3D11Texture2D*		depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT			viewport;
	D3D_FEATURE_LEVEL		featureLevel;

	UINT windowWidth;
	UINT windowHeight;
	std::wstring windowTitle;
	bool isFullScreen;
	bool isOpen;
};

NS_END

#endif