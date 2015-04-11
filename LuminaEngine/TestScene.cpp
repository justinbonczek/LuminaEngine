#include "TestScene.hpp"
#include "Game.hpp"

using namespace Lumina;

void TestScene::LoadAssets()
{
	camera.Initialize(graphicsDevice);
	SetActiveCamera(camera);

	SpotLight* sLight = new SpotLight();
	sLight->ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	sLight->diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	sLight->specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	sLight->attenuation = XMFLOAT3(0.0f, 0.05f, 0.0f);
	sLight->position = XMFLOAT3(0.0f, 10.0f, 10.0f);
	sLight->direction = XMFLOAT3(-sLight->position.x, -sLight->position.y, -sLight->position.z);
	sLight->spot  = 30.0f;
	sLight->range = 100.0f;
	sLight->hasShadows = true;

	DirectionalLight* dLight = new DirectionalLight();
	dLight->ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	dLight->diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	dLight->specular = XMFLOAT4(0.9, 0.9f, 0.9f, 1.0f);
	dLight->direction = XMFLOAT3(1.0f, -1.0f, 0.0f);
	dLight->hasShadows = true;

	MeshData cubeData;
	MeshGenerator::CreateCube(3.0f, 3.0f, 3.0f, cubeData);
	Mesh* cubeMesh = new Mesh(cubeData, graphicsDevice);

	MeshData planeData;
	MeshGenerator::CreatePlane(500.0f, 500.0f, 2, 2, planeData);
	Mesh* planeMesh = new Mesh(planeData, graphicsDevice);

	Material* cubeMat = new Material(L"ShadowedVert.cso", L"ShadowedTexturedBumpedLitPix.cso",graphicsDevice);
	cubeMat->LoadTexture(L"content/Textures/brick.jpg", graphicsDevice);
	cubeMat->LoadNormal(L"content/Textures/brick_normal.jpg", graphicsDevice);
	
	Material* planeMat = new Material(L"ShadowedVert.cso", L"ShadowedTexturedLitPix.cso", graphicsDevice);
	planeMat->LoadTexture(L"content/Textures/tile.jpg", graphicsDevice);
	planeMat->SetTextureTileUV(50);

	LightMaterial* lightMat = new LightMaterial();
	lightMat->ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightMat->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightMat->specular = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
	cubeMat->SetLightMaterial(lightMat);
	planeMat->SetLightMaterial(lightMat);

	ParticleEmitter* fireEmitter = new ParticleEmitter(1000, L"content/Textures/particle.png", Additive, graphicsDevice);
	fireEmitter->SetPosition(0.0f, 0.0f, 0.0f);

	GameObject* cube = new GameObject(cubeMesh, cubeMat);
	cube->SetPosition(0.0f, 0.0f, 0.0f);

	GameObject* cube2 = new GameObject(cubeMesh, cubeMat);
	cube2->SetPosition(25.0f, 0.0f, 0.0f);

	GameObject* cube3 = new GameObject(cubeMesh, cubeMat);
	cube3->SetPosition(0.0f, 5.0f, 10.0f);

	GameObject* cube4 = new GameObject(cubeMesh, cubeMat);
	cube4->SetPosition(-25.0f, 100.0f, 0.0f);

	GameObject* plane = new GameObject(planeMesh, planeMat);
	plane->SetPosition(0.0f, -3.0f, 0.0f);

	AddGameObject(cube);
	AddGameObject(cube2);
	AddGameObject(cube3);
	AddGameObject(cube4);
	AddGameObject(plane);
	//AddSystem(fireEmitter);
	//AddLight(sLight);
	AddLight(dLight);
}

void TestScene::Update(float dt)
{
	MoveCamera(dt);
	UpdateObjects(dt);

	lightData.sLight[0].position = XMFLOAT3(cos(Timer::GetTotalTime()) * 10.0f, 10.0f, sin(Timer::GetTotalTime()) * 10.0f);
	lightData.sLight[0].direction = XMFLOAT3(-lightData.sLight[0].position.x, -lightData.sLight[0].position.y, -lightData.sLight[0].position.z);
}

void TestScene::Draw()
{
	DrawScene();
}

void TestScene::MoveCamera(float dt)
{
	if (GetAsyncKeyState('W') & 0x8000)
		camera.Walk(10.0f*dt);
	if (GetAsyncKeyState('S') & 0x8000)
		camera.Walk(-10.0f*dt);
	if (GetAsyncKeyState('A') & 0x8000)
		camera.Strafe(-10.0f*dt);
	if (GetAsyncKeyState('D') & 0x8000)
		camera.Strafe(10.0f*dt);
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		camera.Pitch(-1.0f * dt);
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		camera.RotateY(-1.0f * dt);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		camera.RotateY(1.0f * dt);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		camera.Pitch(1.0f * dt);
}