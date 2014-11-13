#include "stdafx.h"
#include "Scanner.h"

Scanner::Scanner()
{
}
Scanner::Scanner(string fileName)
{
	this->fileName = fileName;
	loadData(fileName);
}
Scanner::~Scanner()
{
}

void Scanner::loadData(string file)
{
	ifstream stream(file);
	if (stream.is_open())
	{
		data = string((std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>());
		dataStream = istringstream(data);
	}
}

void Scanner::jumpAfter(string str)
{
	string next;
	while (dataStream >> next)
	{
		if (str.size() <= next.size())
		{
			char first = str[0];
			unsigned int firstIndex = 0;
			while (firstIndex < next.size())
			{
				if (next[firstIndex] == first)
					break;
				else firstIndex++;
			}
			bool hasToken = true;
			for (unsigned int i = 0; i < str.size(); i++)
			{
				int nextIndex = firstIndex + i;
				if (str[i] != next[nextIndex])
				{
					hasToken = false;
					break;
				}
			}
			if (hasToken)
				break;
		}
	}
}

float Scanner::getFloat()
{
	float f;
	dataStream >> f;
	return f;
}

unsigned int Scanner::getUInt()
{
	unsigned int ui;
	dataStream >> ui;
	return ui;
}

pair<string, string> Scanner::splitString(string str, char delim)
{
	pair<string, string> result;
	istringstream lineStream(str);
	getline(lineStream, result.first, delim);
	getline(lineStream, result.second, '~'); 
	//getline(lineStream, result.second);
	return result;
}

vector<string> Scanner::splitContent(char delim)
{
	vector<string> result;
	pair<string, string> splitPair = Scanner::splitString(data, delim);
	result.push_back(splitPair.first);
	while (!splitPair.second.empty())
	{
		splitPair = Scanner::splitString(splitPair.second, delim);
		result.push_back(splitPair.first);
	}
	return result;
}

string Scanner::toLower(string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

bool Scanner::lcEquals(string str1, string str2)
{
	bool result = Scanner::toLower(str1) == Scanner::toLower(str2);
	return result;
}
