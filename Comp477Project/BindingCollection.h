#pragma once
/*
	This class is used to store parsed files that use the config storage convention
	Every file of this type uses name=data format, where name is the name of the variable and data is string representing the data associated with that variable
	To get data associated with the variable name, you have to use various get functions to convert data to the data type you need
*/
class BindingCollection
{
public:
	BindingCollection();
	BindingCollection(string data);
	~BindingCollection();
	//accessors and transformers for second half of bindings
	//get binding data by name
	string get(string name);
	//get data as various data types
	vec3 getVec3(string name);
	vec4 getVec4(string name);
	quat getQuat(string name);
	//get data as a primitive data type, such as boolean, float, int, etc.
	template<typename T>
	T getPrimitive(string name)
	{
		T result = 0;
		string str = get(name);
		if (!str.empty())
		{
			istringstream ss(str);
			ss >> result;
		}
		return result;
	}
	//float getFloat(string name);
	//int getInt(string name);
	vector<pair<string, string>> bindings;
};

