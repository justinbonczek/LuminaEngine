//
// A basic 3d camera class 
// Credit to Frank D. Luna for reference
//

#ifndef CAMERA_H
#define CAMERA_H

#include "Config.hpp"
#include "Mesh.hpp"
#include "GraphicsDevice.hpp"
NS_BEGIN
class Camera
{
public:
	Camera();
	~Camera();

	/// <summary>
	/// Sets up initial values for camera
	/// </summary>
	void Initialize(Lumina::GraphicsDevice graphicsDevice);

	///
	// Gets and sets to access and change camera information
	///
	XMVECTOR GetPositionXM() const;
	XMFLOAT3 GetPosition()   const;
	void	 SetPosition(float x, float y, float z);
	void	 SetPosition(XMFLOAT3& v);

	XMVECTOR GetRightXM() const;
	XMFLOAT3 GetRight()   const;
	XMVECTOR GetUpXM() const;
	XMFLOAT3 GetUp()   const;
	XMVECTOR GetLookXM() const;
	XMFLOAT3 GetLook()   const;

	float GetNearZ()  const;
	float GetFarZ()   const;
	float GetAspect() const;
	float GetFovX()   const;
	float GetFovY()   const;

	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	/// <summary>
	/// Sets up the view frustum
	/// </summary>
	void SetLens(float fovY, float aspect, float zn, float zf);

	/// <summary>
	/// Adjusts the view matrix to look at a specific point
	/// </summary>
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);

	/// <summary>
	/// Adjusts the view matrix to look at a specific point
	/// </summary>
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	/// <summary>
	/// Returns the view matrix
	/// </summary>
	XMMATRIX ViewXM() const;
	
	XMFLOAT4X4 View() const;
	XMFLOAT4X4 ViewTranspose() const;

	/// <summary>
	/// Returns the projection matrix
	/// </summary>
	XMMATRIX ProjXM() const;
	
	XMFLOAT4X4 Proj() const;
	XMFLOAT4X4 ProjTranspose() const;

	/// <summary>
	/// Returns the view * projection matrix
	/// </summary>
	XMMATRIX ViewProj() const;

	/// <summary>
	/// Moves d in the right direction
	/// </summary>
	void Strafe(float d);

	/// <summary>
	/// Moves d in the forward direction
	/// </summary>
	void Walk(float d);

	/// <summary>
	/// Pitches by angle
	/// </summary>
	void Pitch(float angle);

	/// <summary>
	/// Rotates Y axis by angle
	/// </summary>
	void RotateY(float angle);

	/// <summary>
	/// Updates the view matrix. Should be called before rendering, or when the camera's position changes.
	/// </summary>
	void UpdateViewMatrix();
private:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Right;
	XMFLOAT3 m_Up;
	XMFLOAT3 m_Look;

	UINT stride;
	UINT offset;

	float m_NearZ;
	float m_FarZ;
	float m_Aspect;
	float m_FovX;
	float m_FovY;
	float m_NearWindowHeight;
	float m_FarWindowHeight;
	float m_NearWindowWidth;
	float m_FarWindowWidth;

	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;
};
NS_END
#endif