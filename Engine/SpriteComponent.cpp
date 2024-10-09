//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "SpriteComponent.h"
#include "GameObject.h"
#include <Globals.h>
#include "sre/SpriteAtlas.hpp"

SpriteComponent::SpriteComponent(GameObject *gameObject) : Component(gameObject) {}

void SpriteComponent::initialize(picojson::value& serializedData)
{
    Component::initialize(serializedData);

    auto spritePath = serializedData.get("sprite").get<std::string>();
    setSprite(GLOBALS->atlas->get(spritePath));

    auto visible = serializedData.get("visible").get<bool>();
    setSpriteVisibility(visible);
}

void SpriteComponent::renderSprite(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) {
    sprite.setPosition(getLocalInGlobalSpace());
    sprite.setRotation(gameObject->getRotation());
    sprite.setScale(gameObject->getScale());
    spriteBatchBuilder.addSprite(sprite);
}

void SpriteComponent::setSprite(const sre::Sprite &sprite) {
    this->sprite = sprite;
}

sre::Sprite SpriteComponent::getSprite() {
    return sprite;
}

void SpriteComponent::setSpriteVisibility(bool visible)
{
    glm::vec4 color = sprite.getColor();

    if (visible) {
        color.w = 1.f;
        sprite.setColor(color);
    }
    else {
        color.w = 0.f;
        sprite.setColor(color);
    }
}
