#include "Game.hpp"

NS_BEGIN

Game Game::instance;

Game::Game():
graphicsManager(GraphicsManager::getInstance())
{ 

}

Game::~Game()
{

}

bool Game::Initialize(void)
{
	if (!graphicsManager.Initialize())
		return false;
	Timer::Initialize();

	scene.Initialize(graphicsManager.getActiveDevice());

	return true;
}

int Game::Run(void)
{
	MSG msg = { 0 };

	scene.LoadAssets();
	scene.InitializePipeline();

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
			graphicsManager.Clear();

			float dt = Timer::GetFrameTime(); 
			scene.Update(dt);
			scene.Draw();
		
			graphicsManager.Display();
		}

		Timer::StopFrame();
	}
	return (int)msg.wParam;
}

Game* Game::GetInstance()
{
	return &instance;
}

LRESULT Game::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (graphicsManager.getActiveDevice()->getDevice())
			graphicsManager.OnSize(lParam);
		return 0;
	case WM_ENTERSIZEMOVE:
		return 0;
	case WM_EXITSIZEMOVE:
		if (graphicsManager.getActiveDevice()->getDevice())
			graphicsManager.OnResizeExit();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

NS_END