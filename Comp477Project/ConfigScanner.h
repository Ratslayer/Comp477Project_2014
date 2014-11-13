#pragma once
#include "Scanner.h"
#include "BindingCollection.h"
class ConfigScanner :
	public Scanner
{
public:
	
	ConfigScanner(string fileName);
	~ConfigScanner();
	/*template <typename T>
	T get(string name)
	{
		string value = params.get(name);
		istringstream vStream(value);
		T f;
		vStream >> f;
		return f;
	}*/
	BindingCollection params;
};

