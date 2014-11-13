#include "stdafx.h"
#include "KinectController.h"


KinectController::KinectController()
{
	m_pBodyFrameReader = NULL;
	m_pCoordinateMapper = NULL;
	m_pKinectSensor = NULL;
	init();
}


KinectController::~KinectController()
{
	if (m_pBodyFrameReader)
		m_pBodyFrameReader->Release();

	// done with coordinate mapper
	if (m_pCoordinateMapper)
		m_pCoordinateMapper->Release();

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
		m_pKinectSensor->Release();
	}
}

void KinectController::init()
{
	HRESULT hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IBodyFrameSource* pBodyFrameSource = NULL;

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		}
		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
		}
		if (pBodyFrameSource)
			pBodyFrameSource->Release();
		//SafeRelease(pBodyFrameSource);
	}
	if (!m_pKinectSensor || FAILED(hr))
	{
		cout << "No ready Kinect found!" << endl;
	}
}

void KinectController::Update()
{
	if (!m_pBodyFrameReader)
	{
		return;
	}

	IBodyFrame* pBodyFrame = NULL;

	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;

		hr = pBodyFrame->get_RelativeTime(&nTime);

		IBody* ppBodies[BODY_COUNT] = { 0 };

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr))
		{
			ProcessBody(nTime, BODY_COUNT, ppBodies);
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			if (ppBodies[i])
				ppBodies[i]->Release();
			//SafeRelease(ppBodies[i]);
		}
	}

	if (pBodyFrame)
		pBodyFrame->Release();
}

void KinectController::ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies)
{
	HRESULT hr;
	for (int i = 0; i < nBodyCount; i++)
	{
		IBody* pBody = ppBodies[i];
		if (pBody)
		{
			BOOLEAN isTracked = false;
			hr = pBody->get_IsTracked(&isTracked);
			if (SUCCEEDED(hr) && isTracked)
			{
				Joint pJoints[JointType_Count];
				vector<vec3> jointPoints;
				vector<Joint> joints;
				hr = pBody->GetJoints(_countof(pJoints), pJoints);
				//get joint points vector
				if (SUCCEEDED(hr))
				{
					for (int i = 0; i < _countof(pJoints); i++)
					{
						joints.push_back(pJoints[i]);
						CameraSpacePoint point = pJoints[i].Position;
						vec3 v(point.X, point.Y, point.Z);
						jointPoints.push_back(v);
					}
				}
				if (joints.size()>0 && joints[0].Position.X != 0)
					cout << "Joints found!" << endl;
				glBegin(GL_LINES);
				DrawJoints(pJoints, jointPoints);
				glEnd();
			}
		}
	}
}

void KinectController::DrawJoint(Joint* pJoints, vector<vec3> jointPoints, JointType joint0, JointType joint1)
{
	TrackingState joint0State = pJoints[joint0].TrackingState;
	TrackingState joint1State = pJoints[joint1].TrackingState;
	if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
	{
		vec3 point1, point2;
		point1 = jointPoints[joint0];
		point2 = jointPoints[joint1];
		glVertex3d(point1.x, point1.y, 0);
		glVertex3d(point2.x, point2.y, 0);
	}
}

void KinectController::DrawJoints(Joint* pJoints, vector<vec3> pJointPoints)
{
	// Draw the bones

	// Torso
	DrawJoint(pJoints, pJointPoints, JointType_Head, JointType_Neck);
	DrawJoint(pJoints, pJointPoints, JointType_Neck, JointType_SpineShoulder);
	DrawJoint(pJoints, pJointPoints, JointType_SpineShoulder, JointType_SpineMid);
	DrawJoint(pJoints, pJointPoints, JointType_SpineMid, JointType_SpineBase);
	DrawJoint(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderRight);
	DrawJoint(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderLeft);
	DrawJoint(pJoints, pJointPoints, JointType_SpineBase, JointType_HipRight);
	DrawJoint(pJoints, pJointPoints, JointType_SpineBase, JointType_HipLeft);

	// Right Arm    
	DrawJoint(pJoints, pJointPoints, JointType_ShoulderRight, JointType_ElbowRight);
	DrawJoint(pJoints, pJointPoints, JointType_ElbowRight, JointType_WristRight);
	DrawJoint(pJoints, pJointPoints, JointType_WristRight, JointType_HandRight);
	DrawJoint(pJoints, pJointPoints, JointType_HandRight, JointType_HandTipRight);
	DrawJoint(pJoints, pJointPoints, JointType_WristRight, JointType_ThumbRight);

	// Left Arm
	DrawJoint(pJoints, pJointPoints, JointType_ShoulderLeft, JointType_ElbowLeft);
	DrawJoint(pJoints, pJointPoints, JointType_ElbowLeft, JointType_WristLeft);
	DrawJoint(pJoints, pJointPoints, JointType_WristLeft, JointType_HandLeft);
	DrawJoint(pJoints, pJointPoints, JointType_HandLeft, JointType_HandTipLeft);
	DrawJoint(pJoints, pJointPoints, JointType_WristLeft, JointType_ThumbLeft);

	// Right Leg
	DrawJoint(pJoints, pJointPoints, JointType_HipRight, JointType_KneeRight);
	DrawJoint(pJoints, pJointPoints, JointType_KneeRight, JointType_AnkleRight);
	DrawJoint(pJoints, pJointPoints, JointType_AnkleRight, JointType_FootRight);

	// Left Leg
	DrawJoint(pJoints, pJointPoints, JointType_HipLeft, JointType_KneeLeft);
	DrawJoint(pJoints, pJointPoints, JointType_KneeLeft, JointType_AnkleLeft);
	DrawJoint(pJoints, pJointPoints, JointType_AnkleLeft, JointType_FootLeft);
}
