#include "Sampler.hpp"

NS_BEGIN

Sampler::Sampler()
{

}

ID3D11SamplerState* Sampler::SamplerState()
{
	return sampler;
}

NS_END