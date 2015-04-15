#include "DepthStencilState.hpp"

#include "GraphicsDevice.hpp"

NS_BEGIN

DepthStencilState DepthStencilState::Default;
DepthStencilState DepthStencilState::NoDepth;

DepthStencilState::DepthStencilState()
{}

DepthStencilState::DepthStencilState(bool depthBufferReadEnable, bool depthBufferWriteEnable, GraphicsDevice* graphicsDevice)
{
	D3D11_DEPTH_STENCIL_DESC dsd;
	dsd.DepthEnable = depthBufferReadEnable;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.StencilEnable = FALSE;
	dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	graphicsDevice->getDevice()->CreateDepthStencilState(&dsd, &depthStencilState);
}

void DepthStencilState::Initialize(GraphicsDevice* graphicsDevice)
{
	Default = DepthStencilState(true, true, graphicsDevice);
	NoDepth = DepthStencilState(false, false, graphicsDevice);
}

void DepthStencilState::BindDepthStencilState(GraphicsDevice* graphicsDevice)
{
	graphicsDevice->getDeviceContext()->OMSetDepthStencilState(depthStencilState, 0);
}

NS_END