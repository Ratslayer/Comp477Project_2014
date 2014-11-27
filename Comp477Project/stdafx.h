// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <NxPhysics.h>
#include <NxPhysicsSDK.h>
extern float g_timeStep;
extern NxPhysicsSDK *g_pSDK;
extern NxScene *g_physXScene;

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "math.h"
//#include <tchar.h
//#include "glew-1.11.0\include\GL\glew.h"
#include <GL\glew.h>
#include "freeglut\include\GL\freeglut.h"
#include "ConfigScanner.h"
#pragma comment(lib, "PhysXLoader.lib")
using namespace std;



// TODO: reference additional headers your program requires here
