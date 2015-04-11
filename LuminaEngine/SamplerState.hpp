#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "Config.hpp"
#include "GraphicsDevice.hpp"

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
	SamplerState(GraphicsDevice* graphicsDevice);
	SamplerState(FilterType fType, AddressType aType, GraphicsDevice* graphicsDevice);
	~SamplerState();

	void BindSampler(GraphicsDevice* graphicsDevice);
	void BindSampler(UINT index, GraphicsDevice* graphicsDevice);

	ID3D11SamplerState* Sampler();
private:
	ID3D11SamplerState* sampler;
};

NS_END

#endif