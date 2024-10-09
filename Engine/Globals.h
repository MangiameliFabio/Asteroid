#pragma once
#include <glm/glm.hpp>
#include "GameObject.h"
#include <random>
#include <picojson.h>

#define GLOBALS Globals::GetInstance()

class GameObject;

class Globals {
	static Globals* _instance;
public:
	Globals();
	~Globals();

	static Globals* GetInstance() { return _instance; }

	void Init(glm::vec2& winSize);
	void rotateVector(glm::vec2& vec, float rotation);
	void spawnExplosion(glm::vec2 pos);
	void setUpScene(std::string&& filePath);
	bool approx_zero(glm::vec2 pos);
	std::shared_ptr<GameObject> createGameObject();
	std::shared_ptr<GameObject> createGameObject(picojson::value& serializedData);
	float getRandValue(int min, int max);

	glm::vec2 window_size;
	std::vector<std::shared_ptr<GameObject>> sceneObjects;
	std::shared_ptr<sre::SpriteAtlas> atlas;
};