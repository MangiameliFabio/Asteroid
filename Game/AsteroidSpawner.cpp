#include "AsteroidSpawner.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <picojson.h> 
#include <Globals.h>
#include <SpriteComponent.h>
#include <PhysicsComponent.h>

#include "sre/SpriteAtlas.hpp"
#include <AsteroidComponent.h>

AsteroidSpawner::AsteroidSpawner(GameObject* gameObject) : Component(gameObject) {}

AsteroidSpawner::~AsteroidSpawner() {}

float spawnCd = 2.f;
float currentSpawnCd = 0.f;

void AsteroidSpawner::update(float deltaTime)
{
    if (currentSpawnCd >= spawnCd) {
        createAsteroid();
        currentSpawnCd = 0.f;

        if (spawnCd > 0.5) {
            spawnCd -= 0.05;
        }
    }
    else {
        currentSpawnCd += deltaTime;
    }
}

void AsteroidSpawner::init(const std::string& filePath)
{
    getAsteroidPaths(filePath);
    generateSpawnPoints();
}

void AsteroidSpawner::getAsteroidPaths(const std::string& filePath)
{
    // Read the JSON file content
    std::ifstream file(filePath, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    std::string jsonContent((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    // Parse JSON using picojson
    picojson::value v;
    std::string err = picojson::parse(v, jsonContent);
    if (!err.empty()) {
        std::cerr << "JSON parsing error: " << err << std::endl;
        return;
    }

    // Check if JSON is an object and contains "frames"
    if (!v.is<picojson::object>()) {
        std::cerr << "Invalid JSON format!" << std::endl;
        return;
    }

    picojson::object& obj = v.get<picojson::object>();

    // Make sure "frames" exists and is an array
    if (!obj["frames"].is<picojson::array>()) {
        std::cerr << "\"frames\" is not an array!" << std::endl;
        return;
    }

    picojson::array& frames = obj["frames"].get<picojson::array>();

    // Iterate over the frames array
    for (picojson::value& frame : frames) {
        picojson::object& frameObj = frame.get<picojson::object>();
        std::string filename = frameObj["filename"].get<std::string>();

        // Check if the filename starts with "meteor"
        if (filename.rfind("meteor", 0) == 0) {
            spritePath.push_back(filename);  // Add filename to spritePath
        }
    }
}

void AsteroidSpawner::generateSpawnPoints()
{
    float offset = 25.f;
    float amountSpawnsX = 10.f;
    float spawnDistX = GLOBALS->window_size.x / amountSpawnsX;

    for (float i = 0.f; i <= GLOBALS->window_size.x; i += spawnDistX)
    {
        spawnPoints.push_back({ i, GLOBALS->window_size.y + offset });
        spawnPoints.push_back({ i, -offset });
    }

    float amountSpawnsY = 5.f;
    float spawnDistY = GLOBALS->window_size.y / amountSpawnsY;

    for (float i = 0.f; i <= GLOBALS->window_size.y; i += spawnDistY)
    {
        spawnPoints.push_back({ GLOBALS->window_size.x + offset, i });
        spawnPoints.push_back({ -offset, i });
    }
}

void AsteroidSpawner::createAsteroid()
{
    auto asteroid = GLOBALS->createGameObject();
    asteroid->name = "Asteroid";

    auto asteroidSprite = asteroid->addComponent<SpriteComponent>();
    asteroidSprite->setSprite(GLOBALS->atlas->get(spritePath[chooseRandomSprite()]));

    auto physComponent = asteroid->addComponent<PhysicsComponent>();
    physComponent->setRotationDrag(0.f);
    physComponent->setVelocityDrag(0.f);
    physComponent->setMass(asteroidSprite->getSprite().getSpriteSize().x / 10.f);
    physComponent->setCollisionCircle((asteroidSprite->getSprite().getSpriteSize().y * getGameObject()->getScale().y) / 2.f);


    auto asteroidComponent = asteroid->addComponent<AsteroidComponent>();

    asteroid->setPosition(getRandomSpawnLocation());
    asteroid->setVelocity(getRandomVelocity(asteroid->getPosition()) * (1.f / physComponent->getMass()));
    asteroid->setRotationSpeed(GLOBALS->getRandValue(3.f, 10.f) * (1.f / physComponent->getMass()));

    physComponent->onOverlap = [asteroidComponent](GameObject* gameObject) {return asteroidComponent->onCollision(gameObject); };
}

int AsteroidSpawner::chooseRandomSprite()
{
    return GLOBALS->getRandValue(0, spritePath.size() - 1);
}

glm::vec2 AsteroidSpawner::getRandomSpawnLocation()
{
    int rand = GLOBALS->getRandValue(0, spawnPoints.size() - 1);
    return spawnPoints[rand];
}

glm::vec2 AsteroidSpawner::getRandomVelocity(const glm::vec2& startPos)
{
    glm::vec2 dir = (GLOBALS->window_size / 2.f) - startPos; //Get dir into the middle of the screen
    dir = glm::normalize(dir);

    GLOBALS->rotateVector(dir, GLOBALS->getRandValue(-20, 20)); //randomly rotate dir vector

    float speed = GLOBALS->getRandValue(4.f, 8.f); //Get a random speed

    return dir * speed;
}
