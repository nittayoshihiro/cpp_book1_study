// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddle1Dir(0)
,mPaddle2Dir(0)
,mBalls(2)
{
	
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	//
	mPaddlePos1.x = 10.0f;
	mPaddlePos1.y = 768.0f / 2.0f;
	mPaddlePos2.x = 1024.0f - (10.0f + 15.0f);
	mPaddlePos2.y = 768.0f / 2.0f;
	mBalls[0].position.x = 1024.0f / 2.0f;
	mBalls[0].position.y = 768.0f / 2.0f;
	mBalls[0].velocity.x = -200.0f;
	mBalls[0].velocity.y = 235.0f;
	mBalls[1].position.x = 1024.0f / 2.0f;
	mBalls[1].position.y = 768.0f / 2.0f;
	mBalls[1].velocity.x = 200.0f;
	mBalls[1].velocity.y = 235.0f;

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle direction based on W/S keys
	mPaddle1Dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddle1Dir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddle1Dir += 1;
	}
	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddle2Dir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddle2Dir += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// Update paddle position based on direction
	if (mPaddle1Dir != 0)
	{
		mPaddlePos1.y += mPaddle1Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos1.y < (paddleH/2.0f + thickness))
		{
			mPaddlePos1.y = paddleH/2.0f + thickness;
		}
		else if (mPaddlePos1.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddlePos1.y = 768.0f - paddleH/2.0f - thickness;
		}
	}

	if (mPaddle2Dir != 0)
	{
		mPaddlePos2.y += mPaddle2Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos2.y < (paddleH/2.0f + thickness))
		{
			mPaddlePos2.y = paddleH/2.0f + thickness;
		}
		else if (mPaddlePos2.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddlePos2.y = 768.0f - paddleH/2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity
	for (size_t i = 0; i < mBalls.size(); i++)
	{
		mBalls[i].position.x += mBalls[i].velocity.x * deltaTime;
		mBalls[i].position.y += mBalls[i].velocity.y * deltaTime;
		printf("deltaTime:%f\n",deltaTime);
		// Bounce if needed
		// Did we intersect with the paddle?
		float diff1 = mPaddlePos1.y - mBalls[i].position.y;
		float diff2 = mPaddlePos2.y - mBalls[i].position.y;
		// Take absolute value of difference
		diff1 = (diff1 > 0.0f) ? diff1 : -diff1;
		if (
			// Our y-difference is small enough
			diff1 <= paddleH / 2.0f &&
			// We are in the correct x-position
			mBalls[i].position.x <= 25.0f && mBalls[i].position.x >= 20.0f &&
			// The ball is moving to the left
			mBalls[i].velocity.x < 0.0f)
		{
			mBalls[i].velocity.x *= -1.0f;
		}
		// Did the ball go off the screen? (if so, end game)
		else if (mBalls[i].position.x <= 0.0f)
		{
			mIsRunning = false;
		}
		//// Did the ball collide with the right wall?
		//else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
		//{
		//	mBallVel.x *= -1.0f;
		//}

		if (
			// Our y-difference is small enough
			diff2 <= paddleH / 2.0f &&
			// We are in the correct x-position
			mBalls[i].position.x <= 1024.0f - (10.0f + 10.0f) && mBalls[i].position.x >= 1024.0f - (10.0f + 15.0f) &&
			// The ball is moving to the left
			mBalls[i].velocity.x > 0.0f)
		{
			mBalls[i].velocity.x *= -1.0f;
		}
		// Did the ball go off the screen? (if so, end game)
		else if (1024.0f <= mBalls[i].position.x)
		{
			mIsRunning = false;
		}

		// Did the ball collide with the top wall?
		if (mBalls[i].position.y <= thickness && mBalls[i].velocity.y < 0.0f)
		{
			mBalls[i].velocity.y *= -1;
		}
		// Did the ball collide with the bottom wall?
		else if (mBalls[i].position.y >= (768 - thickness) &&
			mBalls[i].velocity.y > 0.0f)
		{
			mBalls[i].velocity.y *= -1;
		}
		//ìÆçÏämîFóp
		printf("mBalls[i]:%d\n",i);
		printf("mBalls[i].velocity.x:%f\nmBalls[i].velocity.y:%f\n",mBalls[i].velocity.x,mBalls[i].velocity.y);
		printf("mBalls[i].position.x:%f\nmBalls[i].position.y:%f\n",mBalls[i].position.x,mBalls[i].position.y);
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	//// Draw right wall
	//wall.x = 1024 - thickness;
	//wall.y = 0;
	//wall.w = thickness;
	//wall.h = 1024;
	//SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw paddle
	SDL_Rect paddle1{
		static_cast<int>(mPaddlePos1.x),
		static_cast<int>(mPaddlePos1.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle1);
	SDL_Rect paddle2{
		static_cast<int>(mPaddlePos2.x),
		static_cast<int>(mPaddlePos2.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);
	
	// Draw ball
	for (size_t i = 0; i < mBalls.size(); i++)
	{
		SDL_Rect ball{
			static_cast<int>(mBalls[i].position.x - thickness / 2),
			static_cast<int>(mBalls[i].position.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ball);
	}
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
