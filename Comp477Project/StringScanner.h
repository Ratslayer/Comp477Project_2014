#pragma once
#include "Scanner.h"
class StringScanner :
	public Scanner
{
public:
	StringScanner(string str);
	~StringScanner();
};

