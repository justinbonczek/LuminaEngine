#include "TestScene.hpp"

using namespace Lumina;

void TestScene::LoadAssets(Lumina::Window& window)
{
	camera.Initialize(window);
	SetActiveCamera(camera);

	SpotLight* sLight = new SpotLight();
	sLight->ambient = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
	sLight->diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	sLight->specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 64.0f);
	sLight->attenuation = XMFLOAT3(0.0f, 0.1f, 0.0f);
	sLight->position = XMFLOAT3(0.0f, 0.0f, -10.0f);
	sLight->direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	sLight->spot  = 75.0f;
	sLight->range = 100.0f;

	MeshData cubeData;
	MeshGenerator::CreateCube(3.0f, 3.0f, 3.0f, cubeData);
	Mesh* cubeMesh = new Mesh(cubeData, window.Device());

	MeshData planeData;
	MeshGenerator::CreatePlane(10.0f, 10.0f, 2, 2, planeData);
	Mesh* planeMesh = new Mesh(planeData, window.Device());

	Material* defaultMat = new Material(L"DefaultVert.cso", L"DefaultPix.cso", window);

	Material* cubeMat = new Material(L"DefaultVert.cso", L"TexturedBumpedLitPix.cso", window);
	cubeMat->LoadTexture(L"content/Textures/brick.jpg", window.Device());
	cubeMat->LoadNormal(L"content/Textures/brick_normal.jpg", window.Device());

	LightMaterial * lightMat = new LightMaterial();
	lightMat->ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightMat->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightMat->specular = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
	cubeMat->SetLightMaterial(lightMat);
	defaultMat->SetLightMaterial(lightMat);

	GameObject* cube = new GameObject(cubeMesh, cubeMat);
	cube->SetPosition(0.0f, 0.0f, 0.0f);

	GameObject* plane = new GameObject(planeMesh, defaultMat);

	AddGameObject(cube);
	AddGameObject(plane);
	AddLight(sLight);
}

void TestScene::Update(float dt)
{
	UpdateObjects(dt);

	MoveCamera(dt);
}

void TestScene::Draw(Lumina::Window& window)
{
	camera.UpdateViewMatrix();

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