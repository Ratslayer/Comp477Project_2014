#pragma once
#include "stdafx.h"
#include "freeglut\include\GL\glut.h"
#include "Renderer.h"
#include "AssetManager.h"
//#include "Window.h"
namespace MyNamespace
{
	class Window
	{
	public:
		Window(int width, int height, int x, int y, char* title);
		~Window();
		//attributes
		int width;
		int height;
		int initialX;
		int initialY;
		char* title;
		//methods
		void init(int*, char**);
		void initPhysX();
		static void printGlError(GLenum error);
		static void printGlError();
	};
}


