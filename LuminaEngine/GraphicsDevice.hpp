#ifndef GRAPHICSDEVICE_HPP
#define GRAPHICSDEVICE_HPP

#include "Config.hpp"

#include "Window.hpp"

NS_BEGIN

class GraphicsDevice
{
public:
	GraphicsDevice();

	bool Initialize(const Window& window);

	void OnResize(const Window& window);

	void Clear(XMFLOAT4 color);
	void Clear(float r, float g, float b, float a);
	void Clear(const float color[4]);

	void Display();
	void BindBackBuffer();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	float getAspectRatio();
private:
	ID3D11Device*			dev;
	ID3D11DeviceContext*	devCon;
	IDXGISwapChain*			swapChain;
	ID3D11Texture2D*		depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT			viewport;
	D3D_FEATURE_LEVEL		featureLevel;
};

NS_END
#endif