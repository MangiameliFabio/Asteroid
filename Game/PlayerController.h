#pragma once

#include "Component.h"

enum InputType {
	LEFT,
	RIGHT,
	BOOST,
	FIRE,
	IDLE
};

class PlayerController : public Component{

public:
	PlayerController(GameObject* gameObject);
	~PlayerController();

	void update(float deltaTime) override;
	bool onKey(SDL_Event& event) override;
	bool onCollision(GameObject* gameObject) override;

private:
	void addToBuffer(const InputType input);
	void removeFromBuffer(const InputType input);
	void handleInputBuffer();
	void checkIfOutsideScreen();
	void fireProjectile();
	glm::vec2 getAimDir();

	std::vector<InputType> inputBuffer;

	InputType rotationState = InputType::IDLE;
	InputType boostState = InputType::IDLE;
	InputType fireState = InputType::IDLE;

	float attackSpeed = 0.15f;
	float attackCoolDown = 0.f;
};