#include "PhysicsComponent.h"

#include "GameObject.h"
#include "glm/gtc/epsilon.hpp"
#include <iostream>
#include <Globals.h>

PhysicsComponent::PhysicsComponent(GameObject* gameObject) : Component(gameObject) {};

PhysicsComponent::~PhysicsComponent(){}

void PhysicsComponent::initialize(picojson::value& serializedData)
{
	Component::initialize(serializedData);

	rotationDrag = serializedData.get("rotationDrag").get<double>();
	velocityDrag = serializedData.get("velocityDrag").get<double>();
	mass = serializedData.get("mass").get<double>();
}


void PhysicsComponent::update(float deltaTime)
{
	rotateGameObject(deltaTime);
	moveGameObject(deltaTime);
}

void PhysicsComponent::setVelocityDrag(float newDrag)
{
	PhysicsComponent::velocityDrag = newDrag;
}

float PhysicsComponent::getVelocityDrag()
{
	return velocityDrag;
}

void PhysicsComponent::setRotationDrag(float newDrag)
{
	PhysicsComponent::rotationDrag = newDrag;
}

float PhysicsComponent::getRotationDrag()
{
	return rotationDrag;
}

void PhysicsComponent::setMass(float mass)
{
	PhysicsComponent::mass = mass;
}

float PhysicsComponent::getMass()
{
	return mass;
}

void PhysicsComponent::setCollisionCircle(float radius)
{
	collisionCircleRadius = radius;
}

float PhysicsComponent::getCollisionCircle()
{
	return collisionCircleRadius;
}

void PhysicsComponent::moveGameObject(float deltaTime)
{
	GameObject* gameObject = getGameObject();
	const glm::vec2 pos = gameObject->getPosition();
	const glm::vec2 vel = gameObject->getVelocity();

	glm::vec2 dragVelDir = vel * -1.f;
	glm::normalize(dragVelDir);

	
	glm::vec2 newVel = vel + dragVelDir * deltaTime * velocityDrag;
	if (newVel.length() <= 0.5f) {
		newVel = { 0.f, 0.f };
	};
	gameObject->setVelocity(newVel);
	gameObject->setPosition(pos + newVel);
}

void PhysicsComponent::rotateGameObject(float deltaTime)
{
	GameObject* gameObject = getGameObject();
	const float rot = gameObject->getRotation();
	const float rotSpeed = gameObject->getRotationSpeed();

	float newRotSpeed;

	if (glm::epsilonEqual(rotSpeed, 0.f, 0.1f)) {
		newRotSpeed = 0.f;
	}
	else {
		newRotSpeed = rotSpeed - rotSpeed * rotationDrag * deltaTime;
	}
	gameObject->setRotationSpeed(newRotSpeed);
	
	gameObject->setRotation(rot + newRotSpeed);
}

