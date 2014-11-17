#include "stdafx.h"
#include "Renderer.h"
#include "Window.h"
#include "ColorDepthBuffer.h"
using namespace MyNamespace;
Renderer* Renderer::_renderer = NULL;
Renderer::Renderer()
{
	//Shader::Desc shaderDesc;
	//shaderDesc.vertexFileName = "vertex1";
	//shaderDesc.fragmentFileName = "basic";
	//pEffect = new Effect("vertex1.txt", "color.txt");
	pEffect = new Effect("normalTexture.txt", "phongTexture.txt");
	pPostProcessEffect = new Effect("directTexture.txt", "colorTexture.txt");
	pScene = AssetManager::load<Scene>("scene.txt");
	pKinect = new KinectController();
	FBODesc desc;
	desc.width = 1024;
	desc.height = 768;
	pBuffer = new ColorDepthBuffer(1024, 768);
	//texture = AssetManager::load<Texture>("checker.jpg");
	//mesh = AssetManager::load<Mesh>("sphere_1.ASE");
	/*pObject = new GameObject("ball.txt");
	pObject->setPosition(vec3(10, 5, -3));
	//model = AssetManager::load<Model>("ball.txt");
	camera = new Camera();
	camera->fovy = 90;
	camera->setPosition(vec3(1, 3, 10));
	camera->lookAt(vec3(0,0,0));
	camera->setResolution(1024, 768);*/
}


Renderer::~Renderer()
{
	delete pEffect;
	delete pBuffer;
	delete pPostProcessEffect;
	delete pKinect;
	//delete pScene;
	//delete camera;
}
void Renderer::draw()
{
	/*pBuffer->unbind();
	bindBackBuffer();
	glColor3f(1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pKinect->Update();*/
	//drawFullscreenQuad();
	//bindBackBuffer();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//pScene->pCamera->bind(pEffect);

	//glEnable(GL_LINE_SMOOTH);
	//glLineWidth(3);
	/*glBegin(GL_LINES);
	glVertex3f(-1, -1, 0);
	glVertex3f(1, 1, 0);
	//glVertex3f(-1, 1, 0);
	glEnd();*/
	//glColor3f(1, 1, 1);
	//drawFullscreenQuad();
	drawScene(pScene, pEffect, pBuffer);


	bindBackBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pPostProcessEffect->Bind();
	pPostProcessEffect->getParam("t2Texture") = pBuffer->getColor(0);
	drawFullscreenQuad();
	//pEffect->Bind();
	//pScene->draw(pEffect);
	//pBuffer->unbind();

	/*bindBackBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pPostProcessEffect->Bind();
	pPostProcessEffect->getParam("t2Texture") = pBuffer;
	drawFullscreenQuad();*/
	
	glutSwapBuffers();
}
void Renderer::init()
{
	if (_renderer)
		release();
	_renderer = new Renderer();
}
void Renderer::release()
{
	delete _renderer;
}
Renderer* Renderer::getRenderer()
{
	return _renderer;
}
void MyNamespace::Renderer::draw2DVertex(float x, float y)
{
	float tx = x > 0.0f ? 1.0f : 0.0f;
	float ty = y > 0.0f ? 1.0f : 0.0f;
	glTexCoord2f(tx, ty);
	glVertex2f(x, y);
}
void MyNamespace::Renderer::drawFullscreenQuad()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	draw2DVertex(-1, -1);
	draw2DVertex(-1, 1);
	draw2DVertex(1, 1);
	draw2DVertex(-1, -1);
	draw2DVertex(1, 1);
	draw2DVertex(1, -1);
	glEnd();
}

void MyNamespace::Renderer::bindBackBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MyNamespace::Renderer::drawScene(Scene* scene, Effect* effect, FrameBuffer *buffer)
{
	buffer->bind();
	glEnable(GL_FRONT_AND_BACK);
	effect->Bind();
	scene->draw(effect);
	buffer->unbind();
}

void MyNamespace::Renderer::drawLines(vector<vec3> vertices, vec3 color)
{
	glBegin(GL_LINES);
	glColor3f(color.x, color.y, color.z);
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vec3 vertex = vertices[i];
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}
