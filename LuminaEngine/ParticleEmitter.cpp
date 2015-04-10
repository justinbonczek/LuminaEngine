#include "ParticleEmitter.hpp"

#include "Vertex.hpp"
#include "MeshGenerator.hpp"

NS_BEGIN

ParticleEmitter::ParticleEmitter(UINT maxParticles, Texture2D texture, BlendType blendType, ID3D11Device* dev) :
blendState(blendType, dev),
sampler(dev),
texture(texture),
maxParticles(maxParticles),
firstRun(true)
{
	streamOutShader.LoadShader(L"ParticleStreamVert.cso", Vert, dev);
	streamOutShader.LoadShader(L"ParticleGenerationGeo.cso", GeometrySO, dev);

	drawShader.LoadShader(L"DrawParticleVert.cso", Vert, dev);
	drawShader.LoadShader(L"ParticleBillboardGeo.cso", Geometry, dev);
	drawShader.LoadShader(L"DrawParticlePix.cso", Pixel, dev);

	// Create index and vertex buffers from data
	D3D11_BUFFER_DESC vb;
	ZeroMemory(&vb, sizeof(D3D11_BUFFER_DESC));
	vb.Usage = D3D11_USAGE_DEFAULT;
	vb.ByteWidth = sizeof(Particle) * maxParticles;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	vb.CPUAccessFlags = 0;
	vb.MiscFlags = 0;
	vb.StructureByteStride = 0;
	dev->CreateBuffer(&vb, 0, &streamBuffer);
	dev->CreateBuffer(&vb, 0, &drawBuffer);

	emitterParticle.initialPosition = XMFLOAT3(0.0, 0.0, 0.0);
	emitterParticle.initialVelocity = XMFLOAT3(0.0, 1.0, 0.0);
	emitterParticle.size = XMFLOAT2(1.0, 1.0);
	emitterParticle.age = 0.0f;
	emitterParticle.type = 1;

	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = &emitterParticle;
	dev->CreateBuffer(&vb, &srd, &initBuffer);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.ByteWidth = sizeof(emitterData);
	dev->CreateBuffer(&bd, 0, &particleBuffer);

	emitterData.emitPosition = XMFLOAT3(0.0, 0.0, 0.0);
	emitterData.emitDirection = XMFLOAT3(0.0, 1.0, 0.0);
	emitterData.particleAcceleration = XMFLOAT3(0.0, 7.8, 0.0);
}

ParticleEmitter::ParticleEmitter(UINT maxParticles, wchar_t* filepath, BlendType blendType, ID3D11Device* dev) :
blendState(blendType, dev),
sampler(dev),
texture(filepath, dev),
maxParticles(maxParticles),
firstRun(true)
{
	streamOutShader.LoadShader(L"ParticleStreamVert.cso", Vert, dev);
	streamOutShader.LoadShader(L"ParticleGenerationGeo.cso", GeometrySO, dev);

	drawShader.LoadShader(L"DrawParticleVert.cso", Vert, dev);
	drawShader.LoadShader(L"ParticleBillboardGeo.cso", Geometry, dev);
	drawShader.LoadShader(L"DrawParticlePix.cso", Pixel, dev);

	// Create vertex buffers
	D3D11_BUFFER_DESC vb;
	ZeroMemory(&vb, sizeof(D3D11_BUFFER_DESC));
	vb.Usage = D3D11_USAGE_DEFAULT;
	vb.ByteWidth = sizeof(Particle) * maxParticles;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	vb.CPUAccessFlags = 0;
	vb.MiscFlags = 0;
	vb.StructureByteStride = 0;
	dev->CreateBuffer(&vb, 0, &streamBuffer);
	dev->CreateBuffer(&vb, 0, &drawBuffer);

	emitterParticle.initialPosition = XMFLOAT3(0.0, 0.0, 0.0);
	emitterParticle.initialVelocity = XMFLOAT3(0.0, 1.0, 0.0);
	emitterParticle.size = XMFLOAT2(1.0, 1.0);
	emitterParticle.age = 0.0f;
	emitterParticle.type = 1;

	// Create initialization vertex buffer
	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = &emitterParticle;
	vb.ByteWidth = sizeof(Particle);
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dev->CreateBuffer(&vb, &srd, &initBuffer);

	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.ByteWidth = sizeof(emitterData);
	dev->CreateBuffer(&bd, 0, &particleBuffer);
	
	emitterData.emitPosition = XMFLOAT3(0.0, 0.0, 0.0);
	emitterData.emitDirection = XMFLOAT3(0.0, 1.0, 0.0);
	emitterData.particleAcceleration = XMFLOAT3(0.0, 7.8, 0.0);
}

ParticleEmitter::~ParticleEmitter()
{
	DELETECOM(particleBuffer);
	DELETECOM(initBuffer);
	DELETECOM(drawBuffer);
	DELETECOM(streamBuffer);
}

void ParticleEmitter::Update(float dt)
{

}

void ParticleEmitter::Draw(ID3D11DeviceContext* devCon)
{
	// TODO: Put this somewhere else, VERY slow
	devCon->VSSetConstantBuffers(4, 1, &particleBuffer);
	devCon->GSSetConstantBuffers(4, 1, &particleBuffer);
	devCon->PSSetConstantBuffers(4, 1, &particleBuffer);
	devCon->UpdateSubresource(particleBuffer, 0, 0, &emitterData, 0, 0);

	// Set up particle information
	blendState.BindBlendState(devCon);
	texture.BindTexture(devCon);
	sampler.BindSampler(devCon);

	UINT stride = sizeof(Particle);
	UINT offset = 0;

	// Use the init buffer if first time drawing/ just reset
	if (firstRun)
		devCon->IASetVertexBuffers(0, 1, &initBuffer, &stride, &offset);
	else
		devCon->IASetVertexBuffers(0, 1, &drawBuffer, &stride, &offset);

	// Bind the SO and SO shader
	devCon->SOSetTargets(1, &streamBuffer, &offset);

	streamOutShader.BindShader(devCon);

	// Run the stream out shader pass
	if (firstRun)
	{
		devCon->Draw(1, 0);
		firstRun = false;
	}
	else
		devCon->DrawAuto();

	// Unbind the Stream buffer
	ID3D11Buffer* bufferArray[1] = { 0 };
	devCon->SOSetTargets(1, bufferArray, &offset);

	// Swap the buffers
	std::swap(drawBuffer, streamBuffer);

	// Run the draw shader pass
	devCon->IASetVertexBuffers(0, 1, &drawBuffer, &stride, &offset);

	drawShader.BindShader(devCon);

	devCon->DrawAuto();
}

void ParticleEmitter::Reset()
{
	firstRun = true;
}

void ParticleEmitter::SetMaximumParticles(UINT max)
{
	maxParticles = max;
}

NS_END