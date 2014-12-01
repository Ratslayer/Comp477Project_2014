#include "stdafx.h"
#include "PhysicsObject.h"

PhysicsDesc::PhysicsDesc()
{
	name = "";
	scale = vec3(1, 1, 1);
	this->type = NxShapeType::NX_SHAPE_MESH;
	this->velocity = vec3(0, 0, 0);
	this->modelName = "";
	position = vec3(0, 0, 0);
	rotation.id();
}
PhysicsObject::PhysicsObject()
{
}


PhysicsObject::~PhysicsObject()
{
}
void PhysicsObject::loadFromDesc(ActorDesc &desc)
{
	GameObject::loadFromDesc(desc);
	PhysicsDesc &goDesc = castDesc<PhysicsDesc>(desc);
	NxActorDesc actorDesc;
	NxPlaneShapeDesc planeDesc;
	NxBoxShapeDesc boxDesc;
	NxSphereShapeDesc sphereDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping = 0.75f;
	bodyDesc.linearVelocity = NxVec3(0, 1, 0);
	actorDesc.globalPose.t = goDesc.position;
	actorDesc.globalPose.M = goDesc.rotation;
	Mesh* pMesh = pModel->pMesh;
	switch (goDesc.type)
	{
	case NX_SHAPE_BOX:
		actorDesc.body = &bodyDesc;
		actorDesc.density = 1.0f;
		boxDesc.localPose.t = pMesh->center;
		boxDesc.dimensions = (pMesh->maxB - pMesh->minB) * scale / 2;
		//boxDesc.dimensions.z = boxDesc.dimensions.z - 0.5f * scale.z;
		actorDesc.shapes.pushBack(&boxDesc);
		break;
	case NX_SHAPE_SPHERE:
		actorDesc.body = &bodyDesc;
		actorDesc.density = 1.0f;
		sphereDesc.localPose.t = pMesh->center;
		sphereDesc.radius = pMesh->radius * scale.x;
		actorDesc.shapes.pushBack(&sphereDesc);
		break;
	case NX_SHAPE_PLANE:
		actorDesc.shapes.pushBack(&planeDesc);
		break;
	default:
		break;
	}
	actor = g_physXScene->createActor(actorDesc);
	actor->wakeUp();
	return;
}

vec3 PhysicsObject::getPosition()
{
	NxShape* const* shapes = actor->getShapes();
	int n = actor->getNbShapes();
	vec3 pos = actor->getGlobalPosition();
	if (n > 0)
		pos = shapes[0]->getGlobalPosition();
	//actor->
	return pos;
}

quat PhysicsObject::getRotation()
{
	quat q = actor->getGlobalOrientationQuat();
	q.invert();
	return q;
}

PhysicsDesc::PhysicsDesc(string data)
	:GameObjectDesc(data)
{
	string typeStr = bindings.get("Shape");
	if (Scanner::lcEquals(typeStr, "box"))
		type = NX_SHAPE_BOX;
	else if (Scanner::lcEquals(typeStr, "sphere"))
		type = NX_SHAPE_SPHERE;
	else if (Scanner::lcEquals(typeStr, "plane"))
		type = NX_SHAPE_PLANE;
}
