#include "stdafx.h"
#include "SkinnedModel.h"
#include "AssetManager.h"
#include <Kinect.h>


SkinnedModel::SkinnedModel()
{
	iWeights1VBO = iWeights2VBO = iWeights3VBO = iWeights4VBO = iWeights5VBO = iWeights6VBO = 0;
}


SkinnedModel::~SkinnedModel()
{
	Mesh::deleteVBO(iWeights1VBO);
	Mesh::deleteVBO(iWeights2VBO);
	Mesh::deleteVBO(iWeights3VBO);
	Mesh::deleteVBO(iWeights4VBO);
	Mesh::deleteVBO(iWeights5VBO);
	Mesh::deleteVBO(iWeights6VBO);
}

void SkinnedModel::loadFromFile(string fileName)
{
	Model::loadFromFile(fileName);
	string classPath = AssetManager::getClassPath<SkinnedModel>(*this);
	ConfigScanner sc(fileName);
	string skeletonName = classPath + "\\" + sc.params.get("Skeleton");
	string weightsName = classPath + "\\" + sc.params.get("Weights");
	loadJoints(skeletonName);
	loadWeights(weightsName);
}
void SkinnedModel::loadJoints(string fileName)
{
	Scanner sc(fileName);
	sc.getPrimitive<unsigned int>();
	while (sc.hasData())
	{
		MyJoint joint;
		joint.pos = sc.getVec3();
		joint.parentJointId = sc.getPrimitive<int>();
		joints.push_back(joint);
		sc.getPrimitive<unsigned int>();
		//sc.skipLine();
	}
	/*for (unsigned int i = 0; i < joints.size(); i++)
	{
		joints[i].rotation.id();
	}*/
	//extendJoint(14, 1.3f);
}

void SkinnedModel::loadWeights(string fileName)
{
	Scanner sc(fileName);
	while (sc.hasData())
	{
		//for (unsigned int i = 0; i < joints.size(); i++)
		weights.push_back(sc.getPrimitive<float>());
		//sc.skipLine();
	}
	vector<float> aweights[6];
	for (unsigned int i = 0; i < weights.size(); i++)
	{
		int awId = (i % 17) / 3;
		aweights[awId].push_back(weights[i]);

	}
	Mesh::createVBO<float>(aweights[0], &iWeights1VBO);
	Mesh::createVBO<float>(aweights[1], &iWeights2VBO);
	Mesh::createVBO<float>(aweights[2], &iWeights3VBO);
	Mesh::createVBO<float>(aweights[3], &iWeights4VBO);
	Mesh::createVBO<float>(aweights[4], &iWeights5VBO);
	Mesh::createVBO<float>(aweights[5], &iWeights6VBO);
}

void SkinnedModel::draw(Effect* effect)
{
	/*for (int i = 0; i < joints.size()-2; i+=2)
	{
		joints[i].rotation.setWXYZ(0.5, 0.1, 0.3, 0.1);
		joints[i].rotation.normalize();
	}*/
	//joints[2].rotation.setWXYZ(0.5, 0, 0, 0.3);
	//joints[2].rotation.normalize();
	//joints[7].rotation.setWXYZ(0.7, 0.8, 0.3, 0.2);
	Mesh::bindVBO(iWeights1VBO, effect->getAttribute("weights_1"), 3);
	Mesh::bindVBO(iWeights2VBO, effect->getAttribute("weights_2"), 3);
	Mesh::bindVBO(iWeights3VBO, effect->getAttribute("weights_3"), 3);
	Mesh::bindVBO(iWeights4VBO, effect->getAttribute("weights_4"), 3);
	Mesh::bindVBO(iWeights5VBO, effect->getAttribute("weights_5"), 3);
	Mesh::bindVBO(iWeights6VBO, effect->getAttribute("weights_6"), 2);
	vector<mat44> transforms;
	
	for (unsigned int i = 1; i < joints.size(); i++)
	{
		//string uniformName = "maBoneTransforms[" + to_string(i*4) + "]";
		//effect->getParam(uniformName) = joints[i].transform;
		
		transforms.push_back(joints[i].transform);
	}
	effect->getParam("maBoneTransforms[0]") = transforms;
	Model::draw(effect);
}

mat44 SkinnedModel::getTransform(int jointId)
{
	mat44 result;
	MyJoint joint = joints[jointId];
	if (joint.parentJointId == -1)
		result = mat44::getTranslation(-joint.pos);
	else
	{
		MyJoint parentJoint = joints[joint.parentJointId];
		//
		result = mat44();
	}
	return result;
}

void SkinnedModel::updateTransforms()
{
	for (unsigned int i = 0; i < joints.size(); i++)
	{
		joints[i].transform = Mat44::getIdentity();
	}
	vector<MyJoint*> perJoints;
	perJoints.push_back(&joints[2]);
	perJoints.push_back(&joints[1]);
	perJoints.push_back(&joints[0]);
	for (unsigned int i = 3; i < joints.size(); i++)
		perJoints.push_back(&joints[i]);
	for (unsigned int i = 0; i < perJoints.size(); i++)
	{
		MyJoint *joint = perJoints[i];
		if (joint->parentJointId != -1)
		{
			MyJoint *parent = &joints[joint->parentJointId];
			mat44 parentRot = parent->transform;
			//vector<MyJoint*> children = getChildren(i);
			//for (unsigned int j = 0; j < children.size(); j++)
			//{
			//quat invParentRot = parent->rotation;
			//invParentRot.conjugate();
			//mat44 childRot = mat33(joint->rotation);// *mat33(invParentRot);
			//###############FROM JOINT POSITIONS###################
			vec3 kinectDir = joint->kinectPos - parent->kinectPos;
			vec3 restDir = (parentRot * vec4(joint->pos, 1) 
				- parentRot * vec4(parent->pos, 1))
				.toVec3();
			mat44 childRot = mat33(getRotation(restDir, kinectDir));

			//###############END####################################
			//correctRotation(childRot, i);
			vec3 translation = -parent->pos;
			mat44 transformation = mat44::getTranslation(translation) * parent->transform;
			transformation = childRot * transformation;
			transformation = mat44::getTranslation(-translation) * transformation;//parentRot * mat44::getTranslation(translation) * childRot * mat44::getTranslation(-translation) * parent->transform;
			joint->transform = transformation;
			//}
		}
	}
}
vector<vec3> SkinnedModel::convertVectors(vector<vec3> &a)
{
	vector<vec3> newA;
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
vector<MyJoint*> SkinnedModel::getChildren(unsigned int cId)
{
	vector<MyJoint*> children;
	for (unsigned int i = 0; i < joints.size(); i++)
	{
		if (joints[i].parentJointId == cId)
			children.push_back(&joints[i]);
	}
	return children;
}
vec3 SkinnedModel::getJointPos(int jointId)
{
	MyJoint &joint = joints[jointId];
	mat44 transform = joint.transform;
	vec4 v = transform * vec4(joint.pos, 1);
	return v.toVec3();
}
/*void SkinnedModel::loadRotations(vector<quat> &rotations)
{
	if (rotations.size()>0)
	{
		vector<quat> newRots = convertArray<quat>(rotations);
		for (unsigned int i = 0; i < newRots.size(); i++)
		{
			MyJoint &joint = joints[i];
			joint.rotation = newRots[i];
			joint.rotation.normalize();
			/*vec3 pos, parentPos;
			pos = joint.pos;
			if (joints[i].parentJointId != -1)
			{
				parentPos = joints[joint.parentJointId].pos;
				vec3 diff = pos - parentPos;
				diff.normalize();
				vec3 norm = vec3(0, 1, 0).cross(diff);
				float angle = acos(norm.magnitude());
				quat correction;
				correction.fromAngleAxis(angle, norm);
				//joint.rotation *= correction;
			}
		}
	}
	/*else
	{
		joints[15].rotation.setXYZW(0.5f, 0, 0.3f, 0.7f);
		joints[15].rotation.normalize();
	}
	updateTransforms();
}*/

/*vector<quat> SkinnedModel::convertSkeleton(vector<quat> rots)
{
	vector<quat> newRots = convertArray<quat>(rots);
	return newRots;
}*/

/*void SkinnedModel::correctRotation(mat44 &rot, int id)
{
	quat tQuat;
	switch (id)
	{
	case 4:
	case 8:
	case 12:
	case 15:
		rot *= mat44::fromAxii(vec3(0, 0, 1), vec3(0, -1, 0), vec3(1, 0, 0));
		break;
	}
}*/

void SkinnedModel::loadKinectPositions(vector<vec3> &positions)
{
	vector<vec3> newPositions = convertVectors(positions);
	if (positions.size()>0)
		for (unsigned int i = 0; i < joints.size(); i++)
			joints[i].kinectPos = newPositions[i];
	updateTransforms();
}

quat SkinnedModel::getRotation(vec3 restDir, vec3 kinectDir)
{
	kinectDir.normalize();
	restDir.normalize();
	vec3 normal = kinectDir.cross(restDir);
	float dotP = kinectDir.dot(restDir);
	float angle;
	//float angleFactor = 20;
	if (dotP > 0)
	{
		float sinA = normal.magnitude();
		//float cosA = kinectDir.dot(restDir);
		angle = asin(sinA) * 180 / NxPi; //* angleFactor;
	}
	else
	{
		vec3 binormal = kinectDir.cross(normal);
		binormal.normalize();
		float cosA = binormal.dot(restDir);
		angle = 90 + acos(cosA) * 180 / NxPi;
	}
	quat newRot;
	newRot.fromAngleAxis(angle, normal);
	return newRot;
}

vec3 SkinnedModel::getPelvisPos()
{
	vec3 r = joints[2].kinectPos;
	r.x = -r.x;
	r.z *= 2;
	return r;
}
/*vec3 SkinnedModel::getLeftPos()
{
	vec3 wrist = getJointPos(14);
	wrist.x = -wrist.x;
	vec3 pelvis = getPelvisPos();
	vec3 pos = wrist + pelvis;
	return pos;
}*/
vec3 SkinnedModel::getCorrectedPos(int jid)
{
	vec3 pos = getJointPos(jid);
	pos.x = -pos.x;
	vec3 pelvis = getPelvisPos();
	return pos + pelvis;
}
/*vec3 SkinnedModel::getRightPos()
{
	vec3 wrist = getJointPos(17);
	wrist.x = -wrist.x;
	
}*/
void SkinnedModel::extendJoint(int jointId, float factor)
{
	MyJoint &joint = joints[jointId];
	MyJoint parent = joints[joint.parentJointId];
	vec3 diff = joint.pos - parent.pos;
	joint.pos = parent.pos + diff*factor;
}
