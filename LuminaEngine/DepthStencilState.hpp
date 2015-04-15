#ifndef DEPTHSTENCILSTATE_HPP
#define DEPTHSTENCILSTATE_HPP

#include "Config.hpp"

NS_BEGIN

class GraphicsDevice;

class DepthStencilState
{
public:
	DepthStencilState();
	DepthStencilState(bool depthBufferReadEnable, bool depthBufferWriteEnable, GraphicsDevice* graphicsDevice);
	
	static DepthStencilState Default;
	static DepthStencilState NoDepth;

	static void Initialize(GraphicsDevice* graphicsDevice);

	void BindDepthStencilState(GraphicsDevice* graphicsDevice);
private:
	ID3D11DepthStencilState* depthStencilState;
};

NS_END

#endif