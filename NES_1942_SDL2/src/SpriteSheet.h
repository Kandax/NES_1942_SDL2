#pragma once
#include <string>
#include<iostream>

#include <SDL.h>
#include <SDL_image.h>


class SpriteSheet
{
public:
	SpriteSheet();
	~SpriteSheet();

	bool loadFromFile(SDL_Renderer* renderer, std::string path);

	void free();

	// Sets grid setting. 
	void setGridSettings(int numberOfCells, int cellWitdth, int cellHeight, int cellGapWidth = 0, int cellGapHeight = 0);

	// Render cell from texture. Index begins with 0 to number of cells - 1
	void render(SDL_Renderer* renderer, int x, int y, int index = 0);


	void setScale(float scale);


private:
	SDL_Texture* mTexture;


	int mWidth;
	int mHeight;



	int mCellWidth;
	int mCellHeight;
	int mCellGapWidth;
	int mCellGapHeight;

	int mNumberOfCells;

	float mScale;


};

