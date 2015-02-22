///
// Environment Simulation made by Justin Bonczek using DirectX 11
///

#include <d3dcompiler.h>

#include "Simulation.h"
#include "Vertex.h"
#include "Timer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	Simulation simulation(hInstance);

	if (!simulation.Initialize())
		return 0;

	return simulation.Run();
}

Simulation::Simulation(HINSTANCE hInstance) :
Game(hInstance)
{
	windowTitle = L"Lumina Engine";
	windowWidth = 1600;
	windowHeight = 900;
}

Simulation::~Simulation()
{
	for (GameObject* obj : objects)
	{
		delete obj;
		obj = 0;
	}
	for (Light* n : lights)
	{
		delete n;
		n = 0;
	}
	ReleaseMacro(perFrameBuffer);
	ReleaseMacro(perObjectBuffer);
	ReleaseMacro(lightBuffer);
	ReleaseMacro(depthStencilState);
	ReleaseMacro(noDoubleBlendDSS);
	ReleaseMacro(inputLayout);
	ReleaseMacro(perFrameBuffer);
	ReleaseMacro(perObjectBuffer);
	ReleaseMacro(blendState);
	ReleaseMacro(solid);
	ReleaseMacro(wireframe);
}

bool Simulation::Initialize()
{
	if (!Game::Initialize())
		return false;

	LoadAssets();
	InitializePipeline();

	m_Camera.SetPosition(0.0f, 0.0f, -10.0f);
	return true;
}

void Simulation::LoadAssets()
{
	ID3D11SamplerState* wrapSampler;
	D3D11_SAMPLER_DESC wsd;
	ZeroMemory(&wsd, sizeof(D3D11_SAMPLER_DESC));
	wsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	wsd.MaxAnisotropy = 0;
	wsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	wsd.MinLOD = 0;
	wsd.MaxLOD = 0;
	wsd.MipLODBias = 0;
	dev->CreateSamplerState(&wsd, &wrapSampler);

	ID3D11SamplerState* pcfSampler;
	wsd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	wsd.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	wsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	wsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	dev->CreateSamplerState(&wsd, &pcfSampler);
	devCon->PSSetSamplers(1, 1, &pcfSampler);

	//pLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.f, 1.0f);
	//pLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	//pLight.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//pLight.attenuation = XMFLOAT3(0.0f, 0.1f, 0.0f);
	//pLight.position = XMFLOAT3(10.0f, 10.0f, 0.0f);
	//pLight.range = 50.0f;
	//
	//perFrameData.pLight = pLight;

	SpotLight* sLight1 = new SpotLight();
	SpotLight* sLight2 = new SpotLight();
	SpotLight* sLight3 = new SpotLight();

	sLight1->ambient = XMFLOAT4(0.0f, 0.0f, 0.f, 1.0f);
	sLight1->diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	sLight1->specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	sLight1->attenuation = XMFLOAT3(1.0f, 0.0f, 0.0f);
	sLight1->position = XMFLOAT3(0.0f, 10.0f, 0.0f);
	sLight1->direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	sLight1->spot = 45.0f;
	sLight1->range = 1000.0f;

	sLight2->ambient = XMFLOAT4(0.0f, 0.0f, 0.f, 1.0f);
	sLight2->diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	sLight2->specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	sLight2->attenuation = XMFLOAT3(1.0f, 0.0f, 0.0f);
	sLight2->position = XMFLOAT3(10.0f, -5.0f, 0.0f);
	sLight2->direction = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	sLight2->spot = 45.0f;
	sLight2->range = 1000.0f;

	sLight3->ambient = XMFLOAT4(0.0f, 0.0f, 0.f, 1.0f);
	sLight3->diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	sLight3->specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	sLight3->attenuation = XMFLOAT3(1.0f, 0.0f, 0.0f);
	sLight3->position = XMFLOAT3(0.0f, -5.0f, -10.0f);
	sLight3->direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	sLight3->spot = 45.0f;
	sLight3->range = 1000.0f;

	AddLight(sLight1);
	AddLight(sLight2);
	AddLight(sLight3);

	MeshData doublePlane;
	MeshGenerator::CreateCube(3.0f, 3.0f, 3.0f, doublePlane);
	Mesh* doublePlaneMesh = new Mesh(doublePlane, dev);

	LightMaterial* lightMat = new LightMaterial();
	lightMat->ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	lightMat->diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	lightMat->specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);

	//Material* defaultMat = new Material(L"NoLightVert.cso", L"NoLightPixel.cso", wrapSampler, dev);

	Material* defaultMat = new Material(L"Textures/brick.jpg", wrapSampler, dev);
	defaultMat->LoadShader(L"DefaultVertex.cso", Vert, dev);
	defaultMat->LoadShader(L"TextureBumpedLitPixel.cso", Pixel, dev);
	defaultMat->LoadNormal(L"Textures/brick_normal.jpg", dev);
	defaultMat->SetTileX(1.0f);
	defaultMat->SetTileZ(1.0f);

	defaultMat->SetLightMaterial(lightMat);
	GameObject* cube = new GameObject(doublePlaneMesh, defaultMat);
	cube->SetPosition(XMFLOAT3(0.0, -5.0, 0.0));
	
	objects.push_back(cube);
}

void Simulation::InitializePipeline()
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
	D3DReadFileToBlob(L"DefaultVertex.cso", &vertexByte);

	dev->CreateInputLayout(vDesc, ARRAYSIZE(vDesc), vertexByte->GetBufferPointer(), vertexByte->GetBufferSize(), &inputLayout);

	ReleaseMacro(vertexByte);

	///
	// Pipeline buffers/ states
	// cBuffers, blend state, rasterizer state, stencil states, etc
	///
	D3D11_BUFFER_DESC cd;
	ZeroMemory(&cd, sizeof(D3D11_BUFFER_DESC));
	cd.ByteWidth =  sizeof(perFrameData);
	cd.Usage = D3D11_USAGE_DEFAULT;
	cd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cd.CPUAccessFlags = 0;
	cd.MiscFlags = 0;
	cd.StructureByteStride = 0;
	dev->CreateBuffer(&cd, NULL, &perFrameBuffer);

	cd.ByteWidth = sizeof(perObjectData);
	dev->CreateBuffer(&cd, NULL, &perObjectBuffer);

	cd.ByteWidth = sizeof(lightData);
	dev->CreateBuffer(&cd, NULL, &lightBuffer);

	//
	// Blend State
	//
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;

	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	dev->CreateBlendState(&bd, &blendState);

	//
	// Rasterizer States
	//
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = false;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = false;
	rd.AntialiasedLineEnable = false;
	dev->CreateRasterizerState(&rd, &wireframe);

	rd.FillMode = D3D11_FILL_SOLID;
	dev->CreateRasterizerState(&rd, &solid);

	current = solid;

	//
	// Depth Stencil States
	//
	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsd.StencilEnable = false;
	dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dev->CreateDepthStencilState(&dsd, &depthStencilState);

	D3D11_DEPTH_STENCIL_DESC ndsd;
	ZeroMemory(&ndsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	ndsd.DepthEnable = true;
	ndsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ndsd.DepthFunc = D3D11_COMPARISON_LESS;
	ndsd.StencilEnable = true;
	ndsd.StencilReadMask = 0xff;
	ndsd.StencilWriteMask = 0xff;

	ndsd.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	ndsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	ndsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	ndsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	ndsd.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	ndsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	ndsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	ndsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dev->CreateDepthStencilState(&ndsd, &noDoubleBlendDSS);

	// Configure input assembly
	devCon->IASetInputLayout(inputLayout);
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set up constant buffers
	devCon->VSSetConstantBuffers(0, 1, &perFrameBuffer);
	devCon->VSSetConstantBuffers(1, 1, &perObjectBuffer);
	devCon->VSSetConstantBuffers(2, 1, &lightBuffer);
	
	devCon->PSSetConstantBuffers(0, 1, &perFrameBuffer);
	devCon->PSSetConstantBuffers(1, 1, &perObjectBuffer);
	devCon->PSSetConstantBuffers(2, 1, &lightBuffer);
}

void Simulation::OnResize()
{
	Game::OnResize();

	m_Camera.SetLens(0.25f * 3.1415926535f, AspectRatio(), 0.1f, 200.0f);
	XMStoreFloat4x4(&(perFrameData.projection), XMMatrixTranspose(m_Camera.Proj()));
}

void Simulation::Update(float dt)
{
	///
	// Rudimentary implementation to handle rasterizer state change (space to switch to wireframe/ back)
	///
	time += dt;
	totalTime += dt;

	perFrameData.time = totalTime;
	if (GetAsyncKeyState(VK_SPACE) && 0x8000 && time > 0.25f)
	{
		if (current == solid)
		{
			current = wireframe;
			time = 0.0f;
		}
		else
		{
			current = solid;
			time = 0.0f;
		}
		if (time > 100.0f)
			time -= 100.0f;
	}
	MoveCamera(dt);
	perFrameData.eyePos = m_Camera.GetPosition();
	
	for (GameObject* obj : objects)
	{
		obj->Update(dt);
	}
}

void Simulation::Draw()
{
	// Update camera
	m_Camera.UpdateViewMatrix();

	// Set up the render target (back buffer) and clear it
	const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	devCon->ClearRenderTargetView(renderTargetView, clearColor);
	devCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set various states
	float blendFactors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	devCon->OMSetBlendState(blendState, blendFactors, 0xFFFFFF);
	devCon->RSSetState(current);
	devCon->OMSetDepthStencilState(depthStencilState, 0);

	XMStoreFloat4x4(&(perFrameData.view), XMMatrixTranspose(m_Camera.View()));
	XMStoreFloat4x4(&(perFrameData.projection), XMMatrixTranspose(m_Camera.Proj()));

	devCon->UpdateSubresource(perFrameBuffer, 0, NULL, &perFrameData, 0, 0);
	devCon->UpdateSubresource(lightBuffer, 0, NULL, &lightData, 0, 0);
	// Render the geometry from the camera to the back buffer
	for (GameObject* obj : objects)
	{
		XMStoreFloat4x4(&(perObjectData.world), XMMatrixTranspose(XMLoadFloat4x4(&(obj->GetWorldMatrix()))));
		XMStoreFloat4x4(&(perObjectData.worldInverseTranspose), XMMatrixTranspose(XMMatrixInverse(nullptr, XMMatrixTranspose(XMLoadFloat4x4(&(obj->GetWorldMatrix()))))));
		perObjectData.lightMat = obj->GetLightMaterial();

		perObjectData.tileX = obj->GetTextureTileX();
		perObjectData.tileZ = obj->GetTextureTileZ();

		devCon->UpdateSubresource(perObjectBuffer, 0, NULL, &perObjectData, 0, 0);
		obj->Draw(devCon);
	}
	// Swap the buffer pointers!
	swapChain->Present(0, 0);
}

void Simulation::MoveCamera(float dt)
{
	if (GetAsyncKeyState('W') & 0x8000)
		m_Camera.Walk(10.0f*dt);
	if (GetAsyncKeyState('S') & 0x8000)
		m_Camera.Walk(-10.0f*dt);
	if (GetAsyncKeyState('A') & 0x8000)
		m_Camera.Strafe(-10.0f*dt);
	if (GetAsyncKeyState('D') & 0x8000)
		m_Camera.Strafe(10.0f*dt);
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_Camera.Pitch(-1.0f * dt);
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_Camera.RotateY(-1.0f * dt);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_Camera.RotateY(1.0f * dt);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_Camera.Pitch(1.0f * dt);
}

void Simulation::AddLight(DirectionalLight* dLight)
{
	if (lightData.numDL == NUMDIRECTIONALLIGHTS)
	{
		printf("ERROR: Simulation already has the maximum number of Directional Lights.\n");
	}
		lightData.dLight[lightData.numDL] = *dLight;
		lightData.numDL++;
}
void Simulation::AddLight(PointLight* pLight)
{
	if (lightData.numPL == NUMPOINTLIGHTS)
	{
		printf("ERROR: Simulation already has the maximum number of Directional Lights.\n");
	}
	lightData.pLight[lightData.numPL] = *pLight;
	lightData.numPL++;
}
void Simulation::AddLight(SpotLight* sLight)
{
	if (lightData.numSL == NUMSPOTLIGHTS)
	{
		printf("ERROR: Simulation already has the maximum number of Directional Lights.\n");
	}
	lightData.sLight[lightData.numSL] = *sLight;
	lightData.numSL++;
}