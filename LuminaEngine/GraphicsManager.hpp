#ifndef GRAPHICSMANAGER_HPP
#define GRAPHICSMANAGER_HPP

#include "Config.hpp"
#include "Lumina.hpp"

NS_BEGIN

class GraphicsManager
{
public:
	~GraphicsManager();
	static GraphicsManager& getInstance();

	bool Initialize(UINT windowWidth = 800, UINT windowHeight = 600, std::wstring windowTitle = L"Windows Application");
	void Clear();
	void Display();
	void OnResize();

	GraphicsDevice* getActiveDevice();
private:
	GraphicsManager();

	// Prevent copies
	GraphicsManager(GraphicsManager const&) = delete;
	void operator=(GraphicsManager const&) = delete;

	float clearColor[4];

	// Later will allow for multiple windows/graphics devices
	Window window;
	GraphicsDevice graphicsDevice;
};

NS_END

#endif