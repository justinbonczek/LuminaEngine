#include "Water.h"
#include "Game.h"
#include <time.h>

namespace Lumina
{

	Water::Water(Mesh* mesh, Material* mat) :
		GameObject(mesh, mat)
	{

	}

	Water::~Water()
	{
		ReleaseMacro(waterBuffer);
	}

	void Water::Update(float dt)
	{
		if (GetAsyncKeyState('1') && 0x8000)
			waterBufferData.amp1 += 0.01;
		if (GetAsyncKeyState('2') && 0x8000)
			waterBufferData.amp2 += 0.01;
		if (GetAsyncKeyState('3') && 0x8000)
			waterBufferData.freq1 += 0.01;
		if (GetAsyncKeyState('4') && 0x8000)
			waterBufferData.freq2 += 0.01;
		if (GetAsyncKeyState('5') && 0x8000)
			waterBufferData.speed1 += 0.01;
		if (GetAsyncKeyState('6') && 0x8000)
			waterBufferData.speed2 += 0.01;
		if (GetAsyncKeyState('7') && 0x8000)
			waterBufferData.k += 0.01;

		GameObject::Update(dt);
	}

	void Water::Draw(ID3D11DeviceContext* devCon)
	{
		devCon->UpdateSubresource(waterBuffer, 1, 0, &waterBufferData, 0, 0);
		devCon->VSSetConstantBuffers(1, 1, &waterBuffer);
		GameObject::Draw(devCon);
	}

	void Water::SetupBuffer(ID3D11Device* dev)
	{
		srand(time(0));

		waterBufferData.amp1 = ((float)rand() / (float)RAND_MAX) * 1.0;
		waterBufferData.amp2 = ((float)rand() / (float)RAND_MAX) * 1.0;
		waterBufferData.freq1 = ((float)rand() / (float)RAND_MAX) * 1.0;
		waterBufferData.freq2 = ((float)rand() / (float)RAND_MAX) * 1.0;
		waterBufferData.speed1 = ((float)rand() / (float)RAND_MAX) * 2.0;
		waterBufferData.speed2 = ((float)rand() / (float)RAND_MAX) * 2.0;
		waterBufferData.k = (float)rand() / (float)RAND_MAX + 1;

		//waterBufferData.amp1 = 0.5;
		//waterBufferData.amp2 = 0.5;
		//waterBufferData.freq1 = 1.0;
		//waterBufferData.freq2 = 1.0;
		//waterBufferData.k = 2.0;
		//waterBufferData.speed1 = 1.3;
		//waterBufferData.speed2 = 3.0;

		D3D11_BUFFER_DESC cb;
		ZeroMemory(&cb, sizeof(D3D11_BUFFER_DESC));
		cb.ByteWidth = sizeof(WaterBufferData);
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.Usage = D3D11_USAGE_DEFAULT;
		cb.CPUAccessFlags = 0;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &waterBufferData;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;
		dev->CreateBuffer(&cb, &initData, &waterBuffer);
	}
}