//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "Component.h"
#include <Globals.h>


Component::Component(GameObject *gameObject)
:gameObject(gameObject)
{
}

void Component::renderSprite(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) {

}

bool Component::onCollision(GameObject* gameObject)
{
    return false;
}

void Component::update(float deltaTime) {

}

void Component::initialize(picojson::value& serializedData)
{
    name = serializedData.get("name").get<std::string>();
    auto pos = serializedData.get("localPosition").get<picojson::array>();
    localPosition.x = static_cast<float>(pos[0].get<double>());
    localPosition.y = static_cast<float>(pos[1].get<double>());
}

GameObject *Component::getGameObject() {
    return gameObject;
}

bool Component::onKey(SDL_Event &event) {
    return false;
}

glm::vec2 Component::getLocalPosition()
{
    return localPosition;
}

void Component::setLocalPosition(glm::vec2 localPosition)
{
    Component::localPosition = localPosition;
}

std::string Component::getName()
{
    return name;
}

void Component::setName(std::string &&name)
{
    Component::name = name;
}

glm::vec2 Component::getLocalInGlobalSpace()
{
    glm::vec2 globalPosition = localPosition;
    GLOBALS->rotateVector(globalPosition, gameObject->getRotation());
    return getGameObject()->getPosition() + globalPosition;
}
