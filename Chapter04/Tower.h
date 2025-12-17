// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"

class Tower : public Actor
{
public:
	Tower(class Game* game);
	void UpdateActor(float deltaTime) override;
	float GetAttackRange()const { return AttackRange; };
	float GetNextAttack()const { return mNextAttack; };
	void ReduceAttackTime(float deltaTime){mNextAttack -= deltaTime; };
	void ResetAttackTime(){ mNextAttack = AttackTime; };

private:
	class MoveComponent* mMove;
	float mNextAttack;
	const float AttackTime = 1.0f;
	const float AttackRange = 100.0f;
};
