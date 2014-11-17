#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject(string modelName)
{
	pModel = AssetManager::load<Model>(modelName);
	setScale(1);
}

GameObject::GameObject()
{
	setScale(1);
}


GameObject::~GameObject()
{
}
void GameObject::draw(Effect *effect)
{
	mat44 mScale = mat44::getScale(scale);
	mat44 mRotation = mat33(getRotation());
	mat44 mTranslation = mat44::getTranslation(getPosition());
	mat44 world = mScale * mRotation * mTranslation;
	effect->getParam("mWorld") = world;
	pModel->draw(effect);
}

void GameObject::setScale(vec3 &v)
{
	scale = v;
}

void GameObject::setScale(real s)
{
	scale = vec3(s, s, s);
}

void GameObject::loadFromDesc(ActorDesc &desc)
{
	Actor::loadFromDesc(desc);
	GameObjectDesc &goDesc = castDesc<GameObjectDesc>(desc);
	pModel = AssetManager::load<Model>(goDesc.modelName);
	setScale(goDesc.scale);
}

GameObjectDesc::GameObjectDesc()
{
}

GameObjectDesc::GameObjectDesc(string data)
	:ActorDesc(data)
{
	modelName = bindings.get("model");
	scale = bindings.getVec3("scale");
}
