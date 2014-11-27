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
#include "SkinnedModel.h"
namespace MyNamespace
{
	class Renderer
	{
	private:
		static Renderer* _renderer;
		
		SkinnedModel *pHero;
		Effect *pEffect, *pPostProcessEffect, *pSkinEffect;
		FrameBuffer *pBuffer;
		KinectController *pKinect;
		//GameObject *pObject;
		//Model* model;
		//Texture* texture;
		//Camera* camera;
		//Mesh* mesh;
		void draw2DVertex(float x, float y);
		void drawFullscreenQuad();
		void drawScene(Scene* scene, Effect* effect, FrameBuffer *buffer);
		void drawLines(vector<vec3> vertices, vec3 color);
	public:
		Renderer();
		~Renderer();
		Scene *pScene;
		static Renderer* getRenderer();
		static void init();
		static void release();
		
		void clear();
		void draw();
		void bindBackBuffer();
	};
}

