// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "CircleComponent.h"
#include "Asteroid.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
	:Actor(game)
	,mLaserCooldown(0.0f)
	,mShipActive(true)
	,mShipCooldown(0.0f)
{
	// Create a sprite component
	mSpriteC = new SpriteComponent(this, 150);
	mShipTex = game->GetTexture("Assets/Ship.png");
	mSpriteC->SetTexture(mShipTex);

	// Create an input component and set keys/speed
	mInputC = new InputComponent(this);
	mInputC->SetForwardKey(SDL_SCANCODE_W);
	mInputC->SetBackKey(SDL_SCANCODE_S);
	mInputC->SetClockwiseKey(SDL_SCANCODE_A);
	mInputC->SetCounterClockwiseKey(SDL_SCANCODE_D);
	mInputC->SetMaxForwardSpeed(300.0f);
	mInputC->SetMaxAngularSpeed(Math::TwoPi);

	mCircleC = new CircleComponent(this);
	mCircleC ->SetRadius (40.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	if (mShipActive)
	{
		mLaserCooldown -= deltaTime;
		// Do we intersect with an asteroid?
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircleC, *(ast->GetCircle())))
			{
				// The first asteroid we intersect with,
				// set ourselves and the asteroid to dead
				//SetState(EDead);
				//ast->SetState(EDead);
				mSpriteC->SetTexture(nullptr);
				mShipActive = false;
				mShipCooldown = 2.0f;
				break;
			}
		}
	}
	else
	{
		if (mShipCooldown < 0.0f)
		{
			mSpriteC->SetTexture(mShipTex);
			SetPosition(Vector2(512.0f, 384.0f));
			SetRotation(Math::PiOver2);
			mShipActive = true;
			mLaserCooldown = 0.0f;
			// 物理状態をリセット
			//mInputC->AddForce(Vector2(0.0f, 0.0f));

			// 速度もリセットしたい場合
			mInputC->SetVelocity(Vector2(0.0f, 0.0f));
			mInputC->ClearForces();
		}
		else
		{
			mShipCooldown -= deltaTime;
		}

	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}
