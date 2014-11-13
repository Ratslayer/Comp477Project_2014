#pragma once
#include "stdafx.h"
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;
class Scanner
{
public:
	Scanner();
	Scanner(string file);
	~Scanner();
	void jumpAfter(string str);
	static pair<string, string> splitString(string str, char delim);
	static string toLower(string str);
	static bool lcEquals(string str1, string str2);
	vector<string> splitContent(char delim);
	float getFloat();
	unsigned int getUInt();
	//bindings functions
private:
	void loadData(string file);
protected:
	string fileName;
	string data;
	istringstream dataStream;
};

