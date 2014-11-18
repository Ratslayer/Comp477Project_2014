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
			/*mainBody = getFirstBody(ppBodies, _countof(ppBodies));
			if (hasBody())
				mainBodyJoints = getJoints();*/
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
				mainBodyJoints = getJoints(pBody);
				mainBodyOrientations = getJointOrientations(pBody);
				/*Joint pJoints[JointType_Count];
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
				glColor3f(1, 1, 1);
				DrawJoints(pJoints, jointPoints);
				glEnd();*/
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

vector<Joint> KinectController::getJoints(IBody *mainBody)
{
	vector<Joint> joints;
	Joint pJoints[JointType_Count];
	BOOLEAN isTracked = false;
	HRESULT hr = mainBody->get_IsTracked(&isTracked);
	if (SUCCEEDED(hr) && isTracked)
	{
		hr = mainBody->GetJoints(_countof(pJoints), pJoints);
		if (SUCCEEDED(hr))
		{
			for (unsigned int i = 0; i < _countof(pJoints); i++)
			{
				joints.push_back(pJoints[i]);
			}
		}
	}
	return joints;
}

vector<vec3> KinectController::getJointVertices()
{
	vector<Joint> joints = mainBodyJoints;
	vector<vec3> vertices;
	if (joints.size()>1)
	{
		addJointLine(vertices, joints, JointType_Head, JointType_Neck);
		addJointLine(vertices, joints, JointType_Neck, JointType_SpineShoulder);
		addJointLine(vertices, joints, JointType_SpineShoulder, JointType_SpineMid);
		addJointLine(vertices, joints, JointType_SpineMid, JointType_SpineBase);
		addJointLine(vertices, joints, JointType_SpineShoulder, JointType_ShoulderRight);
		addJointLine(vertices, joints, JointType_SpineShoulder, JointType_ShoulderLeft);
		addJointLine(vertices, joints, JointType_SpineBase, JointType_HipRight);
		addJointLine(vertices, joints, JointType_SpineBase, JointType_HipLeft);

		// Right Arm    
		addJointLine(vertices, joints, JointType_ShoulderRight, JointType_ElbowRight);
		addJointLine(vertices, joints, JointType_ElbowRight, JointType_WristRight);
		addJointLine(vertices, joints, JointType_WristRight, JointType_HandRight);
		addJointLine(vertices, joints, JointType_HandRight, JointType_HandTipRight);
		addJointLine(vertices, joints, JointType_WristRight, JointType_ThumbRight);

		// Left Arm
		addJointLine(vertices, joints, JointType_ShoulderLeft, JointType_ElbowLeft);
		addJointLine(vertices, joints, JointType_ElbowLeft, JointType_WristLeft);
		addJointLine(vertices, joints, JointType_WristLeft, JointType_HandLeft);
		addJointLine(vertices, joints, JointType_HandLeft, JointType_HandTipLeft);
		addJointLine(vertices, joints, JointType_WristLeft, JointType_ThumbLeft);

		// Right Leg
		addJointLine(vertices, joints, JointType_HipRight, JointType_KneeRight);
		addJointLine(vertices, joints, JointType_KneeRight, JointType_AnkleRight);
		addJointLine(vertices, joints, JointType_AnkleRight, JointType_FootRight);
	}
	return vertices;
}

void KinectController::addJointLine(vector<vec3> &vertices, vector<Joint> &joints, JointType joint1, JointType joint2)
{
	Joint j1 = joints[joint1];
	Joint j2 = joints[joint2];
	CameraSpacePoint p1 = j1.Position, p2 = j2.Position;
	vertices.push_back(vec3(p1.X, p1.Y, 0));
	vertices.push_back(vec3(p2.X, p2.Y, 0));
}
vector<quat> KinectController::getJointOrientations(IBody *mainBody)
{
	vector<quat> quats;
	JointOrientation jointOrientations[JointType_Count];
	HRESULT hr = mainBody->GetJointOrientations(_countof(jointOrientations), jointOrientations);
	if (SUCCEEDED(hr))
	{
		for (unsigned int i = 0; i < _countof(jointOrientations); i++)
		{
			Vector4 vOrient = jointOrientations[i].Orientation;
			quat orientation;
			orientation.setXYZW(vOrient.x, vOrient.y, vOrient.z, vOrient.w);
			quats.push_back(orientation);
		}
	}
	return quats;
}

/*IBody* KinectController::getFirstBody(IBody** ppBodies, int numBodies)
{
	IBody *body = NULL;
	for (int i = 0; i < numBodies; i++)
	{
		IBody *pBody = ppBodies[i];
		if (pBody != NULL)
		{
			BOOLEAN isTracked = false;
			HRESULT hr = pBody->get_IsTracked(&isTracked);
			if (SUCCEEDED(hr) && isTracked)
			{
				body = pBody;
				break;
			}
		}
	}
	return body;
}

bool KinectController::hasBody()
{
	return mainBody != NULL;
}*/
