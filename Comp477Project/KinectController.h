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

	vector<Joint> mainBodyJoints;
	vector<quat> mainBodyOrientations;
	vector<vec3> mainBodyJointPositions;
	vector<vec3> getJointVertices();

private:
	vector<Joint> getJoints(IBody* pBody);
	vector<quat> getJointOrientations(IBody *pBody);
	vector<vec3> getJointPositions(IBody *pBody);
	void addJointLine(vector<vec3> &vertices, vector<Joint> &joints, JointType joint1, JointType joint2);
	
	IKinectSensor *m_pKinectSensor;
	ICoordinateMapper*      m_pCoordinateMapper;
	IBodyFrameReader*       m_pBodyFrameReader;
};

