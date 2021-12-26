#include "SpriteSheet.h"

SpriteSheet::SpriteSheet()
	: mTexture(nullptr)
	, mWidth(0)
	, mHeight(0)
	, mCellWidth(0)
	, mCellHeight(0)
	, mCellGapWidth(0)
	, mCellGapHeight(0)
	, mNumberOfCells(1)
	, mScale(1.f)
{
}

SpriteSheet::~SpriteSheet()
{
	free();
}

bool SpriteSheet::loadFromFile(SDL_Renderer* renderer, std::string path)
{
	free();

	SDL_Texture* newTexture = nullptr;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr) {
		std::cout<<"Unable to load image: "<<path.c_str()<< " SDL_IMAGE error: " << IMG_GetError() << std::endl;
	}
	else {

		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == nullptr) {
			std::cout << "Unable to create texture from: " << path.c_str() << " SDL error: " << SDL_GetError() << std::endl;
		}
		else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
		
	}
	mTexture = newTexture;
	return mTexture != nullptr;
}

void SpriteSheet::free()
{
	if (mTexture != nullptr) {
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
		mCellWidth = 0;
		mCellHeight = 0;
		mCellGapWidth = 0;
		mCellGapHeight = 0;

		mNumberOfCells = 1;
	}

}

void SpriteSheet::setGridSettings(int numberOfCells, int cellWitdth, int cellHeight,  int cellGapWidth, int cellGapHeight)
{
	mNumberOfCells = numberOfCells;

	mCellWidth = cellWitdth;
	mCellHeight = cellHeight;
	mCellGapWidth = cellGapWidth;
	mCellGapHeight = cellGapHeight;

}

void SpriteSheet::render(SDL_Renderer* renderer, int x, int y, int index)
{
	if (index > mNumberOfCells-1) {
		index = 0;
	}


	SDL_Rect srcRect = {
		index * mCellWidth,
		0,
		mCellWidth,
		mCellHeight
	};


	SDL_Rect dstRect = {
		x,
		y,
		mCellWidth * mScale,
		mCellHeight * mScale
	};
	SDL_RenderCopy(renderer, mTexture, &srcRect, &dstRect);


}

void SpriteSheet::setScale(float scale)
{
	mScale = scale;
}
