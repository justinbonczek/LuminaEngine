#ifndef SCENE_H
#define SCENE_H

#include "Config.hpp"
#include "Light.hpp"
#include "Material.hpp"

NS_BEGIN

class Window;
class GameObject;
class Camera;
class Game;

struct PerFrameData
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT3 eyePos;
	float elapsedTime;
};

struct PerObjectData
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 worldInverseTranspose;
	LightMaterial lightMat;
};

struct LightData
{
	DirectionalLight dLight[NUMDIRECTIONALLIGHTS];
	PointLight pLight[NUMPOINTLIGHTS];
	SpotLight sLight[NUMSPOTLIGHTS];
	UINT numDL;
	UINT numPL;
	UINT numSL;
	UINT numShadowMaps;
};

// TODO: Window references should be Game references

class Scene
{
public:
	Scene();
	virtual ~Scene();

	/// <summary>
	/// OVERRIDE THIS
	/// Runs at initialization to load assets into memory and create objects needed
	/// </summary>
	virtual void LoadAssets(Window& window) = 0;

	/// <summary>
	/// OVERRIDE THIS
	/// Handles game logic updates
	/// </summary>
	virtual void Update(float dt) = 0;

	/// <summary>
	/// OVERRIDE THIS
	/// Renders objects to the window
	/// </summary>
	virtual void Draw(Window& window) = 0;

	/// <summary>
	/// Does some heavy lifting and sets up the graphics pipeline for rendering
	/// </summary>
	virtual void InitializePipeline(Window& window);
protected:
	Game* game;

	/// <summary>
	/// Updates GameObjects. Should be called in Update()
	/// </summary>
	virtual void UpdateObjects(float dt);
	
	/// <summary>
	/// Draws GameObjects. Should be called in Draw()
	/// </summary>
	virtual void DrawScene();

	/// <summary>
	/// Sets the back buffer as the render target
	/// </summary>
	void SetRenderTarget();

	/// <summary>
	/// Sends necessary data to the GPU once per frame
	/// </summary>
	virtual void UpdateFrameData();

	/// <summary>
	/// Sends necessary data to the GPU once per GameObject
	/// </summary>
	virtual void UpdateObjectData(GameObject& obj);

	/// <summary>
	/// Adds a game object to the list of objects
	/// </summary>
	void AddGameObject(GameObject* obj);

	/// <summary>
	/// Adds a directional light to the scene up to the maximum number of lights
	/// </summary>
	void AddLight(DirectionalLight* light);

	/// <summary>
	/// Adds a point light to the scene up to the maximum number of lights
	/// </summary>
	void AddLight(PointLight* light);

	/// <summary>
	/// Adds a spot light to the scene up to the maximum number of lights
	/// </summary>
	void AddLight(SpotLight* light);

	/// <summary>
	/// Sets the given camera as the current active camera
	/// </summary>
	void SetActiveCamera(Camera& camera);

	/// <summary>
	/// Renders the shadow maps for all given lights. Will disable pixel 
	/// </summary>
	void CalculateShadows();

	/// <summary>
	/// 
	/// </summary>
	void CreateDirectionalShadowMap(DirectionalLight* dLight);

	/// <summary>
	/// 
	/// </summary>
	void CreateSpotShadowMap(SpotLight* sLight);

	/// <summary>
	/// 
	/// </summary>
	void CreatePointShadowMap(PointLight* pLight);

	Camera* activeCamera;

	std::vector<GameObject*> objs;

	ID3D11InputLayout* inputLayout;

	PerFrameData perFrameData;
	ID3D11Buffer* perFrameBuffer;

	PerObjectData perObjectData;
	ID3D11Buffer* perObjectBuffer;

	LightData lightData;
	ID3D11Buffer* lightBuffer;

	ID3D11ShaderResourceView* shadowMaps[NUMDIRECTIONALLIGHTS + NUMPOINTLIGHTS + NUMSPOTLIGHTS];
};

NS_END

#endif