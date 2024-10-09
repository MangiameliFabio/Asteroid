#include "Explosion.h"
#include "GameObject.h"
#include <Globals.h>

Explosion::Explosion(GameObject* gameObject) : Component(gameObject) {
	initalScale = gameObject->getScale();
}

Explosion::~Explosion()
{
}

void Explosion::update(float deltaTime)
{
	if (currentLifeTime >= lifeTime) {
		getGameObject()->markedForDeletion = true;
	}

	glm::vec2 newScale = initalScale * (glm::abs(lifeTime - currentLifeTime) / (lifeTime / 100) / 100);

	getGameObject()->setScale(newScale);

	currentLifeTime += deltaTime;
}
