#include "stdafx.h"
#include "StringScanner.h"


StringScanner::StringScanner(string str)
{
	data = str;
	dataStream = istringstream(data);
}


StringScanner::~StringScanner()
{
}
