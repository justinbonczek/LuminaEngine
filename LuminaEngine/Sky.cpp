#include "Sky.h"
#include "Game.h"

#include <DDSTextureLoader.h>

namespace Lumina
{
	Sky::Sky(Mesh* mesh, ID3D11Device* dev) :
		GameObject(mesh)
	{
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_NONE;
		rd.FrontCounterClockwise = false;
		rd.DepthBias = 0;
		rd.DepthBiasClamp = 0;
		rd.SlopeScaledDepthBias = 0;
		rd.DepthClipEnable = true;
		rd.ScissorEnable = false;
		rd.MultisampleEnable = false;
		rd.AntialiasedLineEnable = false;
		dev->CreateRasterizerState(&rd, &rState);
	}

	Sky::~Sky()
	{
		ReleaseMacro(skymap);
		ReleaseMacro(rState);
	}

	void Sky::Update(float dt)
	{
		GameObject::Update(dt);
	}

	void Sky::Draw(ID3D11DeviceContext* devCon)
	{
		devCon->PSSetShaderResources(6, 1, &skymap);
		devCon->RSSetState(rState);
		GameObject::Draw(devCon);
	}

	void Sky::LoadCubeMap(wchar_t* filepath, ID3D11Device* dev)
	{
		HRESULT hr = CreateDDSTextureFromFile(dev, filepath, 0, &skymap);
		GameObject::SetSRV(skymap);
	}

	void Sky::SetMaterial(wchar_t* vertfilepath, wchar_t* pixelfilepath, ID3D11SamplerState* sampler, ID3D11Device* dev)
	{
		mat = new Material(vertfilepath, pixelfilepath, sampler, dev);
		mat->srv = skymap;
		GameObject::SetSampler(sampler);
	}
}