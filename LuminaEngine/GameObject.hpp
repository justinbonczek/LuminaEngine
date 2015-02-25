#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Config.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Window.hpp"

NS_BEGIN

class GameObject
{
public:
	GameObject();
	GameObject(Mesh* mesh);
	GameObject(Material* mat);
	GameObject(Mesh* mesh, Material* mat);

	virtual ~GameObject();
	
	/// <summary>
	/// Updates object logic to time
	/// </summary>
	virtual void Update(float dt);

	/// <summary>
	/// Sets proper graphics pipeline values and renders the object
	/// </summary>
	virtual void Draw(ID3D11DeviceContext* devCon);

	/// <summary>
	/// Sets the position of the object to the new value
	/// </summary>
	void SetPosition(XMFLOAT3 newPosition);
	void SetPosition(float x, float y, float z);

	/// <summary>
	/// Sets the scale of the object to the new value
	/// </summary>
	void SetScale(XMFLOAT3 newScale);
	void SetScale(float x, float y, float z);

	/// <summary>
	/// Sets the rotation of the object to the new value
	/// </summary>
	void SetRotation(XMFLOAT3 newRotation);
	void SetRotation(float x, float y, float z);

	/// <summary>
	/// Returns the transpose of the world matrix
	/// </summary>
	XMFLOAT4X4 WorldTranspose(void);

	/// <summary>
	/// Returns the inverse transpose of the world matrix
	/// </summary>
	XMFLOAT4X4 WorldInverseTranspose(void);

	/// <summary>
	/// Returns the object's LightMaterial
	/// </summary>
	LightMaterial LightMaterial(void);

	/// <summary>
	/// 
	/// </summary>
	UINT GetTextureTileU();
	
	/// <summary>
	/// 
	/// </summary>
	UINT GetTextureTileV();

	/// <summary>
	/// Sets whether or not the object should be drawn as if it is a shadow pass
	/// </summary>
	void SetShadowPass(bool val);
protected:
	XMFLOAT4X4 worldMat;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	
	Mesh* mesh;
	Material* mat;

	UINT stride;
	UINT offset;

	bool shadowPass;
};

NS_END

#endif