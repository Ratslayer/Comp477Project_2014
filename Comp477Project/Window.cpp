#include "stdafx.h"
#include "Window.h"
using namespace MyNamespace;

Window::Window(int width, int height, int x, int y, char* title)
{
	this->width = width;
	this->height = height;
	initialX = x;
	initialY = y;
	this->title = title;
}
Window::~Window()
{
	Renderer::release();
	AssetManager::release();
}
void renderScene(void) 
{
	Renderer::getRenderer()->draw();
}
void timerFunction(int value)
{
	Renderer::getRenderer()->draw();
	glutPostRedisplay();
	glutTimerFunc(10, timerFunction, 1);
}
void Window::init(int* argc, char** argv)
{
	glutInit(argc, argv);
	ilInit();
	ilutInit();
	glutInitWindowPosition(initialX, initialY);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(title);
	GLenum glError = glewInit();
	//printGlError(glError);
	//init singletons before attempting to render the scene
	//ShaderFactory::init();
	AssetManager::init();
	Renderer::init();
	glutTimerFunc(10, timerFunction, 1);
	glutDisplayFunc(renderScene);
	glutMainLoop();
}
void Window::printGlError(GLenum error)
{
	char* err = (char*)glewGetErrorString(error);
	std::string errorString = std::string("GL Error: ") + err;
	std::cout << errorString << ". #" << glGetError() << std::endl;
}
void Window::printGlError()
{
	printGlError(glGetError());
}

