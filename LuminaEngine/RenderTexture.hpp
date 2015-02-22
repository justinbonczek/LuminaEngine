/// UNDER CONSTRUCTION

#ifndef RENDERTEXTURE_HPP
#define RENDERTEXTURE_HPP

#include "Config.hpp"

NS_BEGIN

class RenderTexture
{
public:
	RenderTexture(UINT width, UINT height, ID3D11Device* dev);
	~RenderTexture();

	/// <summary>
	/// Sets the format of the render texture
	/// </summary>
	void SetFormat(DXGI_FORMAT format);
private:
	UINT width;
	UINT height;

	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* textureSrv;
	ID3D11DepthStencilView* dsv;

	D3D11_VIEWPORT viewport;
};

NS_END

#endif