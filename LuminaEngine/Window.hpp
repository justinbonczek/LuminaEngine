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
	Window();
	~Window();

	/// <summary>
	/// Sets up a window and initializes Direct3D
	/// </summary>
	bool Initialize(HINSTANCE hInstance, UINT windowWidth = 800, UINT windowHeight = 600, std::wstring windowTitle = L"Windows Application");

	/// <summary>
	/// Closes the window
	/// </summary>
	void Close(void);

	/// <summary>
	/// Called by Graphics Manager when the window is resized
	/// </summary>
	void OnResize();

	float getAspectRatio(void)const;
	bool getOpen(void)const;
	UINT getWindowWidth()const;
	UINT getWindowHeight()const;
	HWND getWindowHandle()const;
	bool getFullScreen()const;
private:
	HINSTANCE hInstance;
	HWND	  hWnd;

	UINT windowWidth;
	UINT windowHeight;
	std::wstring windowTitle;
	bool isFullScreen;
	bool isOpen;
};

NS_END

#endif