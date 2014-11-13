#include "stdafx.h"
#include "FileHelper.h"


FileHelper::FileHelper()
{
}


FileHelper::~FileHelper()
{
}

/*vector<pair<string, string>> FileHelper::getConfigParams(string &configFile)
{
	vector<pair<string, string>> configParams;
	vector<string> lines = splitString(configFile, "\n");
	for (int i = 0; i < lines.size(); i++)
	{
		vector<string> param = splitString(lines[i], "=");
		//if (param.size() != 2)
		//	throw string(string("Line ") + i);
		pair<string, string> configParam(lines[0], lines[1]);
		configParams.push_back(configParam);
	}
	return configParams;
}

/*vector<pair<string, string>> FileHelper::getConfigBlock(string &configFile, string blockName)
{
	vector<string> lines = splitString(configFile, "\n");
	for (int i = 0; i < lines.size(); i++)
	{
		string line = lines[i];
		if (isConfigBlock(line))
		{
			if (lines.size()>0)
				break;
			vector<string> blockTitleSplit = splitString(line, "]");
			if (blockTitleSplit.size() > 0)
			{

			}
		}
		else if (lines.size() > 0)
			lines.push_back(line);
	}

}

vector<pair<string, string>> FileHelper::getConfigBlockParams(string &configFile, string blockName)
{
	string block = getConfigBlock(configFile, blockName);
	vector<pair<string, string>> blockParams = getConfigParams(block);
}*/
