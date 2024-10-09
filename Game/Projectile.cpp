#include "Projectile.h"
#include "GameObject.h"
#include <Globals.h>

Projectile::Projectile(GameObject* gameObject) : Component(gameObject)
{
}

Projectile::~Projectile()
{
}

void Projectile::update(float deltaTime)
{
    if (checkIfOutOfScreen()) {
        getGameObject()->markedForDeletion = true;
    }
}

bool Projectile::onCollision(GameObject* gameObject)
{
    if (gameObject->name == "Asteroid") {
        getGameObject()->markedForDeletion = true;
        return true;
    }
    if (gameObject->name == "Player") {
        return true;
    }
    return false;
}

bool Projectile::checkIfOutOfScreen()
{
    if (getGameObject()->getPosition().x <= 0.f - 50.f ||
        getGameObject()->getPosition().x >= GLOBALS->window_size.x + 50.f ||
        getGameObject()->getPosition().y <= 0.f - 50.f ||
        getGameObject()->getPosition().y >= GLOBALS->window_size.y + 50.f)
    {
        return true;
    }
    return false;
}
