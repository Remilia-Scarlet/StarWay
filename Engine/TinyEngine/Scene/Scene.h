#pragma once
#include "TinyEngine/Objects/Object.h"
#include <unordered_map>
#include <functional>
#include "Ash/RefCountPointer/RefCountObj.h"
TINY_DEFINE_PTR(Scene);

class Scene : public RefCountObj
{
public:
	Scene();
	virtual ~Scene();
public:
	static ScenePtr create();
	int aaa;

	virtual void addObject(ObjectPtr obj);
//@@TODO:	virtual void removeObject(...);

	virtual void update(float dt);
	virtual void render();


	virtual bool start() { return true; }
protected:
	virtual bool init();
	std::unordered_map<ObjectID, ObjectWeakPtr> _cameras;
	std::unordered_map<ObjectID,ObjectPtr> _objects;

};

