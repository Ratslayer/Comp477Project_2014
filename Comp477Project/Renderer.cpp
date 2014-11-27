#include "stdafx.h"
#include "Renderer.h"
#include "Window.h"
#include "ColorDepthBuffer.h"
using namespace MyNamespace;
Renderer* Renderer::_renderer = NULL;
Renderer::Renderer()
{
	pEffect = new Effect("normalTexture.txt", "phongTexture.txt");
	pSkinEffect = new Effect("skinned.txt", "color.txt");
	pPostProcessEffect = new Effect("directTexture.txt", "colorTexture.txt");
	//pHero = AssetManager::load<SkinnedModel>("hero.txt");
	pKinect = new KinectController();
	pScene = AssetManager::load<Scene>("scene.txt");
	FBODesc desc;
	desc.width = 1024;
	desc.height = 768;
	pBuffer = new ColorDepthBuffer(1024, 768);
	pBuffer->unbind();
}
Renderer::~Renderer()
{
	delete pEffect;
	delete pBuffer;
	delete pPostProcessEffect;
	delete pSkinEffect;
	delete pKinect;
	delete pHero;
	//delete pScene;
	//delete camera;
}
void Renderer::draw()
{
	g_physXScene->simulate(g_timeStep);
	g_physXScene->fetchResults(NxSimulationStatus::NX_ALL_FINISHED);
	pBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene(pScene, pEffect, pBuffer);

	/*pSkinEffect->Bind();
	quat rot;
	rot.setXYZW(1, 0, 0, 0); //mat44::getScale(30) * mat44(mat33(rot))*mat44::getTranslation(vec3(20, 10 ,15));
	pHero->loadRotations(pKinect->mainBodyOrientations);
	pHero->loadKinectPositions(pKinect->mainBodyJointPositions);
	vec3 pos = pHero->getPelvisPos();// +vec3(10, 0);
	pSkinEffect->getParam("mWorld") = mat44(mat33(rot))* mat44::getTranslation(pos);
	pScene->pCamera->setPosition(pos + vec3(0, 1, 2));
	pScene->pCamera->lookAt(pos);
	pScene->pCamera->bind(pSkinEffect);
	pKinect->Update();
	pHero->draw(pSkinEffect);*/

	pBuffer->unbind();
	bindBackBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pPostProcessEffect->Bind();
	pPostProcessEffect->getParam("t2Texture") = pBuffer->getColor(0);
	drawFullscreenQuad();	
	//kinect skeleton rendering
	/*bindBackBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pKinect->Update();
	vector<vec3> vertices = pKinect->getJointVertices();
	drawLines(vertices, vec3(1, 1, 1));*/
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
