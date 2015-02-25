#include "TestScene.hpp"
#include "Game.hpp"

using namespace Lumina;

void TestScene::LoadAssets(Lumina::Window& window)
{
	camera.Initialize(window);
	SetActiveCamera(camera);

	SpotLight* sLight = new SpotLight();
	sLight->ambient = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
	sLight->diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	sLight->specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	sLight->attenuation = XMFLOAT3(0.0f, 0.05f, 0.0f);
	sLight->position = XMFLOAT3(0.0f, 10.0f, 10.0f);
	sLight->direction = XMFLOAT3(-sLight->position.x, -sLight->position.y, -sLight->position.z);
	sLight->spot  = 30.0f;
	sLight->range = 100.0f;
	sLight->hasShadows = true;

	DirectionalLight* dLight = new DirectionalLight();
	dLight->ambient = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
	dLight->diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dLight->specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dLight->direction = XMFLOAT3(0.0f, -1.0f, -0.5f);
	dLight->hasShadows = true;

	MeshData cubeData;
	MeshGenerator::CreateSphere(3.0f, 4, cubeData);
	Mesh* cubeMesh = new Mesh(cubeData, game->GetDevice());

	MeshData planeData;
	MeshGenerator::CreatePlane(20.0f, 20.0f, 2, 2, planeData);
	Mesh* planeMesh = new Mesh(planeData, game->GetDevice());

	Material* cubeMat = new Material(L"ShadowedVert.cso", L"ShadowTexturedBumpedLitPix.cso", window);
	cubeMat->LoadTexture(L"content/Textures/brick.jpg", game->GetDevice());
	cubeMat->LoadNormal(L"content/Textures/brick_normal.jpg", game->GetDevice());
	
	Material* planeMat = new Material(L"ShadowedVert.cso", L"ShadowedTexturedBumpedLitPix.cso", window);

	LightMaterial * lightMat = new LightMaterial();
	lightMat->ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightMat->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightMat->specular = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
	cubeMat->SetLightMaterial(lightMat);
	planeMat->SetLightMaterial(lightMat);

	GameObject* cube = new GameObject(cubeMesh, cubeMat);
	cube->SetPosition(0.0f, 0.0f, 0.0f);

	GameObject* plane = new GameObject(planeMesh, planeMat);
	plane->SetPosition(0.0f, -3.0f, 0.0f);

	AddGameObject(cube);
	AddGameObject(plane);
	//AddLight(sLight);
	AddLight(dLight);
}

void TestScene::Update(float dt)
{
	UpdateObjects(dt);

	float tTime = Timer::GetTotalTime();
	lightData.dLight[0].direction = XMFLOAT3(0.0, sin(tTime), cos(tTime));

	MoveCamera(dt);
}

void TestScene::Draw(Lumina::Window& window)
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