#ifndef GAME_H
#define GAME_H

#include "Config.hpp"
#include "Lumina.hpp"
#include "TestScene.hpp"

NS_BEGIN

class Game
{
public:
	
	~Game();

	/// <summary>
	/// Where the magic happens
	/// </summary>
	int Run(void);

	/// <summary>
	/// Opens the game window, creates the Direct3D context, loads assets, and starts the graphics pipeline
	/// </summary>
	bool Initialize(void);

	static Game* GetInstance();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11RenderTargetView* GetBackBufferView();
	ID3D11DepthStencilView* GetDepthStencilView();
private:
	Game();
	static Game instance;

	Window* window;
	TestScene scene;
};

NS_END

#endif