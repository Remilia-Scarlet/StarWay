#pragma once
#include "Math\vector\Vector.h"
#include "Graphic\gfx\GfxRenderBuffer.h"

const static int MAX_LIGHT = 5;

struct DirectionLight
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector3 direction;
	float pad;
};

struct PointLight
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;

	Vector3 position;
	float range;

	Vector3 att;
	float pad;
};

struct SpotLight
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;

	Vector3 position;
	float range;

	Vector3 direction;
	float spot;

	Vector3 att;
	float pad;
};

struct LightInfo
{
	int directLightNumber;
	int pointLightNumber;
	int spotLightNumber;
	int __pad;
	DirectionLight directionalLight[MAX_LIGHT];
	PointLight pointLight[MAX_LIGHT];
	SpotLight spotLight[MAX_LIGHT];
};

class LightManager
{
public:
	static bool createInstance();
	static void destroyInstance();
	static LightManager* instance();
public:
	void applyDirectionLight(const DirectionLight& directionLight);
	void applyPointLight(const PointLight& pointLight);

	void preRender();
	void render();
protected:
	LightManager();
	virtual ~LightManager();
	static LightManager* s_instance;

	virtual bool init();

	LightInfo _lightInfo;
};

