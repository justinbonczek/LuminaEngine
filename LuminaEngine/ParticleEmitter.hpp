#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include "Config.hpp"

#include "GameObject.hpp"

#include "SamplerState.hpp"
#include "BlendState.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

NS_BEGIN

struct Particle
{
	Particle(){}
	XMFLOAT3 initialPosition;
	XMFLOAT3 initialVelocity;
	XMFLOAT2 size;
	float age;
	UINT type;
};

struct EmitterData
{
	XMFLOAT3 emitPosition;
	float pad1;
	XMFLOAT3 emitDirection;
	float pad2;
	XMFLOAT3 particleAcceleration;
	float pad3;
};

class ParticleEmitter : public GameObject
{
public:
	ParticleEmitter(UINT maxParticles, Texture2D texture, BlendType blendType, GraphicsDevice* graphicsDevice);
	ParticleEmitter(UINT maxParticles, wchar_t* filepath, BlendType blendType, GraphicsDevice* graphicsDevice);
	~ParticleEmitter();

	void Update(float dt);
	void Draw(GraphicsDevice* graphicsDevice);

	void Reset();

	void SetMaximumParticles(UINT max);
private:
	Particle emitterParticle;

	EmitterData emitterData;
	ID3D11Buffer* particleBuffer;

	ID3D11Buffer* initBuffer;
	ID3D11Buffer* drawBuffer;
	ID3D11Buffer* streamBuffer;

	bool firstRun;
	UINT maxParticles;

	BlendState blendState;
	SamplerState sampler;
	Shader streamOutShader;
	Shader drawShader;
	Texture2D texture;
};

NS_END

#endif