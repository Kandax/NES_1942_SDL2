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
	// Initialize random seed	
	srand(time(NULL));

	mEnemyGreys.push_back(EnemyGrey(rand() % WINDOW_WIDTH, 0));
	mEnemyGreys.back().enemyState = EnemyGrey::EnemyState::FORWARD;
	mEnemyGreys[0].posX = 300;
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
	// Loading Textures
	if (!mTextureWater.loadFromFile(mRenderer, "images/water_sprite.png")) {
		std::cout << "Failed to load water texture\n";
		success = false;
	}

	if (!mTextureBulletPlayer.loadFromFile(mRenderer, "images/player_bullet_sprite.png")) {
		std::cout << "Failed to load player bullet texture \n";
		success = false;
	}
	else {
		mTextureBulletPlayer.setScale(4);
	}
	// Loading sprite sheets
	if (!mSpritePlayer.loadFromFile(mRenderer, "images/player_sprite.png")) {
		std::cout << "Failed to load player texture \n";
		success = false;
	}
	else
	{
		mSpritePlayer.setGridSettings(mPlayer.numberOfTextures, 23, 17);
		mSpritePlayer.setScale(mPlayer.scale);
	}

	if (!mSpriteEnemyGrey.loadFromFile(mRenderer, "images/enemy_gray_spirte.png")) {
		std::cout << "Failed to load player texture \n";
		success = false;
	}
	else {
		mSpriteEnemyGrey.setGridSettings(11, 16, 17);
		mSpriteEnemyGrey.setScale(4);
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
	// Timer for firing bullets by player
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

			mPlayerBullets.back().update();
			
			timerForFiringBulletPlayer = 0;
		}
			
	}


	// Timer for respawning enemys grey
	static float tEnemy = 0;
	tEnemy += 1 * mET.getElapsedTime();

	if (false
		//tEnemy > 0.1f
		) {

		if (mEnemyGreys.size() < 1000) {
			mEnemyGreys.push_back(EnemyGrey(rand() % WINDOW_WIDTH, 0));
			mEnemyGreys.back().enemyState = EnemyGrey::EnemyState::FORWARD;

			/*mEnemyGreys.push_back(EnemyGrey(rand() % WINDOW_WIDTH, 0));
			mEnemyGreys.back().enemyState = EnemyGrey::EnemyState::FORWARD;
			mEnemyGreys.push_back(EnemyGrey(rand() % WINDOW_WIDTH, 0));
			mEnemyGreys.back().enemyState = EnemyGrey::EnemyState::FORWARD;*/

		}

		tEnemy = 0;
	}


	for (int i = 0; i < mPlayerBullets.size(); i++) {
		for (int j = 0; j < mEnemyGreys.size(); j++) {
			if (isColliding(mPlayerBullets[i].boundingBox, mEnemyGreys[j].boundingBox)) {
				mEnemyGreys.erase(mEnemyGreys.begin() + j);
			}
		}
	}








	// Update player
	mPlayer.update();

	// Update list of bullets player
	for (int i = 0; i < mPlayerBullets.size(); i++) {
		
		mPlayerBullets[i].posY -= mPlayerBullets[i].bulletSpeed;
		mPlayerBullets[i].update();

	}

	// Update list of enemy grey's
	for (int i = 0; i < mEnemyGreys.size(); i++) {

		float widthRect = 200;
		float heightRect = 300;

		SDL_Rect leftRect = {
			mEnemyGreys[i].posX - widthRect - mPlayer.boundingBox.w / 2,
			mEnemyGreys[i].posY + mEnemyGreys[i].boundingBox.h ,
			widthRect,
			heightRect
		};

		SDL_Rect rightRect = {
			mEnemyGreys[i].posX + mEnemyGreys[i].boundingBox.w + mPlayer.boundingBox.w / 2,
			mEnemyGreys[i].posY + mEnemyGreys[i].boundingBox.h ,
			widthRect,
			heightRect
		};

		float distBetweenPlayerAndEnemyY = std::sqrt(std::pow(mEnemyGreys[i].posY - mPlayer.posY, 2));
		float distBetweenPlayerAndEnemyX = std::sqrt(std::pow(mEnemyGreys[i].posX - mPlayer.posX, 2));

		std::cout << "dist between player and enemy Y: " << distBetweenPlayerAndEnemyY <<"\n";
		std::cout << "dist between player and enemy X: " << distBetweenPlayerAndEnemyX <<"\n";

		if (distBetweenPlayerAndEnemyY < 100) {
	

			//mEnemyGreys[i].enemyState = EnemyGrey::EnemyState::ATTACK_LEFT;
		}
		if (mEnemyGreys[i].enemyState == EnemyGrey::EnemyState::FORWARD) {
			if (isColliding(mPlayer.boundingBox, leftRect)) {
				mEnemyGreys[i].enemyState = EnemyGrey::EnemyState::ATTACK_LEFT;
			}
			if (isColliding(mPlayer.boundingBox, rightRect)) {
				mEnemyGreys[i].enemyState = EnemyGrey::EnemyState::ATTACK_RIGHT;
			}
		}
		


		switch (mEnemyGreys[i].enemyState)
		{
		case EnemyGrey::EnemyState::FORWARD:
			mEnemyGreys[i].posY += mEnemyGreys[i].enemySpeed;
			break;
		case EnemyGrey::EnemyState::ATTACK_LEFT:
			mEnemyGreys[i].posY += mEnemyGreys[i].enemySpeed;
			mEnemyGreys[i].posX -= mEnemyGreys[i].enemySpeed;

			break;
		case EnemyGrey::EnemyState::ATTACK_RIGHT:
			mEnemyGreys[i].posY += mEnemyGreys[i].enemySpeed;
			mEnemyGreys[i].posX += mEnemyGreys[i].enemySpeed;
			break;

		
		}

		mEnemyGreys[i].update();
	}


	// Erase bullet if he goes over the edge
	for (int i = 0; i < mPlayerBullets.size(); i++) {

		if (mPlayerBullets[i].posY < -10) {
			mPlayerBullets.erase(mPlayerBullets.begin() + i);
		}

	}
	// Erase Enemy grey if he goes over the edge
	for (int i = 0; i < mEnemyGreys.size(); i++) {
		if (mEnemyGreys[i].posY > WINDOW_HEIGHT) {
			mEnemyGreys[i].posY = 0;
			mEnemyGreys[i].posX = 200;

			//mEnemyGreys.erase(mEnemyGreys.begin() + i);
		}
	}

	


}

void Core::render()
{
	//std::cout << "Time used: " << mET.getTimeUsed() << "\n";
	//std::cout << "Elapsed time: " << mET.getElapsedTime() << "\n";
	//std::cout << "FPS: " << 1.f / mET.getTimeUsed() << "\n";

	//std::cout << "Player bullets: " << mPlayerBullets.size() << "\n";
	//std::cout << "Enemy grey's: " << mEnemyGreys.size() << "\n";


	// Clearing screen
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);



	// Background scrolling
	static int counterForBackgroundScrolling = 0;


	for (int y = 0; y < COLUMNS + 1; y++) {
		for (int x = 0; x < ROWS; x++) {

			mTextureWater.render(mRenderer, x *BLOCK_SIZE, (y * BLOCK_SIZE) + counterForBackgroundScrolling - BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
		}
	}

	counterForBackgroundScrolling++;
	if (counterForBackgroundScrolling == BLOCK_SIZE) {
		counterForBackgroundScrolling = 0;
	}


	

	

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


	// Enemy grey
	static float t = 0;
	static int j = 0;
	
	t += 1 * mET.getElapsedTime();


	mSpriteEnemyGrey.render(mRenderer, 0, 0, j);
	if (t > 1) {
		t = 0;
		j++;

		if (j >= 11) {
			j = 0;
		}
	}

	// List of Enemy grey
	for (int i = 0; i < mEnemyGreys.size(); i++) {
		mSpriteEnemyGrey.render(mRenderer, mEnemyGreys[i].posX, mEnemyGreys[i].posY, 0);

		if (showCollisingBox) {
			SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(mRenderer, &mEnemyGreys[i].boundingBox);
		}
	}

	//  player
	mSpritePlayer.render(mRenderer, mPlayer.posX, mPlayer.posY, mPlayer.textureIndex);


	float widthRect = 200;
	float heightRect = 300;

	SDL_Rect leftRect = {
		mEnemyGreys[0].posX - widthRect - mPlayer.boundingBox.w/2,
		mEnemyGreys[0].posY + mEnemyGreys[0].boundingBox.h ,
		widthRect,
		heightRect
	};
	SDL_Rect rightRect = {
			mEnemyGreys[0].posX + mEnemyGreys[0].boundingBox.w + mPlayer.boundingBox.w / 2,
			mEnemyGreys[0].posY + mEnemyGreys[0].boundingBox.h ,
			widthRect,
			heightRect
	};

	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(mRenderer, &leftRect);
	SDL_RenderDrawRect(mRenderer, &rightRect);




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


