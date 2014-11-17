#pragma once
#include "stdafx.h"
#include "Asset.h"
#include "Actor.h"
#include "Camera.h"
#include "Light.h"
#include "GameObject.h"
#include "Effect.h"
class Scene
	: public Asset
{
public:
	Scene();
	~Scene();
	void loadFromFile(std::string fileName);
	std::string getClassName(){ return "Scene"; }
	void draw(Effect *effect);

	Camera* pCamera;
	vector<Actor*> actors;

	Camera* getCamera(string name);
	vector<GameObject*> getAllObjects();
	template<typename T> 
	T* getActor(string name)
	{
		T* actor = nullptr;
		for (unsigned int i = 0; i < actors.size(); i++)
		{
			if (actors[i]->name == name)
			{
				actor = dynamic_cast<T*>(actors[i]);
				break;
			}
		}
		return actor;
	}
	void loadActor(string className, string content);

	template<typename classType, typename descType>
	void load(string &data)
	{
		classType *actor=new classType();
		actor->loadFromData<descType>(data);
		actors.push_back(actor);
	}
};