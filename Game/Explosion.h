#pragma once
#include <Component.h>

class Explosion : public Component {
public:
	Explosion(GameObject* gameObject);
	~Explosion();

	void update(float deltaTime) override;
private:
	float lifeTime = 0.5f;
	float currentLifeTime = 0.f;

	glm::vec2 initalScale = glm::vec2(0.f, 0.f);
};