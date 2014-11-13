#include "stdafx.h"
#include "Asset.h"


Asset::Asset()
{
}
Asset::~Asset()
{
}
void Asset::loadFromFile(std::string fileName)
{
	this->fileName = fileName;
}
