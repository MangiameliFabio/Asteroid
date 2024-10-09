#include <Globals.h>
#include <PhysicsComponent.h>
#include <SpriteComponent.h>
#include <PlayerController.h>
#include <GameObject.h>
#include <Explosion.h>
#include <Component.h>

#include <fstream>
#include <memory>
#include <sre/SpriteAtlas.hpp>


Globals* Globals::_instance = nullptr;

Globals::Globals()
{
	assert(_instance == nullptr && " Only one instance of MyEngine::Engine allowed!");
	_instance = this;
}

Globals::~Globals()
{
}

void Globals::Init(glm::vec2& winSize)
{
    window_size = winSize;
}

void Globals::rotateVector(glm::vec2& vec, float rotation)
{
    rotation = glm::radians(rotation);

    glm::mat2 rotationMatrix = glm::mat2(
        cos(rotation), sin(rotation),
        -sin(rotation), cos(rotation)
    );

    vec = rotationMatrix * vec;
}

void Globals::spawnExplosion(glm::vec2 pos)
{
    auto explosion = createGameObject();
    explosion->setPosition(pos);
    explosion->name = "Explosions";

    auto explosionSprite = explosion->addComponent<SpriteComponent>();
    explosionSprite->setSprite(atlas->get("bang.png"));
    auto explosionComp = explosion->addComponent<Explosion>();
}

void Globals::setUpScene(std::string&& filePath)
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

    auto test = v.get<picojson::object>();

    createGameObject(v.get("Player"));
}

bool Globals::approx_zero(glm::vec2 pos)
{
    if (pos.length() <= 0.01)
        return true;
    return false;
}

std::shared_ptr<GameObject> Globals::createGameObject(){
	auto obj = std::make_shared<GameObject>();
	sceneObjects.push_back(obj);

	return obj;
}

std::shared_ptr<GameObject> Globals::createGameObject(picojson::value& serializedData)
{
    auto newObj = std::make_shared<GameObject>(serializedData);

    auto data = serializedData.get("components").get<picojson::array>();

    for (auto d : data) {
        std::string typeId = d.get("typeId").get<std::string>();

        if (typeId == "PLAYER_CONTROLLER") {
            newObj->addComponentByJSON<PlayerController>(d.get("serializedData"));
        }
        else if (typeId == "PHYSICS_COMPONENT") {
            newObj->addComponentByJSON<PhysicsComponent>(d.get("serializedData"));
        }
        else if (typeId == "SPRITE_COMPONENT") {
            newObj->addComponentByJSON<SpriteComponent>(d.get("serializedData"));
        }
        else {
            std::cerr << "Invalid Component Id used" << std::endl;
            continue;
        }
    }

    sceneObjects.push_back(newObj);

    return newObj;
}
    


float Globals::getRandValue(int min, int max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(min, max);

    return distribution(generator);
}
