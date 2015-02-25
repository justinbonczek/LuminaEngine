#include "Game.hpp"

NS_BEGIN

Game Game::instance;

Game::Game()
{
	window = new Window(GetModuleHandle(NULL), 1280, 720, L"I'm a window!");
	Timer::Initialize();
}

Game::~Game()
{
	delete window;
	window = 0;
}

int Game::Run(void)
{
	Timer::Initialize();
	MSG msg = { 0 };

	scene.LoadAssets(*window);
	scene.InitializePipeline(*window);

	while (msg.message != WM_QUIT)
	{
		Timer::StartFrame();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			window->Clear();

			scene.Update(Timer::GetFrameTime());
			scene.Draw(*window);
		
			window->Display();
		}

		Timer::StopFrame();
	}
	return (int)msg.wParam;
}

bool Game::Initialize(void)
{
	if (!window->Initialize())
		return false;
	
	return true;
}

void Game::SetRenderTarget()
{
	window->SetRenderTarget();
}

Game* Game::GetInstance()
{
	return &instance;
}

ID3D11Device* Game::GetDevice()
{
	return window->Device();
}

ID3D11DeviceContext* Game::GetDeviceContext()
{
	return window->DeviceContext();
}

ID3D11RenderTargetView* Game::GetBackBufferView()
{
	return window->BackBuffer();
}

ID3D11DepthStencilView* Game::GetDepthStencilView()
{
	return window->DepthStencilView();
}

NS_END