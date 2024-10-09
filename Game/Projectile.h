#pragma once
#include <Component.h>

class Projectile : public Component {
public:
	Projectile(GameObject* gameObject);
	~Projectile();

	void update(float deltaTime) override;
	bool onCollision(GameObject* gameObject) override;

private:
	bool checkIfOutOfScreen();
};