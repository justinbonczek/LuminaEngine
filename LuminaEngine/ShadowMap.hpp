#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "Config.hpp"
#include "Light.hpp"
#include "GraphicsDevice.hpp"

NS_BEGIN

class ShadowMap
{
public:
	//ShadowMap(DirectionalLight* light, ID3D11Device* dev, UINT width = SHADOWMAPRESOLUTION, UINT height = SHADOWMAPRESOLUTION);
	ShadowMap(DirectionalLight* light, ID3D11Device* dev, UINT width = SHADOWMAPRESOLUTION, UINT height = SHADOWMAPRESOLUTION);
	ShadowMap(PointLight* light, ID3D11Device* dev, UINT width = SHADOWMAPRESOLUTION, UINT height = SHADOWMAPRESOLUTION);
	ShadowMap(SpotLight* light, ID3D11Device* dev, UINT width = SHADOWMAPRESOLUTION, UINT height = SHADOWMAPRESOLUTION);	
	~ShadowMap();

	/// <summary>Set the shadowmap to the pixel shader for shadow calculations
	/// </summary>
	void SetSRVToShaders(ID3D11DeviceContext* devCon);

	/// <summary>Sets up the render target for shadowmap rendering
	/// </summary>
	void BindDSVAndSetNullRenderTarget(GraphicsDevice* graphicsDevice);

	void UpdateViewProjMatrix();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetViewMatrixTranspose();
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT4X4 GetProjectionMatrixTranspose();
	float GetResolution();
	ID3D11ShaderResourceView* GetDepthMapSrv();
private:
	Light* light;

	UINT width;
	UINT height;

	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;

	ID3D11ShaderResourceView* shadowMap;
	ID3D11DepthStencilView* dsv;

	D3D11_VIEWPORT viewport;
};

NS_END

#endif