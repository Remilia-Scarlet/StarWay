#pragma once
#include "TinyEngine/Objects/Object.h"
#include <map>
#include <functional>
#include "Ash/RefCountPointer/RefCountObj.h"

TINY_DEFINE_PTR(Scene);
class Task;
class Scene : public RefCountObj
{
public:
	Scene();
	virtual ~Scene();
public:
	static ScenePtr create();

	virtual void addObject(ObjectPtr obj);
//@@TODO:	virtual void removeObject(...);

	virtual void removeObject(ObjectPtr obj);

	virtual void update(Task* task, float dt);
	virtual void render();


	virtual bool start() { return true; }
protected:
	virtual bool init();
	std::map<ObjectID, ObjectWeakPtr> _cameras;//we need to sort cameras for draw
	std::unordered_map<ObjectID, ObjectWeakPtr> _lights;
	std::unordered_map<ObjectID,ObjectPtr> _objects;

};

