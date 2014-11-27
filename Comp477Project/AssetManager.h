#pragma once
#include "stdafx.h"
#include "Asset.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "ConfigScanner.h"
class AssetManager
{
public:
	AssetManager(string configFile);
	~AssetManager();
	static void init();
	static void release();
	template<typename T>
	static T* load(string name)
	{
		T* asset = NULL;
		if (!name.empty())
		{
			string fullName = _manager->getClassPath(T()) + "\\" + name;
			asset = (T*)_manager->getAssetByName(fullName);
			if (!asset)
			{
				asset = new T();
				asset->loadFromFile(fullName);
				_manager->assets.push_back(asset);
			}
		}
		return asset;
	}
	template<typename T>
	static string getClassPath(T &obj)
	{
		string path;
		path = _manager->pathNames.get(obj.getClassName());
		return _manager->getAssetsPath() + "\\" + path;
	}
private:
	static AssetManager* _manager;
	BindingCollection pathNames;
	vector<Asset*> assets;
	string getAssetsPath();
	
	
	Asset* getAssetByName(string &name);
	string getFullPath(string path);
};

