#pragma once

#include "Component.h"
#include "sre/Sprite.hpp"

class SpriteComponent : public Component {
public:
    explicit SpriteComponent(GameObject *gameObject);

    void initialize(picojson::value& serializedData) override;

    void renderSprite(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) override;

    void setSprite(const sre::Sprite& sprite);
    sre::Sprite getSprite();

    void setSpriteVisibility(bool visible);
private:
    sre::Sprite sprite;
};

