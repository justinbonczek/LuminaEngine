#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "Config.hpp"

NS_BEGIN

enum FilterType
{
	Point,
	Linear,
	Anisotropic
};

enum AddressType
{
	Clamp,
	Wrap
};

enum ComparisonType
{
	Never,
	Less,
	Greater,
	LessEqual,
	GreaterEqual,
	Equal
};

class SamplerState
{
public:
	SamplerState(ID3D11Device* dev);
	SamplerState(FilterType fType, AddressType aType, ID3D11Device* dev);
	~SamplerState();

	void BindSampler(ID3D11DeviceContext* devCon);
	void BindSampler(UINT index, ID3D11DeviceContext* devCon);

	ID3D11SamplerState* Sampler();
private:
	ID3D11SamplerState* sampler;
};

NS_END

#endif