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

	DELETECOM(inputLayout);
	DELETECOM(perFrameBuffer);
	DELETECOM(perObjectBuffer);
	DELETECOM(lightBuffer);
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

	window.DeviceContext()->VSSetConstantBuffers(0, 1, &perFrameBuffer);
	window.DeviceContext()->VSSetConstantBuffers(1, 1, &perObjectBuffer);
	window.DeviceContext()->VSSetConstantBuffers(2, 1, &lightBuffer);

	window.DeviceContext()->PSSetConstantBuffers(0, 1, &perFrameBuffer);
	window.DeviceContext()->PSSetConstantBuffers(1, 1, &perObjectBuffer);
	window.DeviceContext()->PSSetConstantBuffers(2, 1, &lightBuffer);
}

void Scene::UpdateObjects(float dt)
{
	for (GameObject* n : objs)
		n->Update(dt);
}

void Scene::DrawScene()
{
	//CalculateShadows();

	//SetRenderTarget();
	UpdateFrameData();

	for (GameObject* n : objs)
	{
		UpdateObjectData(*n);
		n->Draw(game->GetDeviceContext());
	}
}

void Scene::SetRenderTarget()
{
	ID3D11RenderTargetView* rtv = game->GetBackBufferView();
	game->GetDeviceContext()->OMSetRenderTargets(1, &rtv, game->GetDepthStencilView());
}

void Scene::UpdateFrameData()
{
	assert(("Must set an active camera using SetActiveCamera()", activeCamera != NULL));
	perFrameData.view = activeCamera->ViewTranspose();
	perFrameData.projection = activeCamera->ProjTranspose();
	perFrameData.eyePos = activeCamera->GetPosition();
	perFrameData.elapsedTime = Timer::GetFrameTime();

	game->GetDeviceContext()->UpdateSubresource(perFrameBuffer, 0, NULL, &perFrameData, 0, 0);
	game->GetDeviceContext()->UpdateSubresource(lightBuffer, 0, 0, &lightData, 0, 0);
}

void Scene::UpdateObjectData(GameObject& obj)
{
	perObjectData.world = obj.WorldTranspose();
	perObjectData.worldInverseTranspose = obj.WorldInverseTranspose();
	perObjectData.lightMat = obj.LightMaterial();

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
}

void Scene::AddLight(PointLight* light)
{
	assert(("Attempted to add too many point lights to the scene.", lightData.numPL <= NUMPOINTLIGHTS));
	lightData.pLight[lightData.numPL] = *light;
	lightData.numPL++;
}

void Scene::AddLight(SpotLight* light)
{
	assert(("Attempted to add too many spot lights to the scene.", lightData.numSL <= NUMSPOTLIGHTS));
	lightData.sLight[lightData.numSL] = *light;
	lightData.numSL++;
}

void Scene::SetActiveCamera(Camera& camera)
{
	activeCamera = &camera;
}

void Scene::CalculateShadows()
{
	//for (GameObject* n : objs)
	//{
	//	n->SetShadowPass(true);
	//}
	//
	//for (int i = 0; i < lightData.numDL; i++)
	//{
	//	if (lightData.dLight[i].hasShadows)
	//		CreateDirectionalShadowMap(&lightData.dLight[i]);
	//}
	//for (int i = 0; i < lightData.numSL; i++)
	//{
	//	if (lightData.sLight[i].hasShadows)
	//		CreateSpotShadowMap(&lightData.sLight[i]);
	//}
	//for (int i = 0; i < lightData.numPL; i++)
	//{
	//	if (lightData.pLight[i].hasShadows)
	//		CreatePointShadowMap(&lightData.pLight[i]);
	//}
	//
	//for (GameObject* n : objs)
	//{
	//	n->SetShadowPass(false);
	//}
}

void Scene::CreateDirectionalShadowMap(DirectionalLight* dLight)
{
	
}

void Scene::CreateSpotShadowMap(SpotLight* sLight)
{
	XMVECTOR pos = XMLoadFloat3(&sLight->position);
	XMVECTOR dir = XMLoadFloat3(&sLight->direction);

	XMVECTOR target = XMVector3Normalize(pos + dir);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMMATRIX proj = XMMatrixOrthographicLH(ORTHOGRAPHICVIEWWIDTH, ORTHOGRAPHICVIEWHEIGHT, 0.01f, 100.0f);

	XMStoreFloat4x4(&perFrameData.view, view);
	XMStoreFloat4x4(&perFrameData.projection, proj);
}

void Scene::CreatePointShadowMap(PointLight* pLight)
{

}


NS_END