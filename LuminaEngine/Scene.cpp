#include "Scene.hpp"

#include <d3dcompiler.h>

#include "Window.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Game.hpp"
#include "Light.hpp"

NS_BEGIN

Scene::Scene()
{
	game = Game::GetInstance();
}

Scene::~Scene()
{
	for (GameObject* n : objs)
		delete n;
	for (int i = 0; i < shadowData.numSM; i++)
	{
		delete shadowMaps[i];
	}
	DELETECOM(inputLayout);
	DELETECOM(perFrameBuffer);
	DELETECOM(perObjectBuffer);
	DELETECOM(lightBuffer);
	DELETECOM(shadowBuffer);
}

void Scene::InitializePipeline(Window& window)
{
	///
	// Input Layout
	///
	D3D11_INPUT_ELEMENT_DESC vDesc[] =
	{
		{ "POSITION", NULL, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "COLOR", NULL, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "TEXCOORD", NULL, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "NORMAL", NULL, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, NULL },
		{ "TANGENT", NULL, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, NULL }
	};

	ID3DBlob* vertexByte;
	D3DReadFileToBlob(L"DefaultVert.cso", &vertexByte);

	window.Device()->CreateInputLayout(vDesc, ARRAYSIZE(vDesc), vertexByte->GetBufferPointer(), vertexByte->GetBufferSize(), &inputLayout);

	DELETECOM(vertexByte);

	// Configure input assembly
	window.DeviceContext()->IASetInputLayout(inputLayout);
	window.DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// TODO: SAMPLER CLASS TO TAKE CARE OF THIS
	HRESULT hr;

	D3D11_SAMPLER_DESC wsd;
	ZeroMemory(&wsd, sizeof(D3D11_SAMPLER_DESC));

	ID3D11SamplerState* pcfSampler;
	wsd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	wsd.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	wsd.MinLOD = 0;
	wsd.MaxLOD = 0;
	wsd.MipLODBias = 0;
	wsd.MaxAnisotropy = 0;
	wsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = game->GetDevice()->CreateSamplerState(&wsd, &pcfSampler);

	game->GetDeviceContext()->VSSetSamplers(1, 1, &pcfSampler);
	game->GetDeviceContext()->PSSetSamplers(1, 1, &pcfSampler);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage				= D3D11_USAGE_DEFAULT;
	bd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags		= 0;
	bd.MiscFlags			= 0;
	bd.StructureByteStride	= 0;

	bd.ByteWidth = sizeof(perFrameData);
	window.Device()->CreateBuffer(&bd, NULL, &perFrameBuffer);

	bd.ByteWidth = sizeof(perObjectData);
	window.Device()->CreateBuffer(&bd, NULL, &perObjectBuffer);

	bd.ByteWidth = sizeof(lightData);
	window.Device()->CreateBuffer(&bd, NULL, &lightBuffer);

	bd.ByteWidth = sizeof(shadowData);
	window.Device()->CreateBuffer(&bd, NULL, &shadowBuffer);

	window.DeviceContext()->VSSetConstantBuffers(0, 1, &perFrameBuffer);
	window.DeviceContext()->VSSetConstantBuffers(1, 1, &perObjectBuffer);
	window.DeviceContext()->VSSetConstantBuffers(2, 1, &lightBuffer);
	window.DeviceContext()->VSSetConstantBuffers(3, 1, &shadowBuffer);

	window.DeviceContext()->PSSetConstantBuffers(0, 1, &perFrameBuffer);
	window.DeviceContext()->PSSetConstantBuffers(1, 1, &perObjectBuffer);
	window.DeviceContext()->PSSetConstantBuffers(2, 1, &lightBuffer);
	window.DeviceContext()->PSSetConstantBuffers(3, 1, &shadowBuffer);
}

void Scene::UpdateObjects(float dt)
{
	for (GameObject* n : objs)
		n->Update(dt);
}

void Scene::DrawScene()
{
	CalculateShadows();

	game->SetRenderTarget();

	UpdateFrameData();

	for (GameObject* n : objs)
	{
		UpdateObjectData(*n);
		n->Draw(game->GetDeviceContext());
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
		shadowMaps[i]->SetSRVToShaders(game->GetDeviceContext());
	}

	game->GetDeviceContext()->UpdateSubresource(perFrameBuffer, 0, 0, &perFrameData, 0, 0);
	game->GetDeviceContext()->UpdateSubresource(lightBuffer, 0, 0, &lightData, 0, 0);
	game->GetDeviceContext()->UpdateSubresource(shadowBuffer, 0, 0, &shadowData, 0, 0);
}

void Scene::UpdateObjectData(GameObject& obj)
{
	perObjectData.world = obj.WorldTranspose();
	perObjectData.worldInverseTranspose = obj.WorldInverseTranspose();
	perObjectData.lightMat = obj.LightMaterial();
	perObjectData.tileU = obj.GetTextureTileU();
	perObjectData.tileV = obj.GetTextureTileV();

	game->GetDeviceContext()->UpdateSubresource(perObjectBuffer, 0, NULL, &perObjectData, 0, 0);
}

void Scene::AddGameObject(GameObject* obj)
{
	objs.push_back(obj);
}

void Scene::AddLight(DirectionalLight* light)
{
	assert(("Attempted to add too many directional lights to the scene.", lightData.numDL <= NUMDIRECTIONALLIGHTS));
	lightData.dLight[lightData.numDL] = *light;
	lightData.numDL++;

	if (light->hasShadows)
		AddShadowMap(light);
}

void Scene::AddLight(PointLight* light)
{
	assert(("Attempted to add too many point lights to the scene.", lightData.numPL <= NUMPOINTLIGHTS));
	lightData.pLight[lightData.numPL] = *light;
	lightData.numPL++;

	if (light->hasShadows)
		AddShadowMap(light);
}

void Scene::AddLight(SpotLight* light)
{
	assert(("Attempted to add too many spot lights to the scene.", lightData.numSL <= NUMSPOTLIGHTS));
	lightData.sLight[lightData.numSL] = *light;
	lightData.numSL++;

	if (light->hasShadows)
		AddShadowMap(light);
}

void Scene::AddShadowMap(DirectionalLight* light)
{
	shadowMaps[shadowData.numSM] = new ShadowMap(light, game->GetDevice());
	shadowData.numSM++;
}

void Scene::AddShadowMap(PointLight* light)
{
	// TODO: FUN CUBEMAPPING STUFF HERE
	shadowMaps[shadowData.numSM] = new ShadowMap(light, game->GetDevice());
	shadowData.numSM++;
}

void Scene::AddShadowMap(SpotLight* light)
{
	shadowMaps[shadowData.numSM] = new ShadowMap(light, game->GetDevice());
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
	
	// Render Shadowmaps
	for (int i = 0; i < shadowData.numSM; i++)
	{
		SetShadowMap(shadowMaps[i], i);
		
		for (GameObject* obj : objs)
		{
			UpdateObjectData(*obj);
			obj->Draw(game->GetDeviceContext());
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

	game->GetDeviceContext()->UpdateSubresource(perFrameBuffer, 0, 0, &perFrameData, 0, 0);
	game->GetDeviceContext()->UpdateSubresource(shadowBuffer, 0, 0, &shadowData, 0, 0);

	shadowMap->BindDSVAndSetNullRenderTarget(game->GetDeviceContext());
}

NS_END