#include "Window.hpp"
#include "Game.hpp"

NS_BEGIN

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Game::GetInstance()->MsgProc(hwnd, msg, wParam, lParam);
}

Window::Window():
hInstance(0),
windowWidth(0),
windowHeight(0),
windowTitle(),
isFullScreen(false),
isOpen(false),
hWnd(0)
{}

Window::~Window()
{
}

bool Window::Initialize(HINSTANCE hInstance, UINT windowWidth, UINT windowHeight, std::wstring windowTitle)
{
	this->hInstance = hInstance;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->windowTitle = windowTitle;

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

void Window::Close(void)
{
	Window::~Window();
}

void Window::OnResize(LPARAM lParam)
{
	windowWidth = LOWORD(lParam);
	windowHeight = HIWORD(lParam);
}

float Window::getAspectRatio(void)const
{
	return (float)windowWidth / windowHeight;
}

bool Window::getOpen(void)const
{
	return isOpen;
}

UINT Window::getWindowWidth()const
{
	return windowWidth;
}

UINT Window::getWindowHeight()const
{
	return windowHeight;
}

HWND Window::getWindowHandle()const
{
	return hWnd;
}

bool Window::getFullScreen()const
{
	return isFullScreen;
}
NS_END