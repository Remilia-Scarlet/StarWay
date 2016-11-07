#pragma once
#include "Math\vector\Vector.h"

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
	Vector4 lightNumber;//x : direction light number; y : point light number; z : spot light number; w : not used
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

	void preRender();
	void render();
protected:
	LightManager();
	virtual ~LightManager();
	static LightManager* s_instance;

	virtual bool init();

	LightInfo _lightInfo;
};

