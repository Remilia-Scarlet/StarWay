#pragma once
#include "DangoEngine/Objects/Object.h"
#include <map>
#include <functional>
#include "Ash/RefCountPointer/RefCountObj.h"

ASH_DEFINE_PTR(Scene);
class Scene : public Ash::RefCountObj
{
public:
	Scene();
	virtual ~Scene();
public:
	static ScenePtr create();

	virtual void addObject(ObjectPtr obj);
//@@TODO:	virtual void removeObject(...);

	virtual void removeObject(ObjectPtr obj);

	virtual void update(Ash::FunctorSeq& seq, float dt);
	virtual void render();


	virtual bool start() { return true; }
protected:
	virtual bool init();
	std::map<Ash::ObjectID, ObjectWeakPtr> _cameras;//we need to sort cameras for draw
	std::unordered_map<Ash::ObjectID, ObjectWeakPtr> _lights;
	std::unordered_map<Ash::ObjectID,ObjectPtr> _objects;

};

