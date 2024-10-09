#include "Physics.h"
#include "Globals.h"
#include "PhysicsComponent.h"
#include "GameObject.h"

void Physics::updatePhysics()
{
	for (int i = 0; i < GLOBALS->sceneObjects.size() - 1; i++) {
		auto gameObjA = GLOBALS->sceneObjects[i];
		auto physCompA = gameObjA->getComponent<PhysicsComponent>();
		if (physCompA == nullptr) {
			continue;
		}

		for (int j = i + 1; j < GLOBALS->sceneObjects.size(); j++) {
			auto gameObjB = GLOBALS->sceneObjects[j];
			auto physCompB = gameObjB->getComponent<PhysicsComponent>();
			if (physCompB == nullptr) {
				continue;
			}

			glm::vec2 normal;
			float depth;
			if (checkForIntersection(gameObjA->getPosition(), physCompA->getCollisionCircle(),
				gameObjB->getPosition(), physCompB->getCollisionCircle(), normal, depth)) {

				bool consumed = false;
				if (physCompA->onOverlap) {
					if (physCompA->onOverlap(gameObjB.get())) {
						consumed = true;
					}
				}
				if (physCompB->onOverlap) {
					if (physCompB->onOverlap(gameObjA.get())) {
						consumed = true;
					}
				}
				if (consumed) {
					continue;
				}

				glm::vec2 velA = gameObjA->getVelocity();
				glm::vec2 velB = gameObjA->getVelocity();

				gameObjA->setPosition(gameObjA->getPosition() + -normal * depth / 2.f);
				gameObjA->setVelocity(velA + -normal * (static_cast<float>(velB.length()) / physCompA->getMass()));

				gameObjB->setPosition(gameObjB->getPosition() + normal * depth / 2.f);
				gameObjB->setVelocity(velB + normal * (static_cast<float>(velA.length()) / physCompA->getMass()));
			}
		}
	}
}

bool Physics::checkForIntersection(glm::vec2 centerA, float radiusA, glm::vec2 centerB, float radiusB, glm::vec2& normal, float& depth)
{
	normal = glm::vec2(0.f, 0.f);
	depth = 0.f;

	float distance = glm::distance(centerA, centerB);
	float radii = radiusA + radiusB;

	if (distance >= radii) {
		return false;
	}

	normal = glm::normalize(centerB - centerA);
	depth = radii - distance;
	
	return true;
}


