#pragma once

#include "Globals.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "PhysicsComponent.h"

#include <memory>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <PlayerController.h>
#include <AsteroidSpawner.h>
#include <Physics.h>

sre::SDLRenderer renderer;
sre::Camera camera;
Globals globals;
Physics physic;

void ProcessEvents(SDL_Event& event) {
	std::vector<std::shared_ptr<GameObject>> tempList = GLOBALS->sceneObjects;
	for (auto& gameObject : tempList) {
		for (auto& c : gameObject->getComponents()) {
			bool consumed = c->onKey(event);
			if (consumed) {
				return;
			}
		}
	}
}
void Update(float deltaTime) {
	physic.updatePhysics();

	bool cleanUp = false;
	for (int i = 0; i < GLOBALS->sceneObjects.size(); i++) {
		if (GLOBALS->sceneObjects[i]->markedForDeletion) {
			GLOBALS->sceneObjects[i] = nullptr;
			cleanUp = true;
			continue;
		}
		GLOBALS->sceneObjects[i]->update(deltaTime);
	}
	if (cleanUp) {
		GLOBALS->sceneObjects.erase(
			std::remove(begin(GLOBALS->sceneObjects),
				end(GLOBALS->sceneObjects), nullptr),
			end(GLOBALS->sceneObjects));
	}
}
void Render() {
	sre::Color backgroundColor = { 0.f, 0.133f, 0.341f, 1.0f };

	sre::RenderPass renderPass = sre::RenderPass::create()
		.withCamera(camera)
		.withClearColor(true, backgroundColor)
		.build();
	sre::SpriteBatch::SpriteBatchBuilder spriteBatchBuilder
		= sre::SpriteBatch::create();
	// send spriteBatchBuilder to your game elements, so that
	//they can add their sprites for rendering
	for (int i = 0; i < GLOBALS->sceneObjects.size(); i++) {
		GLOBALS->sceneObjects[i]->renderSprite(spriteBatchBuilder);
	}
	auto spriteBatch = spriteBatchBuilder.build();
	renderPass.draw(spriteBatch);
}

int main() {
	GLOBALS->Init(glm::vec2(800, 600));

	renderer.frameRender = Render;
	renderer.frameUpdate = Update;
	renderer.keyEvent = ProcessEvents;
	renderer.setWindowSize(GLOBALS->window_size);
	renderer.init();
	camera.setWindowCoordinates();

	GLOBALS->atlas = sre::SpriteAtlas::create("data/asteroid_sprite_sheet.json",
		"data/asteroid_sprite_sheet.png");

	GLOBALS->setUpScene("data/MainScene.json");
	
	/*auto player = GLOBALS->createGameObject();
	player->name = "Player";
	player->setPosition(GLOBALS->window_size / 2.f);
	player->setScale({ 0.5f,0.5f });
	
	auto playerSprite = player->addComponent<SpriteComponent>();
	playerSprite->setSprite(GLOBALS->atlas->get("playerShip2_green.png"));
	playerSprite->setName("playerSprite");

	auto thrusterSprite = player->addComponent<SpriteComponent>();
	thrusterSprite->setSprite(GLOBALS->atlas->get("fire16.png"));
	thrusterSprite->setName("thrusterSprite");
	thrusterSprite->setLocalPosition({ 0, -25.f });
	thrusterSprite->setSpriteVisibility(false);

	auto playerPhys = player->addComponent<PhysicsComponent>();
	auto playerContr = player->addComponent<PlayerController>();
	playerPhys->onOverlap = [&playerContr](GameObject* gameObject) { return playerContr->onCollision(gameObject); };
	playerPhys->setCollisionCircle((playerSprite->getSprite().getSpriteSize().y * player->getScale().y) / 2.f); */

	auto asteroidSpawner = GLOBALS->createGameObject();
	asteroidSpawner->name = "AsteroidSpawner";
	auto asc = asteroidSpawner->addComponent<AsteroidSpawner>();
	asc->init("data/asteroid_sprite_sheet.json");

	renderer.startEventLoop();
}
