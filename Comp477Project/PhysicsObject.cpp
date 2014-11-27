#include "stdafx.h"
#include "PhysicsObject.h"


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
		boxDesc.dimensions = (pMesh->maxB - pMesh->minB) * scale;
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
