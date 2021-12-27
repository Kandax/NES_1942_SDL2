#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>


#include <SDL.h>
#include <SDL_image.h>

#include "ElapsedTime.h"

#include "Texture.h"
#include "SpriteSheet.h"


struct Player {
	float posX = 300;
	float posY = 300;

	float width = 23;
	float height = 17;

	float scale = 15;

	float playerSpeed = 4;
	int numberOfTextures = 7;
	int textureIndex = 1;

	SDL_Rect boundingBox;

	void update() {
		boundingBox.x = posX;
		boundingBox.y = posY;
		boundingBox.w = width * scale;
		boundingBox.h = height * scale;
	}

};

struct BulletPlayer {



	float posX = 0;
	float posY = 0;

	float width = 2;
	float height = 7;

	float scale = 15;

	float bulletSpeed = 14;
	int numberOfProjectiles = 4;
	int projectilesOffset = 2;

	Texture *bulletTexture = nullptr;

	SDL_Rect boundingBox;

	BulletPlayer(float posX, float posY) 
		: posX(posX)
		, posY(posY)
	{

	}


	void update() {
		boundingBox.x = posX;
		boundingBox.y = posY;
		boundingBox.w = width  * numberOfProjectiles * projectilesOffset * scale - width * scale;
		boundingBox.h = height * scale;
	}


};



class Core
{
public:
	Core();
	~Core();
	// Runs program
	void run();
private:
	// Initialize sdl, sdl_image, window and renderer.
	// Returns true if initializing was successful.
	bool init();
	// Loads textures	
	// Returns true if loading media was successful.
	bool loadMedia();
	// Free memory
	void close();

	// For the game loop
	void events();
	void input();
	void update();
	void render();
	


	// Checks if two bounding boxes collides. Return true if collide
	bool isColliding(const SDL_Rect& firstBoundingBox, const SDL_Rect& secondBoundingBox);

private:
	std::string mWindowTitle;

	
	const uint8_t BLOCK_SIZE;
	// Number of rows needed to create background
	const uint8_t ROWS;
	// Number of columns needed to create background
	const uint8_t COLUMNS;
	 
	const uint16_t WINDOW_WIDTH;
	const uint16_t WINDOW_HEIGHT;


	bool mQuitWindow;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Event mEvent;

	const uint8_t* KEYBOARD;

	// Delta time
	ElapsedTime mET;

	// Textures
	Texture mTextureWater;
	SpriteSheet mSpritePlayer;
	Texture mTextureBulletPlayer;

	Player mPlayer;
	std::vector<BulletPlayer> mPlayerBullets;


};

