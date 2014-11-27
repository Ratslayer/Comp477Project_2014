#pragma once
#include "Model.h"
class MyJoint
{
public:
	vec3 pos;
	vec3 kinectPos;
	quat rotation;
	mat44 transform;
	int parentJointId;
};
class SkinnedModel :
	public Model
{
public:
	SkinnedModel();
	virtual ~SkinnedModel();
	void loadFromFile(string fileName);
	string getClassName(){ return "SkinnedModel"; }
	void loadRotations(vector<quat> &rotations);
	void loadKinectPositions(vector<vec3> &positions);
	vec3 getPelvisPos();
	void draw(Effect* effect);
private:
	template<typename T>
	vector<T> convertArray(vector<T> &a)
	{
		vector<T> newA;
		if (a.size() > 0)
		{
			newA.push_back(a[JointType_SpineShoulder]);
			newA.push_back(a[JointType_SpineMid]);
			newA.push_back(a[JointType_SpineBase]);
			newA.push_back(a[JointType_Neck]);
			//id: 4
			newA.push_back(a[JointType_HipLeft]);
			newA.push_back(a[JointType_KneeLeft]);
			newA.push_back(a[JointType_AnkleLeft]);
			newA.push_back(a[JointType_FootLeft]);
			//id: 8
			newA.push_back(a[JointType_HipRight]);
			newA.push_back(a[JointType_KneeRight]);
			newA.push_back(a[JointType_AnkleRight]);
			newA.push_back(a[JointType_FootRight]);
			//id: 12
			newA.push_back(a[JointType_ShoulderLeft]);
			newA.push_back(a[JointType_ElbowLeft]);
			newA.push_back(a[JointType_WristLeft]);
			//id: 15
			newA.push_back(a[JointType_ShoulderRight]);
			newA.push_back(a[JointType_ElbowRight]);
			newA.push_back(a[JointType_WristRight]);
			
			
		}
		return newA;
	}
	vector<float> weights;
	vector<MyJoint> joints;
	GLuint iWeights1VBO, iWeights2VBO, iWeights3VBO, iWeights4VBO, iWeights5VBO, iWeights6VBO;
	void loadWeights(string fileName);
	void loadJoints(string fileName);
	mat44 getTransform(int jointId);
	quat getRotation(vec3 restDir, vec3 kinectDir);
	void updateTransforms();
	vector<quat> convertSkeleton(vector<quat> rots);
	vector<MyJoint*> getChildren(unsigned int cId);
	void correctRotation(mat44 &rot, int id);
	//unsigned int getRootJointId();

};

