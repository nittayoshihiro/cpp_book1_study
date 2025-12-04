// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "SDL/SDL.h"
class Ship : public Actor
{
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
private:
	class SpriteComponent* mSpriteC;
	class CircleComponent* mCircleC;
	SDL_Texture* mShipTex;
	float mLaserCooldown;
	bool mShipActive;
	float mShipCooldown;
};