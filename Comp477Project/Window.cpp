#include "stdafx.h"
#include "Window.h"
using namespace MyNamespace;
float g_timeStep = 0.18f;
NxPhysicsSDK *g_pSDK;
NxScene *g_physXScene;
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
	g_pSDK->releaseScene(*g_physXScene);
	g_pSDK->release();
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
	initPhysX();
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

void MyNamespace::Window::initPhysX()
{
	g_pSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
	if (g_pSDK == NULL)
	{
		cerr << "Error creating PhysX device." << endl;
		cerr << "Exiting..." << endl;
		exit(1);
	}


	// Set the debug visualization parameters 
	g_pSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	g_pSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	g_pSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

	// Set scale dependent parameters 
	NxReal scale = 1.f;   // scale is meters per PhysX units 

	// gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.05f*(1.0f /scale)); 
	g_pSDK->setParameter(NX_SKIN_WIDTH, 0.005f*(1.0f / scale));
	g_pSDK->setParameter(NX_DEFAULT_SLEEP_LIN_VEL_SQUARED, 0.001f*0.001f*(1 / scale)*(1 / scale));
	g_pSDK->setParameter(NX_BOUNCE_THRESHOLD, -2 * (1.0f / scale));
	g_pSDK->setParameter(NX_VISUALIZATION_SCALE, 0.5f*(1.0f / scale));



	//Create the scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity.set(0.0f, -9.8f, 0.0f);
	sceneDesc.simType = NX_SIMULATION_SW;
	g_physXScene = g_pSDK->createScene(sceneDesc);

	NxMaterial* defaultMaterial = g_physXScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);


	g_physXScene->setTiming(g_timeStep / 4.0f, 4, NX_TIMESTEP_FIXED);
}

