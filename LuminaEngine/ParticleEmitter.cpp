#include "ParticleEmitter.hpp"

#include "Vertex.hpp"
#include "MeshGenerator.hpp"

NS_BEGIN

ParticleEmitter::ParticleEmitter(UINT maxParticles, Texture2D texture, BlendType blendType, GraphicsDevice* graphicsDevice) :
blendState(blendType, graphicsDevice),
sampler(graphicsDevice),
texture(texture),
maxParticles(maxParticles),
firstRun(true)
{
	streamOutShader.LoadShader(L"ParticleStreamVert.cso", Vert, graphicsDevice);
	streamOutShader.LoadShader(L"ParticleGenerationGeo.cso", GeometrySO, graphicsDevice);

	drawShader.LoadShader(L"DrawParticleVert.cso", Vert, graphicsDevice);
	drawShader.LoadShader(L"ParticleBillboardGeo.cso", Geometry, graphicsDevice);
	drawShader.LoadShader(L"DrawParticlePix.cso", Pixel, graphicsDevice);

	// Create index and vertex buffers from data
	D3D11_BUFFER_DESC vb;
	ZeroMemory(&vb, sizeof(D3D11_BUFFER_DESC));
	vb.Usage = D3D11_USAGE_DEFAULT;
	vb.ByteWidth = sizeof(Particle) * maxParticles;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	vb.CPUAccessFlags = 0;
	vb.MiscFlags = 0;
	vb.StructureByteStride = 0;
	graphicsDevice->getDevice()->CreateBuffer(&vb, 0, &streamBuffer);
	graphicsDevice->getDevice()->CreateBuffer(&vb, 0, &drawBuffer);

	emitterParticle.initialPosition = XMFLOAT3(0.0, 0.0, 0.0);
	emitterParticle.initialVelocity = XMFLOAT3(0.0, 1.0, 0.0);
	emitterParticle.size = XMFLOAT2(1.0, 1.0);
	emitterParticle.age = 0.0f;
	emitterParticle.type = 1;

	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = &emitterParticle;
	graphicsDevice->getDevice()->CreateBuffer(&vb, &srd, &initBuffer);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.ByteWidth = sizeof(emitterData);
	graphicsDevice->getDevice()->CreateBuffer(&bd, 0, &particleBuffer);

	emitterData.emitPosition = XMFLOAT3(0.0, 0.0, 0.0);
	emitterData.emitDirection = XMFLOAT3(0.0, 1.0, 0.0);
	emitterData.particleAcceleration = XMFLOAT3(0.0, 7.8, 0.0);
}

ParticleEmitter::ParticleEmitter(UINT maxParticles, wchar_t* filepath, BlendType blendType, GraphicsDevice* graphicsDevice) :
blendState(blendType, graphicsDevice),
sampler(graphicsDevice),
texture(filepath, graphicsDevice),
maxParticles(maxParticles),
firstRun(true)
{
	streamOutShader.LoadShader(L"ParticleStreamVert.cso", Vert, graphicsDevice);
	streamOutShader.LoadShader(L"ParticleGenerationGeo.cso", GeometrySO, graphicsDevice);

	drawShader.LoadShader(L"DrawParticleVert.cso", Vert, graphicsDevice);
	drawShader.LoadShader(L"ParticleBillboardGeo.cso", Geometry, graphicsDevice);
	drawShader.LoadShader(L"DrawParticlePix.cso", Pixel, graphicsDevice);

	// Create vertex buffers
	D3D11_BUFFER_DESC vb;
	ZeroMemory(&vb, sizeof(D3D11_BUFFER_DESC));
	vb.Usage = D3D11_USAGE_DEFAULT;
	vb.ByteWidth = sizeof(Particle) * maxParticles;
	vb.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	vb.CPUAccessFlags = 0;
	vb.MiscFlags = 0;
	vb.StructureByteStride = 0;
	graphicsDevice->getDevice()->CreateBuffer(&vb, 0, &streamBuffer);
	graphicsDevice->getDevice()->CreateBuffer(&vb, 0, &drawBuffer);

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
	graphicsDevice->getDevice()->CreateBuffer(&vb, &srd, &initBuffer);

	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.ByteWidth = sizeof(emitterData);
	graphicsDevice->getDevice()->CreateBuffer(&bd, 0, &particleBuffer);
	
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

void ParticleEmitter::Draw(GraphicsDevice* graphicsDevice)
{
	// TODO: Put this somewhere else, VERY slow
	graphicsDevice->getDeviceContext()->VSSetConstantBuffers(4, 1, &particleBuffer);
	graphicsDevice->getDeviceContext()->GSSetConstantBuffers(4, 1, &particleBuffer);
	graphicsDevice->getDeviceContext()->PSSetConstantBuffers(4, 1, &particleBuffer);
	graphicsDevice->getDeviceContext()->UpdateSubresource(particleBuffer, 0, 0, &emitterData, 0, 0);

	// Set up particle information
	blendState.BindBlendState(graphicsDevice);
	texture.BindTexture(graphicsDevice);
	sampler.BindSampler(graphicsDevice);

	UINT stride = sizeof(Particle);
	UINT offset = 0;

	// Use the init buffer if first time drawing/ just reset
	if (firstRun)
		graphicsDevice->getDeviceContext()->IASetVertexBuffers(0, 1, &initBuffer, &stride, &offset);
	else
		graphicsDevice->getDeviceContext()->IASetVertexBuffers(0, 1, &drawBuffer, &stride, &offset);

	// Bind the SO and SO shader
	graphicsDevice->getDeviceContext()->SOSetTargets(1, &streamBuffer, &offset);

	streamOutShader.BindShader(graphicsDevice);
	// Run the stream out shader pass
	if (firstRun)
	{
		graphicsDevice->getDeviceContext()->Draw(1, 0);
		firstRun = false;
	}
	else
		graphicsDevice->getDeviceContext()->DrawAuto();

	// Unbind the Stream buffer
	ID3D11Buffer* bufferArray[1] = { 0 };
	graphicsDevice->getDeviceContext()->SOSetTargets(1, bufferArray, &offset);

	// Swap the buffers
	std::swap(drawBuffer, streamBuffer);

	// Run the draw shader pass
	graphicsDevice->getDeviceContext()->IASetVertexBuffers(0, 1, &drawBuffer, &stride, &offset);

	drawShader.BindShader(graphicsDevice);

	graphicsDevice->getDeviceContext()->DrawAuto();
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