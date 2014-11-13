// Comp477Project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Window.h"
int main(int argc, char* argv[])
{
	
	MyNamespace::Window window(1024, 768, 0, 0, "Comp 477 Project");
	window.init(&argc, argv);
	return 1;
}