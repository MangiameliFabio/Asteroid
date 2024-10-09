#pragma once

#include "Component.h"
#include <functional>


class PhysicsComponent : public Component {
public:
	PhysicsComponent(GameObject* gameObject);
	~PhysicsComponent();

	void initialize(picojson::value& serializedData) override;

	void update(float deltaTime) override;

	void setVelocityDrag(float newDrag);
	float getVelocityDrag();

	void setRotationDrag(float newDrag);
	float getRotationDrag();

	void setMass(float mass);
	float getMass();

	void setCollisionCircle(float radius);
	float getCollisionCircle();

	std::function<bool(GameObject*)> onOverlap = nullptr;

private:
	void moveGameObject(float deltaTime);
	void rotateGameObject(float deltaTime);

	float velocityDrag = 2.f;
	float rotationDrag = 10.f;

	float mass = 1.0f;
	float collisionCircleRadius = 0.f;
};

