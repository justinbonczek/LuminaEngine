#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Config.hpp"
NS_BEGIN
/// <summary>
/// Abstract class for lights
/// </summary>
class Light
{
public:
	Light();
	virtual ~Light();
	XMFLOAT3 dPad;

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	bool hasShadows;
	XMFLOAT3 pad;
private:

};

/// <summary>
/// Sun-like light
/// </summary>
class DirectionalLight : public Light
{
public:
	DirectionalLight();

	XMFLOAT3 direction;
	float pad3;
private:

};

/// <summary>
/// Lightbulb-like light
/// </summary>
class PointLight : public Light
{
public:
	PointLight();

	XMFLOAT3 position;
	float range;
	XMFLOAT3 attenuation;
	float pad3;
private:

};

/// <summary>
/// Flashlight-like light
/// </summary>
class SpotLight : public Light
{
public:
	SpotLight();

	XMFLOAT3 position;
	float range;
	XMFLOAT3 direction;
	float spot;
	XMFLOAT3 attenuation;
	float pad3;
private:

};

NS_END

#endif