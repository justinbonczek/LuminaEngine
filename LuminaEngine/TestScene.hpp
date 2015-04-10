#ifndef TESTSCENE_HPP
#define TESTSCENE_HPP

#include "Scene.hpp"

class TestScene : public Lumina::Scene
{
public:
	// Overrided functions from Scene
	void LoadAssets();
	void Update(float dt);
	void Draw();
private:
	// TestScene stuff
	Lumina::Camera camera;
	void MoveCamera(float dt);
};

#endif