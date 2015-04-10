#include "SamplerState.hpp"

NS_BEGIN


SamplerState::SamplerState(ID3D11Device* dev)
{
	D3D11_SAMPLER_DESC wsd;
	ZeroMemory(&wsd, sizeof(D3D11_SAMPLER_DESC));
	wsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	wsd.MaxAnisotropy = 0;
	wsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	wsd.MinLOD = 0;
	wsd.MaxLOD = 0;
	wsd.MipLODBias = 0;
	dev->CreateSamplerState(&wsd, &sampler);
}

SamplerState::SamplerState(FilterType fType, AddressType aType, ID3D11Device* dev)
{
	D3D11_SAMPLER_DESC wsd;
	ZeroMemory(&wsd, sizeof(D3D11_SAMPLER_DESC));

	switch (aType)
	{
	case Clamp:
		wsd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		wsd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		wsd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case Wrap:
		wsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		wsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		wsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	}
	switch(fType)
	{
	case Point:
		wsd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case Linear:
		wsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case Anisotropic:
		wsd.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	}
	wsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	wsd.MaxAnisotropy = 0;
	wsd.MinLOD = 0;
	wsd.MaxLOD = 0;
	wsd.MipLODBias = 0;
	dev->CreateSamplerState(&wsd, &sampler);
}

SamplerState::~SamplerState()
{
	delete sampler;
}

void SamplerState::BindSampler(ID3D11DeviceContext* devCon)
{
	if (sampler)
	{
		devCon->VSSetSamplers(0, 1, &sampler);
		devCon->PSSetSamplers(0, 1, &sampler);
	}
}

void SamplerState::BindSampler(UINT index, ID3D11DeviceContext* devCon)
{
	if (sampler)
	{
		devCon->VSSetSamplers(index, 1, &sampler);
		devCon->PSSetSamplers(index, 1, &sampler);
	}
}

ID3D11SamplerState* SamplerState::Sampler()
{
	return sampler;
}

NS_END