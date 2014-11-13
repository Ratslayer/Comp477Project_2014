#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
	for (unsigned int i = 0; i < actors.size(); i++)
		delete actors[i];
}

void Scene::loadFromFile(std::string fileName)
{
	Asset::loadFromFile(fileName);
	Scanner sc(fileName);
	vector<string> objStrings = sc.splitContent('[');
	for (unsigned int i = 0; i < objStrings.size(); i++)
	{
		if (!objStrings[i].empty() && !(objStrings[i][0] == '#'))
		{
			pair<string, string> classContentPair = Scanner::splitString(objStrings[i], ']');
			loadActor(classContentPair.first, classContentPair.second);
		}
	}
	pCamera = getCamera("");
}

void Scene::loadActor(string className, string content)
{
	string lcName = Scanner::toLower(className);

	if (lcName == "gameobject")
	{
		load<GameObject, GameObjectDesc>(content);
	}
	if (lcName == "camera")
	{
		load<Camera, CameraDesc>(content);
	}
}

void Scene::draw(Effect *effect)
{
	pCamera->bind(effect);
	vector<GameObject*> objects = getAllObjects();
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->draw(effect);
	}
}

Camera* Scene::getCamera(string name)
{
	Camera* pNewCamera = NULL;
	for (unsigned int i = 0; i < actors.size(); i++)
	{
		pNewCamera = dynamic_cast<Camera*>(actors[i]);
		if (pNewCamera)
			break;
	}
	return pNewCamera;
}

vector<GameObject*> Scene::getAllObjects()
{
	vector<GameObject*> objects;
	for (unsigned int i = 0; i < actors.size(); i++)
	{
		GameObject *pObject = dynamic_cast<GameObject*>(actors[i]);
		if (pObject)
			objects.push_back(pObject);
	}
	return objects;
}
