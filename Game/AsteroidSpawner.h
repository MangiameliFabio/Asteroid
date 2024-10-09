#pragma once

#include "Component.h"

class AsteroidSpawner : public Component {
public:
	AsteroidSpawner(GameObject* gameObject);
	~AsteroidSpawner();

	void update(float deltaTime) override;
	void init(const std::string& filePath);

private:
	void getAsteroidPaths(const std::string& filePath);
	void generateSpawnPoints();
	void createAsteroid();

	int chooseRandomSprite();

	std::vector<glm::vec2> spawnPoints;
	std::vector<std::string> spritePath;

	glm::vec2 getRandomSpawnLocation();

	glm::vec2 getRandomVelocity(const glm::vec2& startPos);


};