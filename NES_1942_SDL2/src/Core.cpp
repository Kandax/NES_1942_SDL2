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
	, KEYBOARD(SDL_GetKeyboardState(nullptr))
	, mET(60)
	, showCollisingBox(false)

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
		std::cout << "Failed to load water texture\n";
		success = false;
	}

	if (!mSpritePlayer.loadFromFile(mRenderer, "images/player_sprite.png")) {
		std::cout << "Failed to load player texture \n";
		success = false;
	}
	else
	{
		mSpritePlayer.setGridSettings(mPlayer.numberOfTextures,23, 17);
		mSpritePlayer.setScale(mPlayer.scale);
	}

	if (!mTextureBulletPlayer.loadFromFile(mRenderer, "images/player_bullet_sprite.png")) {
		std::cout << "Failed to load player bullet texture \n";
		success = false;
	}
	else {
		mTextureBulletPlayer.setScale(4);
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
	if (KEYBOARD[SDL_SCANCODE_F1]) {
		showCollisingBox = true;
	}
	if (KEYBOARD[SDL_SCANCODE_F2]) {
		showCollisingBox = false;
	}




	if (KEYBOARD[SDL_SCANCODE_RIGHT]) {
		mPlayer.posX += mPlayer.playerSpeed;
	}
	if (KEYBOARD[SDL_SCANCODE_LEFT]) {
		mPlayer.posX -= mPlayer.playerSpeed;
	}

	if (KEYBOARD[SDL_SCANCODE_UP]) {
		mPlayer.posY -= mPlayer.playerSpeed;
	}
	if (KEYBOARD[SDL_SCANCODE_DOWN]) {
		mPlayer.posY += mPlayer.playerSpeed;
	}

	// Firing bullets
	if (KEYBOARD[SDL_SCANCODE_Z]) {
		isPlayerFiring = true;
	}
	else {
		isPlayerFiring = false;
	}





}

void Core::update()
{
	
	static float timerForFiringBulletPlayer = 0;
	timerForFiringBulletPlayer += 1 * mET.getElapsedTime();

	if (timerForFiringBulletPlayer >= 0.3f) {
		if (isPlayerFiring) {
			// Create bullet at player position
			mPlayerBullets.push_back(BulletPlayer(mPlayer.posX, mPlayer.posY));
			// update bullet collider box
			mPlayerBullets.back().update();
			// Center bullet in the middle of player 
			mPlayerBullets.back().posX = mPlayerBullets.back().posX + (mPlayer.width * mPlayer.scale) / 2 - mPlayerBullets.back().boundingBox.w / 2;
			// Pick bullet above player
			mPlayerBullets.back().posY = mPlayerBullets.back().posY - mPlayerBullets.back().boundingBox.h;

			

		}
			timerForFiringBulletPlayer = 0;
	}




	mPlayer.update();

	// Bullet player
	for (int i = 0; i < mPlayerBullets.size(); i++) {
		
		mPlayerBullets[i].posY -= mPlayerBullets[i].bulletSpeed;
		mPlayerBullets[i].update();

	}

	// Erase bullet if he goes over the edge
	for (int i = 0; i < mPlayerBullets.size(); i++) {

		if (mPlayerBullets[i].posY < -10) {
			mPlayerBullets.erase(mPlayerBullets.begin() + i);
		}

	}
	

}

void Core::render()
{
	//std::cout << "Time used: " << mET.getTimeUsed() << "\n";
	//std::cout << "Elapsed time: " << mET.getElapsedTime() << "\n";
	std::cout << "FPS: " << 1.f / mET.getTimeUsed() << "\n";

	//std::cout << "Player bullets: " << mPlayerBullets.size() << "\n";



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


	//  player
	mSpritePlayer.render(mRenderer, mPlayer.posX, mPlayer.posY, mPlayer.textureIndex);

	

	// Colliding box
	if (showCollisingBox) {
		SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(mRenderer, &mPlayer.boundingBox);
	}
	



	// Bullet player
	for (int i = 0; i < mPlayerBullets.size(); i++) {
		// Draw bullet for each projectiles
		for (int j = 0; j < mPlayerBullets[i].numberOfProjectiles; j++) {
			// Space between a bullets
			float bulletInterspace = j * mPlayerBullets[i].width * mPlayerBullets[i].scale * mPlayerBullets[i].projectilesOffset;
			

			// Rendering each bullet with interspace
			mTextureBulletPlayer.render(
				mRenderer, 
				mPlayerBullets[i].posX + bulletInterspace,
				mPlayerBullets[i].posY
			);

		}
		// Colliding box 
		if (showCollisingBox) {
			SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(mRenderer, &mPlayerBullets[i].boundingBox);
		}

	}

	// Updating screen
	SDL_RenderPresent(mRenderer);


}

bool Core::isColliding(const SDL_Rect& firstBoundingBox, const SDL_Rect& secondBoundingBox)
{

	// first x < second x + width && first x + width > second x
	if (firstBoundingBox.x < secondBoundingBox.x + secondBoundingBox.w && firstBoundingBox.x + firstBoundingBox.w > secondBoundingBox.x
		// first y < second y + height && first y + height > second y
		&& firstBoundingBox.y < secondBoundingBox.y + secondBoundingBox.h && firstBoundingBox.y + firstBoundingBox.h > secondBoundingBox.y) 
	{
		return true;

	}
	else {
		return false;

	}


}


