#include "GameObject.hpp"

NS_BEGIN

GameObject::GameObject():
position(0, 0, 0),
rotation(0, 0, 0),
scale(0, 0, 0),
stride(sizeof(Vertex)),
offset(0)
{

}

GameObject::GameObject(Mesh* mesh) :
mesh(mesh),
stride(sizeof(Vertex)),
offset(0)
{
	worldMat = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 };
}

GameObject::GameObject(Material* mat) :
mat(mat),
stride(sizeof(Vertex)),
offset(0)
{
	XMStoreFloat4x4(&worldMat, XMMatrixIdentity());
	position = { 0.0, 0.0, 0.0 };
	rotation = { 0.0, 0.0, 0.0 };
	scale = { 1.0, 1.0, 1.0 };
}

GameObject::GameObject(Mesh* mesh, Material* mat) :
mesh(mesh),
mat(mat),
stride(sizeof(Vertex)),
offset(0)
{
	worldMat = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 };
	position = { 0.0, 0.0, 0.0 };
	rotation = { 0.0, 0.0, 0.0 };
	scale = { 1.0, 1.0, 1.0 };
}

GameObject::~GameObject()
{

}

void GameObject::Update(float dt)
{
	worldMat = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0 };

	// Do the math!
	XMMATRIX rotationX = XMMatrixRotationX(rotation.x);
	XMMATRIX rotationY = XMMatrixRotationY(rotation.y);
	XMMATRIX rotationZ = XMMatrixRotationZ(rotation.z);
	XMMATRIX scaleM = XMMatrixScalingFromVector(XMLoadFloat3(&scale));
	XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
	XMStoreFloat4x4(&worldMat, scaleM * rotationX * rotationY * rotationZ * translation);
}

void GameObject::Draw(ID3D11DeviceContext* devCon)
{
	// TODO: SHADOW PASS OPTIMIZATION
	if (!shadowPass)
		mat->BindShader(devCon);
	else
	{
		mat->BindShader(devCon);
		mat->UnbindPixelShader(devCon);
	}

	mat->BindSampler(devCon);
	mat->BindSRV(devCon);
	mat->BindBlendState(devCon);

	if (mesh->VBuffer())
	{
		ID3D11Buffer* vBuffer = mesh->VBuffer();
		devCon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	}
	if (mesh->IBuffer())
	{
		devCon->IASetIndexBuffer(mesh->IBuffer(), DXGI_FORMAT_R32_UINT, 0);
	}

	devCon->DrawIndexed(mesh->GetNumIndices(), 0, 0);
}

void GameObject::SetPosition(XMFLOAT3 newPosition)
{
	position = newPosition;
}

void GameObject::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
}

void GameObject::SetScale(XMFLOAT3 newScale)
{
	scale = newScale;
}

void GameObject::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
}

void GameObject::SetRotation(XMFLOAT3 newRotation)
{
	rotation = newRotation;
}

void GameObject::SetRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
}

XMFLOAT4X4 GameObject::WorldTranspose(void)
{
	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, XMMatrixTranspose(XMLoadFloat4x4(&worldMat)));
	return temp;
}

XMFLOAT4X4 GameObject::WorldInverseTranspose(void)
{
	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&worldMat))));
	return temp;
}

void GameObject::LoadTexture(wchar_t* filepath, ID3D11Device* dev)
{
	mat->LoadTexture(filepath, dev);
}

void GameObject::SetShader(Shader* shader)
{
	mat->SetShader(shader);
}

void GameObject::SetShader(wchar_t* filepath, ShaderType type, ID3D11Device* dev)
{
	mat->SetShader(filepath, type, dev);
}

LightMaterial GameObject::LightMaterial(void)
{
	return mat->GetLightMaterial();
}

UINT GameObject::GetTextureTileU()
{
	return mat->GetTextureTileU();
}

UINT GameObject::GetTextureTileV()
{
	return mat->GetTextureTileV();
}

void GameObject::SetShadowPass(bool val)
{
	shadowPass = val;
}

NS_END