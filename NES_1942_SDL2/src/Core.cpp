#include "Core.h"

Core::Core()
	: mWindowTitle("1942")
	, BLOCK_SIZE(64)
	, ROWS(15)
	, COLUMNS(15)
	, WINDOW_WIDTH(BLOCK_SIZE* ROWS)
	, WINDOW_HEIGHT(BLOCK_SIZE* COLUMNS)
	, mQuitWindow(false)
	, mWindow(nullptr)
	, mRenderer(nullptr)
	, mEvent()
	, mET(60)

{
	

}

Core::~Core()
{
	close();
}

void Core::run()
{
	if (!init()) {
		std::cout << "Failed to initialize!" << std::endl;
		std::cin.get();
	}
	else
	{
		if (!loadMedia()) {
			std::cout << "Failed to load media!" << std::endl;
			std::cin.get();
		}
		else
		{
			while (!mQuitWindow)
			{
				mET.run();
				events();
				input();
				update();
				render();
			}
		}
	}


}

bool Core::init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "Problem with initializing sdl. SDL error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else {
		mWindow = SDL_CreateWindow(
			mWindowTitle.c_str(), 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			WINDOW_WIDTH, 
			WINDOW_HEIGHT, 
			SDL_WINDOW_SHOWN
		);

		if (mWindow == nullptr) {
			std::cout << "Problem with creating window. SDL error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else {
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
			if (mRenderer == nullptr) {
				std::cout << "Renderer could not be created! SDL error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
				int imgFlag = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlag) & imgFlag)) {
					std::cout << "SDL_IMAGE could not be initialize! SDL_IMAGE error: " << IMG_GetError() << std::endl;
				}
			}
		}

	}

	return success;
}

bool Core::loadMedia()
{
	bool success = true;

	if (!mTextureWater.loadFromFile(mRenderer, "images/water_sprite.png")) {
		std::cout << "Failed to load water texture" << std::endl;
		success = false;
	}

	if (!mSpritePlayer.loadFromFile(mRenderer, "images/player_sprite.png")) {
		std::cout << "Failed to load player texture" << std::endl;
		success = false;
	}
	else
	{
		mSpritePlayer.setGridSettings(7,23, 17);
		mSpritePlayer.setScale(15);
	}

	return success;
}

void Core::close()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	mRenderer = nullptr;
	mWindow = nullptr;

	// Exit submodules
	IMG_Quit();
	SDL_Quit();

}

void Core::events()
{
	while (SDL_PollEvent(&mEvent) != 0)
	{
		if (mEvent.type == SDL_QUIT) {
			mQuitWindow = true;
		}
	}

}

void Core::input()
{
}

void Core::update()
{
}

void Core::render()
{
	std::cout << "Time used: " << mET.getTimeUsed() << "\n";
	std::cout << "Elapsed time: " << mET.getElapsedTime() << "\n";
	std::cout << "FPS: " << 1.f / mET.getTimeUsed() << "\n";




	// Clearing screen
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);



	// Background scrolling
	static int i = 0;


	for (int y = 0; y < COLUMNS + 1; y++) {
		for (int x = 0; x < ROWS; x++) {

			mTextureWater.render(mRenderer, x *BLOCK_SIZE, (y * BLOCK_SIZE) + i - BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
		}
	}

	i++;
	if (i == BLOCK_SIZE) {
		i = 0;
	}


	// Temporary player
	static int j = 0;


	mSpritePlayer.render(mRenderer, 0, 0, j);

	j++;
	if (j >= 7) {
		j = 0;
	}

	// Updating screen
	SDL_RenderPresent(mRenderer);


}
