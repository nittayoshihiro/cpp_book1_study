// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mAnimIndex(0)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimIndex >= mAnims.size())
	{
		return;// アニメーションが存在しない
	}

	if (mAnims[mAnimIndex].textures.size() > 0)
	{
		if (!mAnims[mAnimIndex].loop && mAnims[mAnimIndex].textures[static_cast<int>(mCurrFrame)] == mAnims[mAnimIndex].textures[mAnims[mAnimIndex].textures.size()-1])
		{
			return;//アニメーションループしない
		}

		// Update the current frame based on frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;
		
		// Wrap current frame if needed
		while (mCurrFrame >= mAnims[mAnimIndex].textures.size())
		{
			mCurrFrame -= mAnims[mAnimIndex].textures.size();
		}

		// Set the current texture
		SetTexture(mAnims[mAnimIndex].textures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	//mAnimTextures = textures;
	//if (mAnimTextures.size() > 0)
	//{
	//	// Set the active texture to first frame
	//	mCurrFrame = 0.0f;
	//	SetTexture(mAnimTextures[0]);
	//}
	//アニメーションが1つの時はアニメーションindexを0で追加する
	mAnims.resize(1);
	mAnims[0].textures = textures;
	mAnims[0].loop = true;
	if (mAnims[0].textures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnims[0].textures[0]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const int animIndex, const std::vector<SDL_Texture*>& textures, bool loop)
{
	if (mAnims.size()<=animIndex)
	{
		mAnims.resize(animIndex+1);
	}

	mAnims[animIndex].loop = loop;
	mAnims[animIndex].textures = textures;
	if (mAnims[animIndex].textures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnims[animIndex].textures[0]);
	}
}

void AnimSpriteComponent::ChangeAnim(int animIndex)
{
	if (animIndex <= 0&&animIndex <= mAnims.size()-1)
	{
		return;//無効な数値
	}
	mAnimIndex = animIndex;
	mCurrFrame = 0.0f;
}
