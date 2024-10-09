#pragma once

#include "sre/SpriteBatch.hpp"
#include <picojson.h>

// Forward declaration
class GameObject;
class PhysicsComponent;

class Component {                                           // Abstract class that adds behavior to a GameObject
public:
    explicit Component(GameObject *gameObject);
    virtual ~Component() = default;

    virtual void initialize(picojson::value& serializedData);

    GameObject *getGameObject();

    virtual bool onKey(SDL_Event &event);                   // The function should return true, if the key event is consumed. This prevents other components to receive the event.

    virtual void update(float deltaTime);
    virtual void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);

    virtual bool onCollision(GameObject* gameObject);

    glm::vec2 getLocalPosition();
    void setLocalPosition(glm::vec2 localPosition);

    std::string getName();
    void setName(std::string &&name);

    glm::vec2 getLocalInGlobalSpace();

protected:
    GameObject *gameObject;

    friend class GameObject;
private:
    glm::vec2 localPosition = { 0.f , 0.f };
    std::string name = "_";
};

