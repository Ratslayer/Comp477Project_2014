#pragma once
#include "Asset.h"

class Skeleton :
	public Asset
{
public:
	Skeleton();
	~Skeleton();
	void loadFromFile(string fileName);
	string getClassName(){ return "Skeleton"; }
private:
	
};

