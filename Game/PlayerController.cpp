#include "PlayerController.h"

#include "SDL_events.h"
#include "GameObject.h"
#include "Globals.h"

#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <SpriteComponent.h>
#include <PhysicsComponent.h>
#include "sre/SpriteAtlas.hpp"
#include <Projectile.h>

PlayerController::PlayerController(GameObject* gameObject) : Component(gameObject) {};

PlayerController::~PlayerController() {}

void PlayerController::update(float deltaTime)
{
    if (attackCoolDown > 0.f) {
        attackCoolDown -= deltaTime;
    }
    else {
        attackCoolDown = 0.f;
    }
    handleInputBuffer();

    auto gameObject = getGameObject();
    switch( rotationState) {
    case InputType::LEFT: {
        gameObject->setRotationSpeed(5.f);
        break;
       }
           
    case InputType::RIGHT: {
        gameObject->setRotationSpeed(-5.f);
        break;
       }

       default:
           break;
    }

    switch (boostState)
    {
    case InputType::BOOST:
    {
        gameObject->setVelocity(getAimDir() * 5.f);
        break;
    }
    default:
        break;
    }

    switch (fireState) {
        case InputType::FIRE: {
            if (attackCoolDown <= 0.f) {
                attackCoolDown = attackSpeed;
                fireProjectile();
            }
            break;
        }

        default:
            break;
    }

    checkIfOutsideScreen();
}

bool PlayerController::onKey(SDL_Event& event)
{
    switch (event.key.keysym.sym) {
    case SDLK_w:
        if (event.type == SDL_KEYDOWN) {
            addToBuffer(InputType::BOOST);

            auto thrusterSprite = gameObject->getComponentByName<SpriteComponent>("thrusterSprite");
            thrusterSprite->setSpriteVisibility(true);
        }
        if (event.type == SDL_KEYUP) {
            removeFromBuffer(InputType::BOOST);

            auto thrusterSprite = gameObject->getComponentByName<SpriteComponent>("thrusterSprite");
            thrusterSprite->setSpriteVisibility(false);
        }
        break;
    case SDLK_a:
        if (event.type == SDL_KEYDOWN) {
            addToBuffer(InputType::LEFT);
        }
        if (event.type == SDL_KEYUP) {
            removeFromBuffer(InputType::LEFT);
        }
        break;
    case SDLK_d:
        if (event.type == SDL_KEYDOWN) {
            addToBuffer(InputType::RIGHT);
        }
        if (event.type == SDL_KEYUP) {
            removeFromBuffer(InputType::RIGHT);
        }
        break;
    case SDLK_SPACE:
        if (event.type == SDL_KEYDOWN) {
            addToBuffer(InputType::FIRE);
        }
        if (event.type == SDL_KEYUP) {
            removeFromBuffer(InputType::FIRE);
        }
        break;
    default:
        break;
    }

    return false;
}

bool PlayerController::onCollision(GameObject* gameObject)
{
    if (gameObject->name == "Asteroid") {
        auto gameObj = getGameObject();
        gameObj->markedForDeletion = true;

        GLOBALS->spawnExplosion(gameObj->getPosition());
        return true;
    }

    if (gameObject->name == "Projectile") {
        return true;
    }
    return false;
}

void PlayerController::addToBuffer(const InputType input)
{
    auto it = std::find(inputBuffer.begin(), inputBuffer.end(), input);
    if (it == inputBuffer.end()) {
        inputBuffer.push_back(input);
    }
}

void PlayerController::removeFromBuffer(const InputType input)
{
    auto it = std::find(inputBuffer.begin(), inputBuffer.end(), input);
    inputBuffer.erase(it);
}

void PlayerController::handleInputBuffer()
{
    boostState = InputType::IDLE;
    rotationState = InputType::IDLE;
    fireState = InputType::IDLE;

    for (auto input : inputBuffer) {
        switch (input) {
        case InputType::LEFT: {
            rotationState = InputType::LEFT;
            break;
        }
        case InputType::RIGHT: {
            rotationState = InputType::RIGHT;
            break;
        }
        case InputType::BOOST: {
            boostState = InputType::BOOST;
            break;
        }
        case InputType::FIRE: {
            fireState = InputType::FIRE;
            break;
        }
        default: {
            break;
        }
    }
    }
}

void PlayerController::checkIfOutsideScreen()
{
    glm::vec2 pos = gameObject->getPosition();

    if (pos.x < 0.f) {
        pos.x = GLOBALS->window_size.x;
    }

    if (pos.x > GLOBALS->window_size.x) {
        pos.x = 0.f;
    }

    if (pos.y < 0.f) {
        pos.y = GLOBALS->window_size.y;
    }

    if (pos.y > GLOBALS->window_size.y) {
        pos.y = 0.f;
    }

    gameObject->setPosition(pos);
}

void PlayerController::fireProjectile()
{
    auto projectile = GLOBALS->createGameObject();
    projectile->name = "Projectile";
    projectile->setPosition(getGameObject()->getPosition());
    projectile->setVelocity(getAimDir() * 10.f);
    projectile->setRotation(getGameObject()->getRotation());
    auto projComp = projectile->addComponent<Projectile>();

    auto projectileSprite = projectile->addComponent<SpriteComponent>();
    projectileSprite->setSprite(GLOBALS->atlas->get("laserRed01.png"));

    auto physics = projectile->addComponent<PhysicsComponent>();
    physics->setVelocityDrag(0.f);
    physics->setCollisionCircle((projectileSprite->getSprite().getSpriteSize().x * projectile->getScale().x) / 2);
    physics->onOverlap = [projComp](GameObject* gameObject) { return projComp->onCollision(gameObject); };
}

glm::vec2 PlayerController::getAimDir()
{
    glm::vec2 dir(0.f, 1.f);

    GLOBALS->rotateVector(dir, gameObject->getRotation());
    return dir;
}
