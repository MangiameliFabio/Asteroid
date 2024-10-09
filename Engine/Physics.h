#pragma once
#include "glm/glm.hpp"

class Physics {
public:
	void updatePhysics();

private:
	bool checkForIntersection(glm::vec2 centerA, float radiusA, glm::vec2 centerB, float radiusB, glm::vec2& normal, float& depth);

};