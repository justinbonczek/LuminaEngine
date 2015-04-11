#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Config.hpp"
#include "Shader.hpp"
#include "BlendState.hpp"
#include "GraphicsDevice.hpp"

NS_BEGIN

class Window;

struct LightMaterial
{
	LightMaterial() { ZeroMemory(this, sizeof(LightMaterial)); }
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;	
	XMFLOAT4 specular;
};

class Material
{
public:
	Material(GraphicsDevice* graphicsDevice);
	Material(BlendType blendType, GraphicsDevice* graphicsDevice);
	/// <summary>
	/// Uses the filepath of a texture to create a material
	/// </summary>
	Material(wchar_t* filepath, ID3D11SamplerState* sampler, GraphicsDevice* graphicsDevice);
	~Material();


	/// <summary>
	/// Uses the filepath of a vertex and a pixel shader to create a material
	/// </summary>
	Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, GraphicsDevice* graphicsDevice);
	Material(wchar_t* vertfilepath, wchar_t* pixelfilepath, ID3D11SamplerState* _sampler, GraphicsDevice* graphicsDevice);

	/// <summary>
	/// Binds the material's shader resource to the pipeline
	/// </summary>
	void BindSRV(GraphicsDevice* graphicsDevice);
	void BindSRV(UINT index, GraphicsDevice* graphicsDevice);

	/// <summary>
	/// Binds the material's shader to the pipeline
	/// </summary>
	void BindShader(GraphicsDevice* graphicsDevice);

	void UnbindPixelShader(GraphicsDevice* graphicsDevice);

	/// <summary>
	/// Binds the material's sampler to the pipeline
	/// </summary>
	void BindSampler(GraphicsDevice* graphicsDevice);

	void BindBlendState(GraphicsDevice* graphicsDevice);

	/// <summary>
	/// Loads a texture from the file and stores it into the srv
	/// </summary>
	void LoadTexture(wchar_t* texturefilepath, GraphicsDevice* graphicsDevice);

	/// <summary>
	/// Loads a normal map from the file and stores it
	/// </summary>
	void LoadNormal(wchar_t* texturefilepath, GraphicsDevice* graphicsDevice);

	void SetShader(Shader* shader);
	void SetShader(wchar_t* filepath, ShaderType type, GraphicsDevice* graphicsDevice);
	
	/// <summary>
	/// Sets the material's light-interaction values
	/// </summary>
	void SetLightMaterial(LightMaterial* _lightMat);

	/// <summary>
	/// Returns a copy of the light material
	/// </summary>
	LightMaterial GetLightMaterial(void);

	/// <summary>
	/// 
	/// </summary>
	void SetTextureTileU(UINT val);
	
	/// <summary>
	/// 
	/// </summary>
	void SetTextureTileV(UINT val);
	
	/// <summary>
	/// 
	/// </summary>
	void SetTextureTileUV(UINT val);
	
	/// <summary>
	/// 
	/// </summary>
	void SetTextureTileUV(UINT u, UINT v);

	/// <summary>
	/// 
	/// </summary>
	UINT GetTextureTileU();

	/// <summary>
	/// 
	/// </summary>
	UINT GetTextureTileV();

	/// <summary>
	/// Returns a pointer to the material's sampler
	/// </summary>
	ID3D11SamplerState* Sampler(void);
	ID3D11ShaderResourceView* SRV(void);
private:
	Shader* shader;
	LightMaterial* lightMat;
	UINT tileUV[2];

	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* normalSrv;
	ID3D11SamplerState* sampler;
	BlendState blendState;
};

NS_END

#endif