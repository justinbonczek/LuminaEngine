#ifndef SHADER_HPP
#define SHADER_HPP

#include "Config.hpp"
#include "GraphicsDevice.hpp"

NS_BEGIN

enum ShaderType
{
	Vert,
	Geometry,
	GeometrySO,
	Pixel,
	Compute,
	Domain
};

class Shader
{
public:
	Shader();
	virtual ~Shader();

	static Shader ShadowShader;

	/// <summary>Loads a compiled shader (.cso, etc) and stores it in its proper DirectX shader object
	/// </summary>
	bool LoadShader(wchar_t* filepath, ShaderType type, GraphicsDevice* graphicsDevice);

	/// <summary>Binds the given shader type to the pipeline for use
	/// </summary>
	void BindShader(ShaderType type, GraphicsDevice* graphicsDevice);

	void BindShader(GraphicsDevice* graphicsDevice);

	void UnbindPixelShader(GraphicsDevice* graphicsDevice);
private:
	bool CheckLoaded(ShaderType type);

	ID3D11VertexShader* vert;
	ID3D11PixelShader*  pix;
	ID3D11GeometryShader* geo;
	ID3D11ComputeShader* comp;
	ID3D11DomainShader* dom;
};

NS_END

#endif