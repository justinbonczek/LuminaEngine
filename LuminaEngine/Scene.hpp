#ifndef SCENE_H
#define SCENE_H

#include "Config.hpp"
#include "Lumina.hpp"

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
	UINT tileU;
	UINT tileV;
	float pad[2];
};

struct LightData
{
	DirectionalLight dLight[NUMDIRECTIONALLIGHTS];
	PointLight pLight[NUMPOINTLIGHTS];
	SpotLight sLight[NUMSPOTLIGHTS];
	UINT numDL;
	UINT numPL;
	UINT numSL;
	float pad;
};

struct ShadowData
{
	XMFLOAT4X4 sView[TOTALNUMLIGHTS];
	XMFLOAT4X4 sProj[TOTALNUMLIGHTS];
	UINT numSM;
	float resolution;
	float pad[2];
};

// TODO: Window references should be Game references

class Scene
{
public:
	Scene();
	virtual ~Scene();

	void Initialize(GraphicsDevice* _graphicsDevice);

	/// <summary>
	/// OVERRIDE THIS
	/// Runs at initialization to load assets into memory and create objects needed
	/// </summary>
	virtual void LoadAssets() = 0;

	/// <summary>
	/// OVERRIDE THIS
	/// Handles game logic updates
	/// </summary>
	virtual void Update(float dt) = 0;

	/// <summary>
	/// OVERRIDE THIS
	/// Renders objects to the window
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// Does some heavy lifting and sets up the graphics pipeline for rendering
	/// </summary>
	virtual void InitializePipeline();
protected:
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
	void UpdateObjectData(GameObject& obj);

	void UpdateParticleObjectData(ParticleEmitter* e);

	/// <summary>
	/// Adds a game object to the list of objects
	/// </summary>
	void AddGameObject(GameObject* obj);

	/// <summary>
	/// 
	/// </summary>
	void AddSystem(ParticleEmitter* pEmitter);

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
	/// 
	/// </summary>
	void AddShadowMap(DirectionalLight* light);
	
	/// <summary>
	/// 
	/// </summary>
	void AddShadowMap(PointLight* light);
	
	/// <summary>
	/// 
	/// </summary>
	void AddShadowMap(SpotLight* light);

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
	void SetShadowMap(ShadowMap* shadowMap, int index);

	GraphicsDevice* graphicsDevice;
	Camera* activeCamera;

	std::vector<GameObject*> objs;
	std::vector<ParticleEmitter*> particles;

	ID3D11InputLayout* inputLayout;
	ID3D11InputLayout* particleInputLayout;

	PerFrameData perFrameData;
	ID3D11Buffer* perFrameBuffer;

	PerObjectData perObjectData;
	ID3D11Buffer* perObjectBuffer;

	LightData lightData;
	ID3D11Buffer* lightBuffer;

	ShadowData shadowData;
	ID3D11Buffer* shadowBuffer;

	ShadowMap* shadowMaps[NUMDIRECTIONALLIGHTS + NUMPOINTLIGHTS + NUMSPOTLIGHTS];
};

NS_END

#endif