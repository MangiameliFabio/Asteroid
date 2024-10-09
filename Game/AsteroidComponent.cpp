#include "AsteroidComponent.h"

#include "GameObject.h"
#include "Globals.h"

AsteroidComponent::AsteroidComponent(GameObject* gameObject) : Component(gameObject){}

AsteroidComponent::~AsteroidComponent(){

}

void AsteroidComponent::update(float deltaTime)
{
    if (checkIfOutOfScreen()) {
        getGameObject()->markedForDeletion = true;
    }
}

bool AsteroidComponent::onCollision(GameObject* gameObject)
{
   if (gameObject->name == "Projectile") {
        auto gameObj = getGameObject();
        gameObj->markedForDeletion = true;

        GLOBALS->spawnExplosion(gameObj->getPosition());
        return true;
   }
   return false;
}

bool AsteroidComponent::checkIfOutOfScreen()
{
    if (getGameObject()->getPosition().x <= 0.f - 50.f||
        getGameObject()->getPosition().x >= GLOBALS->window_size.x + 50.f ||
        getGameObject()->getPosition().y <= 0.f - 50.f ||
        getGameObject()->getPosition().y >= GLOBALS->window_size.y + 50.f)
    {
        return true;
    }
    return false;
}
