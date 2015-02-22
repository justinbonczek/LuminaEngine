#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include "Lumina.hpp"
#include "Scene.hpp"

class TestScene : public Lumina::Scene
{
public:
	// Overrided functions from Scene
	void LoadAssets(Lumina::Window& window);
	void Update(float dt);
	void Draw(Lumina::Window& window);
private:
	// TestScene stuff
	Lumina::Camera camera;
	void MoveCamera(float dt);
};

#endif