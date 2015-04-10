#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Config.hpp"

NS_BEGIN

class Texture2D
{
public:
	Texture2D(wchar_t* filepath, ID3D11Device* dev);
	~Texture2D();

	void BindTexture(ID3D11DeviceContext* devCon);
	void BindTexture(UINT index, ID3D11DeviceContext* devCon);
private:
	ID3D11ShaderResourceView* srv;
};

NS_END

#endif