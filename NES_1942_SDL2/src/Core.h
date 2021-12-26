#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>


#include <SDL.h>
#include <SDL_image.h>

#include "ElapsedTime.h"

#include "Texture.h"
#include "SpriteSheet.h"


struct Player {
	float posX = 0;
	float posY = 0;
	int textureIndex = 0;
};


class Core
{
public:
	Core();
	~Core();
	// Run program
	void run();
private:
	// Initialize sdl, sdl_image, window and renderer
	bool init();
	// Loads textures
	bool loadMedia();
	// Free memory
	void close();

	// For the game loop
	void events();
	void input();
	void update();
	void render();

private:
	std::string mWindowTitle;

	const uint8_t BLOCK_SIZE;
	const uint8_t ROWS;
	const uint8_t COLUMNS;
	 
	const uint16_t WINDOW_WIDTH;
	const uint16_t WINDOW_HEIGHT;


	bool mQuitWindow;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Event mEvent;


	// Timer
	ElapsedTime mET;

	// Textures
	Texture mTextureWater;
	SpriteSheet mSpritePlayer;


};

