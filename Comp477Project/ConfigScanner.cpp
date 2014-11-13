#include "stdafx.h"
#include "ConfigScanner.h"


ConfigScanner::ConfigScanner(string fileName)
	:Scanner(fileName)
{
	params = BindingCollection(data);
	/*string line;
	while (getline(dataStream, line))
	{
		if (line.size() > 0 && line[0] != '#')
		{
			pair<string, string> param;
			istringstream lineStream(line);
			getline(lineStream, param.first, '=');
			getline(lineStream, param.second);
			params.bindings.push_back(param);
		}
	}*/
}


ConfigScanner::~ConfigScanner()
{
}


