#include "stdafx.h"
#include "AssetManager.h"
#include "ConfigScanner.h"

AssetManager* AssetManager::_manager = NULL;
AssetManager::AssetManager(string configFile)
{
	ConfigScanner sc(configFile);
	pathNames = sc.params;
}


AssetManager::~AssetManager()
{
	for (unsigned int i = 0; i < assets.size(); i++)
		delete assets[i];
}

std::string AssetManager::getAssetsPath()
{
	string filePath = pathNames.get("Assets");
	filePath = getFullPath(filePath);
	return filePath;
}

Asset* AssetManager::getAssetByName(std::string &name)
{
	Asset* asset = NULL;
	for (unsigned int i = 0; i < assets.size(); i++)
	{
		if (assets[i]->fileName == name)//assets[i]->fileName == name)
		{
			asset = assets[i];
			break;
		}
	}
	return asset;
}

void AssetManager::init()
{
	_manager = new AssetManager("..\\Comp477Project\\Configs\\assets.txt");
}

void AssetManager::release()
{
	delete _manager;
}

string AssetManager::getFullPath(string path)
{
	char full[_MAX_PATH];
	_fullpath(full, ("..\\Comp477Project\\" + path).c_str(), _MAX_PATH);
	return string(full);
	//return "..\\Comp477Project\\" + path;
}