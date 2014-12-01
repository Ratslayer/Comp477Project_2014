#include "stdafx.h"
#include "Renderer.h"
#include "Window.h"
#include "ColorDepthBuffer.h"
using namespace MyNamespace;
Renderer* Renderer::_renderer = NULL;
Renderer::Renderer()
{
	pEffect = new Effect("vertex1.txt", "colorTexture.txt");
	pSkinEffect = new Effect("skinned.txt", "color.txt");
	pPostProcessEffect = new Effect("directTexture.txt", "colorTexture.txt");
	pHero = AssetManager::load<SkinnedModel>("hero.txt");
	pKinect = new KinectController();
	pScene = AssetManager::load<Scene>("scene.txt");

	pLeft = pScene->getActor<PhysicsObject>("lefthand");
	pRight = pScene->getActor<PhysicsObject>("righthand");
	pRightFoot = pScene->getActor<PhysicsObject>("rightfoot");
	pLeftFoot = pScene->getActor<PhysicsObject>("lefthand");
	pLeft->bVisible = pRight->bVisible = pRightFoot->bVisible = pLeftFoot->bVisible = false;

	PhysicsDesc pDesc;
	pDesc.modelName = "box.txt";
	pDesc.scale = vec3(0.15f, 0.15f, 0.15f);
	pDesc.type = NxShapeType::NX_SHAPE_BOX;
	vec3 pyramidPos(0, 0, -3);
	float distance = 0;
	int iBase = 5;
	int iHeight = 0;
	vector<vec3> positions;
	vec3 offsetX = vec3(pDesc.scale.x + distance, 0, 0);
	vec3 offsetZ = vec3(0, 0, pDesc.scale.x + distance);
	while (iBase > 0)
	{
		vec3 offsetY = vec3(0, (pDesc.scale.y + distance)*(float)iHeight, 0);
		vec3 initPos = pyramidPos + offsetY - (float)iBase / 2.0f *offsetX;
		for (int i = 0; i < iBase; i++)
		{
			for (int j = 0; j < iBase; j++)
			{
				vec3 pos = initPos + (float)i*offsetX + (float)j*offsetZ;
				positions.push_back(pos);
			}
		}
		iHeight++;
		iBase--;
	}
	pScene->multiplyObjects(pDesc, positions);
	pLeft->actor->raiseBodyFlag(NxBodyFlag::NX_BF_KINEMATIC);
	pRight->actor->raiseBodyFlag(NxBodyFlag::NX_BF_KINEMATIC);
	pLeftFoot->actor->raiseBodyFlag(NxBodyFlag::NX_BF_KINEMATIC);
	pRightFoot->actor->raiseBodyFlag(NxBodyFlag::NX_BF_KINEMATIC);
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

	pKinect->Update();
	vec3 pos = vec3();
	if (pKinect->mainBodyJointPositions.size() > 0)
	{
		pHero->loadKinectPositions(pKinect->mainBodyJointPositions);
		pos = pHero->getPelvisPos();
		//vec3 leftPos = pHero->getLeftPos();
		//vec3 rightPos = pHero->getRightPos();
		//float lMag = leftPos.magnitude(), rMag = rightPos.magnitude();
		pLeft->actor->moveGlobalPosition(pHero->getCorrectedPos(14));
		pRight->actor->moveGlobalPosition(pHero->getCorrectedPos(17));
		pLeftFoot->actor->moveGlobalPosition(pHero->getCorrectedPos(7));
		pRightFoot->actor->moveGlobalPosition(pHero->getCorrectedPos(11));
	}
	pBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene(pScene, pEffect, pBuffer);

	pSkinEffect->Bind();
	quat rot;
	rot.setXYZW(1, 0, 0, 0); //mat44::getScale(30) * mat44(mat33(rot))*mat44::getTranslation(vec3(20, 10 ,15));
	//pHero->loadRotations(pKinect->mainBodyOrientations);
	// +vec3(10, 0);
	pSkinEffect->getParam("mWorld") = mat44(mat33(rot))* mat44::getTranslation(pos);
	pScene->pCamera->setPosition(pos + vec3(1.5, 0.5, 0));
	pScene->pCamera->lookAt(pos);
	vec3 up = pHero->getCorrectedPos(3) - pHero->getCorrectedPos(0);
	//pScene->pCamera->setUp(up);
	pScene->pCamera->bind(pSkinEffect);
	
	pHero->draw(pSkinEffect);

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
