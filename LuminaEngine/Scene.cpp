#include "Scene.hpp"

#include <d3dcompiler.h>

#include "Window.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Game.hpp"
#include "Light.hpp"

NS_BEGIN

Scene::Scene()
{}

Scene::~Scene()
{
	for (GameObject* n : objs)
		delete n;
	for (unsigned int i = 0; i < shadowData.numSM; i++)
	{
		delete shadowMaps[i];
	}
	DELETECOM(inputLayout);
	DELETECOM(perFrameBuffer);
	DELETECOM(perObjectBuffer);
	DELETECOM(lightBuffer);
	DELETECOM(shadowBuffer);
}

void Scene::Initialize(GraphicsDevice* _graphicsDevice)
{
	graphicsDevice = _graphicsDevice;
}

void Scene::InitializePipeline()
{
	///
	// Input Layout
	///
	D3D11_INPUT_ELEMENT_DESC vDesc[] =
	{
		{ "POSITION", NULL, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "COLOR",    NULL, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "TEXCOORD", NULL, DXGI_FORMAT_R32G32_FLOAT,       0, 28, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "NORMAL",   NULL, DXGI_FORMAT_R32G32B32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "TANGENT",  NULL, DXGI_FORMAT_R32G32B32_FLOAT,    0, 48, D3D11_INPUT_PER_VERTEX_DATA, NULL }
	};

	D3D11_INPUT_ELEMENT_DESC pVDesc[] = 
	{
		{ "POSITION",  NULL,	DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "TEXCOORD", 0,		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "TEXCOORD", 1,		DXGI_FORMAT_R32G32_FLOAT,	 0, 24, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "TEXCOORD", 2,		DXGI_FORMAT_R32_FLOAT,		 0, 32, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "TEXCOORD", 3,		DXGI_FORMAT_R32_UINT,		 0, 36, D3D11_INPUT_PER_VERTEX_DATA, NULL }
	};

	ID3DBlob* vertexByte;
	D3DReadFileToBlob(L"DefaultVert.cso", &vertexByte);

	ID3DBlob* particleVertexByte;
	D3DReadFileToBlob(L"ParticleStreamVert.cso", &particleVertexByte);

	graphicsDevice->getDevice()->CreateInputLayout(vDesc, ARRAYSIZE(vDesc), vertexByte->GetBufferPointer(), vertexByte->GetBufferSize(), &inputLayout);
	graphicsDevice->getDevice()->CreateInputLayout(pVDesc, ARRAYSIZE(pVDesc), particleVertexByte->GetBufferPointer(), particleVertexByte->GetBufferSize(), &particleInputLayout);

	DELETECOM(vertexByte);
	DELETECOM(particleVertexByte);

	// Configure input assembly
	graphicsDevice->getDeviceContext()->IASetInputLayout(inputLayout);
	graphicsDevice->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_SAMPLER_DESC wsd;
	ZeroMemory(&wsd, sizeof(D3D11_SAMPLER_DESC));

	ID3D11SamplerState* pcfSampler;
	wsd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	wsd.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	wsd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	wsd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	wsd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

	wsd.BorderColor[0] = 1.0;
	wsd.BorderColor[1] = 1.0;
	wsd.BorderColor[2] = 1.0;
	wsd.BorderColor[3] = 1.0;

	graphicsDevice->getDevice()->CreateSamplerState(&wsd, &pcfSampler);

	graphicsDevice->getDeviceContext()->VSSetSamplers(1, 1, &pcfSampler);
	graphicsDevice->getDeviceContext()->PSSetSamplers(1, 1, &pcfSampler);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage				= D3D11_USAGE_DEFAULT;
	bd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags		= 0;
	bd.MiscFlags			= 0;
	bd.StructureByteStride	= 0;

	bd.ByteWidth = sizeof(perFrameData);
	graphicsDevice->getDevice()->CreateBuffer(&bd, NULL, &perFrameBuffer);

	bd.ByteWidth = sizeof(perObjectData);
	graphicsDevice->getDevice()->CreateBuffer(&bd, NULL, &perObjectBuffer);

	bd.ByteWidth = sizeof(lightData);
	graphicsDevice->getDevice()->CreateBuffer(&bd, NULL, &lightBuffer);

	bd.ByteWidth = sizeof(shadowData);
	graphicsDevice->getDevice()->CreateBuffer(&bd, NULL, &shadowBuffer);

	graphicsDevice->getDeviceContext()->VSSetConstantBuffers(0, 1, &perFrameBuffer);
	graphicsDevice->getDeviceContext()->VSSetConstantBuffers(1, 1, &perObjectBuffer);
	graphicsDevice->getDeviceContext()->VSSetConstantBuffers(2, 1, &lightBuffer);
	graphicsDevice->getDeviceContext()->VSSetConstantBuffers(3, 1, &shadowBuffer);

	graphicsDevice->getDeviceContext()->GSSetConstantBuffers(0, 1, &perFrameBuffer);
	graphicsDevice->getDeviceContext()->GSSetConstantBuffers(1, 1, &perObjectBuffer);
	graphicsDevice->getDeviceContext()->GSSetConstantBuffers(2, 1, &lightBuffer);
	graphicsDevice->getDeviceContext()->GSSetConstantBuffers(3, 1, &shadowBuffer);

	graphicsDevice->getDeviceContext()->PSSetConstantBuffers(0, 1, &perFrameBuffer);
	graphicsDevice->getDeviceContext()->PSSetConstantBuffers(1, 1, &perObjectBuffer);
	graphicsDevice->getDeviceContext()->PSSetConstantBuffers(2, 1, &lightBuffer);
	graphicsDevice->getDeviceContext()->PSSetConstantBuffers(3, 1, &shadowBuffer);
}

void Scene::UpdateObjects(float dt)
{
	for (GameObject* n : objs)
		n->Update(dt);
	for (ParticleEmitter* p : particles)
		p->Update(dt);
}

void Scene::DrawScene()
{
	graphicsDevice->getDeviceContext()->IASetInputLayout(inputLayout);
	graphicsDevice->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Shadow pass
	CalculateShadows();

	// Set the render target back
	graphicsDevice->BindBackBuffer();

	UpdateFrameData();

	// Render Geometry
	for (GameObject* n : objs)
	{
		UpdateObjectData(*n);
		n->Draw(graphicsDevice);
	}

	// Render Particles
	graphicsDevice->getDeviceContext()->IASetInputLayout(particleInputLayout);
	graphicsDevice->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	for (ParticleEmitter* p : particles)
	{
		UpdateParticleObjectData(p);
		p->Draw(graphicsDevice);
	}
}

void Scene::UpdateFrameData()
{
	assert(("Must set an active camera using SetActiveCamera()", activeCamera != NULL));

	activeCamera->UpdateViewMatrix();
	perFrameData.view = activeCamera->ViewTranspose();
	perFrameData.projection = activeCamera->ProjTranspose();
	perFrameData.eyePos = activeCamera->GetPosition();
	perFrameData.elapsedTime = Timer::GetFrameTime();

	for (int i = 0; i < shadowData.numSM; i++)
	{
		shadowMaps[i]->SetSRVToShaders(graphicsDevice);
	}

	graphicsDevice->getDeviceContext()->UpdateSubresource(perFrameBuffer, 0, 0, &perFrameData, 0, 0);
	graphicsDevice->getDeviceContext()->UpdateSubresource(lightBuffer, 0, 0, &lightData, 0, 0);
	graphicsDevice->getDeviceContext()->UpdateSubresource(shadowBuffer, 0, 0, &shadowData, 0, 0);
}

void Scene::UpdateObjectData(GameObject& obj)
{
	perObjectData.world = obj.WorldTranspose();
	perObjectData.worldInverseTranspose = obj.WorldInverseTranspose();
	perObjectData.lightMat = obj.LightMaterial();
	perObjectData.tileU = obj.GetTextureTileU();
	perObjectData.tileV = obj.GetTextureTileV();

	graphicsDevice->getDeviceContext()->UpdateSubresource(perObjectBuffer, 0, NULL, &perObjectData, 0, 0);
}

void Scene::UpdateParticleObjectData(ParticleEmitter* e)
{
	perObjectData.world = e->WorldTranspose();
	perObjectData.worldInverseTranspose = e->WorldInverseTranspose();

	graphicsDevice->getDeviceContext()->UpdateSubresource(perObjectBuffer, 0, NULL, &perObjectData, 0, 0);
}

void Scene::AddGameObject(GameObject* obj)
{
	objs.push_back(obj);
}

void Scene::AddSystem(ParticleEmitter* pEmitter)
{
	particles.push_back(pEmitter);
}

void Scene::AddLight(DirectionalLight* light)
{
	assert(("Attempted to add too many directional lights to the scene.", lightData.numDL <= NUMDIRECTIONALLIGHTS));
	lightData.dLight[lightData.numDL] = *light;
	lightData.numDL++;

	if (light->hasShadows)
		AddShadowMap(&lightData.dLight[lightData.numDL - 1]);
}

void Scene::AddLight(PointLight* light)
{
	assert(("Attempted to add too many point lights to the scene.", lightData.numPL <= NUMPOINTLIGHTS));
	lightData.pLight[lightData.numPL] = *light;
	lightData.numPL++;

	if (light->hasShadows)
		AddShadowMap(&lightData.pLight[lightData.numPL - 1]);
}

void Scene::AddLight(SpotLight* light)
{
	assert(("Attempted to add too many spot lights to the scene.", lightData.numSL <= NUMSPOTLIGHTS));
	lightData.sLight[lightData.numSL] = *light;
	lightData.numSL++;

	if (light->hasShadows)
		AddShadowMap(&lightData.sLight[lightData.numSL - 1]);
}

void Scene::AddShadowMap(DirectionalLight* light)
{
	shadowMaps[shadowData.numSM] = new ShadowMap(light, graphicsDevice);
	shadowData.numSM++;
}

void Scene::AddShadowMap(PointLight* light)
{
	// TODO: FUN CUBEMAPPING STUFF HERE
	shadowMaps[shadowData.numSM] = new ShadowMap(light, graphicsDevice);
	shadowData.numSM++;
}

void Scene::AddShadowMap(SpotLight* light)
{
	shadowMaps[shadowData.numSM] = new ShadowMap(light, graphicsDevice);
	shadowData.numSM++;
}

void Scene::SetActiveCamera(Camera& camera)
{
	activeCamera = &camera;
}

void Scene::CalculateShadows()
{
	for (GameObject* n : objs)
	{
		n->SetShadowPass(true);
	}
	
	graphicsDevice->SetShaderResource(3, NULL);

	// Render Shadowmaps
	for (int i = 0; i < shadowData.numSM; i++)
	{
		shadowMaps[i]->UpdateViewProjMatrix();
		SetShadowMap(shadowMaps[i], i);
		
		for (GameObject* obj : objs)
		{
			UpdateObjectData(*obj);
			obj->Draw(graphicsDevice);
		}
	}

	for (GameObject* n : objs)
	{
		n->SetShadowPass(false);
	}
}

void Scene::SetShadowMap(ShadowMap* shadowMap, int index)
{
	perFrameData.view = shadowMap->GetViewMatrixTranspose();
	perFrameData.projection = shadowMap->GetProjectionMatrixTranspose();

	shadowData.sView[index] = shadowMaps[index]->GetViewMatrixTranspose();
	shadowData.sProj[index] = shadowMaps[index]->GetProjectionMatrixTranspose();
	shadowData.resolution = shadowMaps[index]->GetResolution();

	graphicsDevice->getDeviceContext()->UpdateSubresource(perFrameBuffer, 0, 0, &perFrameData, 0, 0);
	graphicsDevice->getDeviceContext()->UpdateSubresource(shadowBuffer, 0, 0, &shadowData, 0, 0);

	shadowMap->BindDSVAndSetNullRenderTarget(graphicsDevice);
}

NS_END