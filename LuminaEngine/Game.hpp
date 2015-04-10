#ifndef GAME_H
#define GAME_H

#include "Config.hpp"
#include "GraphicsManager.hpp"
#include "Lumina.hpp"
#include "TestScene.hpp"

NS_BEGIN

class Game
{
public:
	~Game();
	static Game* GetInstance();

	/// <summary>
	/// Handles Windows messages
	/// </summary>
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// Where the magic happens
	/// </summary>
	int Run(void);

	/// <summary>
	/// Opens the game window, creates the Direct3D context, loads assets, and starts the graphics pipeline
	/// </summary>
	bool Initialize(void);

private:
	Game();
	static Game instance;

	GraphicsManager& graphicsManager;
	TestScene scene;
};

NS_END

#endif