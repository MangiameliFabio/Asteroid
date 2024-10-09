#include "GameObject.h"
#include <cassert>
#include <algorithm>
#include "Component.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(picojson::value& serializedData)
{
    name = serializedData.get("name").get<std::string>();
    auto newPosition = serializedData.get("position").get<picojson::array>();
    auto newScale = serializedData.get("position").get<picojson::array>();

    position.x = newPosition[0].get<double>();
    position.y = newPosition[1].get<double>();

    scale.x = newPosition[0].get<double>();
    scale.y = newPosition[1].get<double>();

    rotation = serializedData.get("rotation").get<double>();

}

GameObject::~GameObject(){
    // remove reference to this in components
    for (auto& c : components) {
        c->gameObject = nullptr;
    }
}

bool GameObject::removeComponent(std::shared_ptr<Component> component) {
    auto comp = std::find(components.begin(), components.end(), component);
    if (comp != components.end()){
        components.erase(comp);
    }
    return false;
}

void GameObject::cleanGameObject()
{
    markedForDeletion = true;
}

const glm::vec2 &GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const glm::vec2 &position) {
    GameObject::position = position;
}

const glm::vec2& GameObject::getVelocity() const
{
    return velocity;
}

void GameObject::setVelocity(const glm::vec2& velocity)
{
    GameObject::velocity = velocity;
}

float GameObject::getRotation() const {
    return rotation;
}

void GameObject::setRotation(const float rotation) {
    GameObject::rotation = rotation;
}

float GameObject::getRotationSpeed() const
{
    return rotationSpeed;
}

void GameObject::setRotationSpeed(float rotationSpeed)
{
    GameObject::rotationSpeed = rotationSpeed;
}

const glm::vec2& GameObject::getScale()
{
    return scale;
}

void GameObject::setScale(const glm::vec2& scale)
{
    GameObject::scale = scale;
}

void GameObject::renderSprite(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) {
    for (auto& comp : components){
        comp->renderSprite(spriteBatchBuilder);
    }
}

void GameObject::update(float deltaTime) {
    for (auto& comp : components){
        comp->update(deltaTime);
    }
}

const std::vector<std::shared_ptr<Component>> &GameObject::getComponents() {
    return components;
}
