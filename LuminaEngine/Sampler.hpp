#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "Config.hpp"

NS_BEGIN

class Sampler
{
public:
	Sampler();

	ID3D11SamplerState* SamplerState();
private:
	ID3D11SamplerState* sampler;
};

NS_END

#endif