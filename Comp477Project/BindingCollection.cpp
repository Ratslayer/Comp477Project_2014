#include "stdafx.h"
#include "BindingCollection.h"
BindingCollection::BindingCollection()
{
}

BindingCollection::BindingCollection(string data)
{
	string line;
	istringstream dataStream(data);
	while (getline(dataStream, line))
	{
		if (line.size() > 0 && line[0] != '#')
		{
			pair<string, string> param= Scanner::splitString(line, '=');
			/*istringstream lineStream(line);
			getline(lineStream, param.first, '=');
			getline(lineStream, param.second);*/
			bindings.push_back(param);
		}
	}
}

BindingCollection::~BindingCollection()
{
}

string BindingCollection::get(string name)
{
	string result;
	for (unsigned int i = 0; i < bindings.size(); i++)
	{
		pair<string, string> &param = bindings[i];
		if (Scanner::lcEquals(param.first, name))
		{
			result = param.second;
			break;
		}
	}
	if (result.empty())
		cout << "Could not find binding with name " + name << endl;
	return result;
}

vec3 BindingCollection::getVec3(string name)
{
	vec3 result(0,0,0);
	string str = get(name);
	if (!str.empty())
	{
		istringstream ss(str);
		ss >> result.x;
		ss >> result.y;
		ss >> result.z;
	}
	return result;
}

vec4 BindingCollection::getVec4(string name)
{
	vec4 result(0, 0, 0, 0);
	string str = get(name);
	if (!str.empty())
	{
		istringstream ss(str);
		ss >> result.x;
		ss >> result.y;
		ss >> result.z;
		ss >> result.w;
	}
	return result;
}

quat BindingCollection::getQuat(string name)
{
	vec4 result = getVec4(name);
	return result.toQuat();
}

/*float BindingCollection::getFloat(string name)
{
	float result = 0;
	string str = get(name);
	if (!str.empty())
	{
		istringstream ss(str);
		ss >> result;
	}
	return result;
}

int BindingCollection::getInt(string name)
{
	int result = 0;
	string str = get(name);
	if (!str.empty())
	{
		istringstream ss(str);
		ss >> result;
	}
	return result;
}*/
