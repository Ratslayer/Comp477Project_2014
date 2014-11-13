#pragma once
#include "freeglut\include\GL\glut.h"
#include "ShaderFactory.h"
#include "Effect.h"
#include "Camera.h"
#include "Model.h"
#include "GameObject.h"
#include "Scene.h"
#include "FrameBuffer.h"
#include "KinectController.h"
namespace MyNamespace
{
	class Renderer
	{
	private:
		static Renderer* _renderer;
		Scene *pScene;
		Effect *pEffect, *pPostProcessEffect;
		FrameBuffer *pBuffer;
		KinectController *pKinect;
		//GameObject *pObject;
		//Model* model;
		//Texture* texture;
		//Camera* camera;
		//Mesh* mesh;
		void draw2DVertex(float x, float y);
		void drawFullscreenQuad();
	public:
		Renderer();
		~Renderer();

		static Renderer* getRenderer();
		static void init();
		static void release();
		
		void clear();
		void draw();
		void bindBackBuffer();
	};
}

