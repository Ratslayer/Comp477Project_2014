#pragma once
#include "stdafx.h"
#include <Windows.h>
#include <Ole2.h>
#include <Kinect.h>
class KinectController
{
public:
	KinectController();
	~KinectController();
	void init();
	void Update();
	void ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies);
	void DrawJoints(Joint* pJoints, vector<vec3> jointPoints);
	void DrawJoint(Joint* pJoints, vector<vec3> jointPoints, JointType joint1, JointType joint2);

private:
	IKinectSensor *m_pKinectSensor;
	ICoordinateMapper*      m_pCoordinateMapper;
	IBodyFrameReader*       m_pBodyFrameReader;
	//void SafeRelease(
};

