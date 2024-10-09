#pragma once

#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "sre/SpriteBatch.hpp"
#include <picojson.h>

#define GLM_ENABLE_EXPERIMENTAL

// Forward declaration
class Component;

// GameObject are empty container objects, which contains Components
class GameObject {
public:
    GameObject() = default;
    GameObject(picojson::value& serializedData);
    ~GameObject();

    template <class T>
    std::shared_ptr<T> addComponent();   

    template <class T>
    std::shared_ptr<Component> addComponentByJSON(picojson::value& serializedData);

    template <class T>                                   
    std::shared_ptr<T> getComponent();                   
    
    template <class T>
    std::shared_ptr<T> getComponentByName(std::string name);                  

    bool removeComponent(std::shared_ptr<Component> component);

    void cleanGameObject();

    void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);
    void update(float deltaTime);

    const glm::vec2 &getPosition() const;

    void setPosition(const glm::vec2 &position);

    const glm::vec2& getVelocity() const;

    void setVelocity(const glm::vec2& velocity);

    float getRotation() const;

    void setRotation(const float rotation);

    float getRotationSpeed() const;

    void setRotationSpeed(float rotationSpeed);

    const glm::vec2& getScale();

    void setScale(const glm::vec2& scale);

    const std::vector<std::shared_ptr<Component>>& getComponents();

    std::string name = "_";
    bool markedForDeletion = false;

private:
    std::vector<std::shared_ptr<Component>> components;

    glm::vec2 position;
    glm::vec2 velocity = { 0.f, 0.f };
    glm::vec2 scale = { 1.f, 1.f };
    float rotation;
    float rotationSpeed = 0.f;
};

// definition of the template member function addComponent
// Usage:
// GameObject* go = something;
// shared_ptr<SpriteComponent> sc = go->addComponent<SpriteComponent>();
template <class T>
inline std::shared_ptr<T> GameObject::addComponent(){
    auto obj = std::shared_ptr<T>(new T(this));
    components.push_back(obj);

    return obj;
}

template<class T>
inline std::shared_ptr<Component> GameObject::addComponentByJSON(picojson::value& serializedData)
{
    auto obj = std::shared_ptr<T>(new T(this));
    obj->initialize(serializedData);

    components.push_back(obj);
    return obj;
}

// definition of the template member function addComponent
// Usage:
// GameObject* go = something;
// shared_ptr<SpriteComponent> sc = go->addComponent<SpriteComponent>();
template <class T>
inline std::shared_ptr<T> GameObject::getComponent(){
    for (auto c : components){
        std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(c);
        if (res != nullptr){
            return res;
        }
    }
    return std::shared_ptr<T>();
}

template <class T>
inline std::shared_ptr<T> GameObject::getComponentByName(std::string name) {
    for (auto c : components) {
        std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(c);
        if (res->getName() != name) {
            continue;
        }
        if (res != nullptr) {
            return res;
        }
    }
    return std::shared_ptr<T>();
}
