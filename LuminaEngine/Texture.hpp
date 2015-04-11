#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Config.hpp"
#include "GraphicsDevice.hpp"

NS_BEGIN

class Texture2D
{
public:
	Texture2D(wchar_t* filepath, GraphicsDevice* graphicsDevice);
	~Texture2D();

	void BindTexture(GraphicsDevice* graphicsDevice);
	void BindTexture(UINT index, GraphicsDevice* graphicsDevice);
private:
	ID3D11ShaderResourceView* srv;
};

NS_END

#endif