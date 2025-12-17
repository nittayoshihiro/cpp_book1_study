// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AIState.h"
#include "AIComponent.h"
#include <SDL/SDL_log.h>
#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"

void AIPatrol::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
	bool dead = true;
	if (dead)
	{
		mOwner->ChangeState("Death");
	}
}

void AIPatrol::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIPatrol::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void AIDeath::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
}

void AIDeath::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIDeath::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void AIAttack::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
}

void AIAttack::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIAttack::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void TowerAIInvestigate::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
	Tower* t = static_cast<Tower*>(mOwner->GetOwner());
	if (t != nullptr)
	{
		t->Actor::UpdateActor(deltaTime);
		Enemy* e = t->GetGame()->GetNearestEnemy(t->GetPosition());
		if (e != nullptr)
		{
			// Vector from me to enemy
			Vector2 dir = e->GetPosition() - t->GetPosition();
			float dist = dir.Length();
			if (dist < t->GetAttackRange())
			{
				//チェンジ標的ステート
				mOwner->ChangeState("TowerAIAlert");
			}
		}
	}

}

void TowerAIInvestigate::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void TowerAIInvestigate::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void TowerAIAlert::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
	Tower* t = static_cast<Tower*>(mOwner->GetOwner());
	if (t != nullptr)
	{
		t->Actor::UpdateActor(deltaTime);
		Enemy* e = t->GetGame()->GetNearestEnemy(t->GetPosition());
		if (e != nullptr)
		{
			// Vector from me to enemy
			Vector2 dir = e->GetPosition() - t->GetPosition();
			// Rotate to face enemy
			t->SetRotation(Math::Atan2(-dir.y, dir.x));

			float dist = dir.Length();
			if (dist < t->GetAttackRange())
			{
				if (t->GetNextAttack()<0.0f)//攻撃クールタイム
				{
					//チェンジ攻撃ステート
					mOwner->ChangeState("TowerAIAttack");
				}
				else
				{
					t->ReduceAttackTime(deltaTime);
				}
			}
			else
			{
				//チェンジ調査ステート
				mOwner->ChangeState("TowerAIInvestigate");
			}
		}
	}
}

void TowerAIAlert::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
	Tower* t = static_cast<Tower*>(mOwner->GetOwner());
	if (t != nullptr)
	{
		t->ResetAttackTime();
	}
}

void TowerAIAlert::OnExit()
{
	SDL_Log("Exiting %s state", GetName());

}

void TowerAIAttack::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
	Tower* t = static_cast<Tower*>(mOwner->GetOwner());
	if (t != nullptr)
	{
		// Spawn bullet at tower position facing enemy
		Bullet* b = new Bullet(t->GetGame());
		b->SetPosition(t->GetPosition());
		b->SetRotation(t->GetRotation());
		//チェンジ標的ステート
		mOwner->ChangeState("TowerAIAlert");
	}

}

void TowerAIAttack::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void TowerAIAttack::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}
