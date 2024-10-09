#pragma once

#include "Component.h"

class AsteroidComponent : public Component {
public:
	AsteroidComponent(GameObject* gameObject);
	~AsteroidComponent();

	void update(float deltaTime) override;

	bool onCollision(GameObject* gameObject) override;

private:
	bool checkIfOutOfScreen();
};