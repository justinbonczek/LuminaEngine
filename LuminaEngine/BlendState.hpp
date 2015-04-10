#ifndef BLENDSTATE_HPP
#define BLENDSTATE_HPP

#include "Config.hpp"

NS_BEGIN

enum BlendType
{
	Transparency,
	NoTransparency,
	Additive, 
	Subtractive
};

class BlendState
{
public:
	BlendState(BlendType blendType, ID3D11Device* dev);
	~BlendState();

	/// <summary>
	///
	/// </summary>
	void BindBlendState(ID3D11DeviceContext* devCon);

private:
	ID3D11BlendState* blendState;
};

NS_END

#endif