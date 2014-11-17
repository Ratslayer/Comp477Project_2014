#pragma once
#include "stdafx.h"
/*
	This class is the parent class for every object that is loaded from a file.
	If you wish to extend it and keep the loading functionality, you have to do the following:
	1.	Have a Child() constructor that initializes everything to default.
	2.	Override function getClassName() so it returns a string corresponding to the name of the class (usually capitalized)
	3.	Override loadFromFile(string fileName) and put the code that loads it in there
	4.	Add an entry to the file Comp477Project/Configs/Assets.txt, 
		ClassName=FolderName where classname if the name returned by Child::getClassName() 
		and FolderName is the name of the folder with respect to Comp477Project/Assets/
*/
class Asset
{
public:
	Asset();
	virtual ~Asset();
	std::string fileName;
	virtual std::string getClassName() = NULL;
	virtual void loadFromFile(std::string fileName);
};

