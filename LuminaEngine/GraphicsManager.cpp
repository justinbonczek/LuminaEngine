#include "GraphicsManager.hpp"

NS_BEGIN

GraphicsManager::GraphicsManager()
{
	clearColor[0] = 0.0f;
	clearColor[1] = 0.0f;
	clearColor[2] = 0.0f;
	clearColor[3] = 1.0f;
}

GraphicsManager::~GraphicsManager()
{
}

GraphicsManager& GraphicsManager::getInstance()
{
	static GraphicsManager instance;

	return instance;
}

bool GraphicsManager::Initialize(UINT windowWidth, UINT windowHeight, std::wstring windowTitle)
{
	if (!window.Initialize(GetModuleHandle(NULL), windowWidth, windowHeight, windowTitle) )
		return false;
	if (!graphicsDevice.Initialize(window))
		return false;
}

void GraphicsManager::Clear()
{
	graphicsDevice.Clear(clearColor);
}

void GraphicsManager::Display()
{
	graphicsDevice.Display();
}

void GraphicsManager::OnSize(LPARAM lParam)
{
	window.OnResize(lParam);
	graphicsDevice.OnResize(window);
}

void GraphicsManager::OnResizeExit()
{
	graphicsDevice.OnResize(window);
}

GraphicsDevice* GraphicsManager::getActiveDevice()
{
	return &graphicsDevice;
}
NS_END