#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "Config.hpp"

NS_BEGIN

class ShadowMap
{
public:
	ShadowMap(ID3D11Device* dev, UINT width, UINT height);
	~ShadowMap();

	ID3D11ShaderResourceView* GetDepthMapSrv();

	/// <summary>Set the shadowmap to the pixel shader for shadow calculations
	/// </summary>
	void SetSRVToShaders(ID3D11DeviceContext* devCon);

	/// <summary>Sets up the render target for shadowmap rendering
	/// </summary>
	void BindDSVAndSetNullRenderTarget(ID3D11DeviceContext* devCon);
private:
	UINT width;
	UINT height;

	ID3D11ShaderResourceView* shadowMap;
	ID3D11DepthStencilView* dsv;

	D3D11_VIEWPORT viewport;
};

NS_END

#endif