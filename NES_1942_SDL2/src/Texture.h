#pragma once
#include <string>
#include<iostream>

#include <SDL.h>
#include <SDL_image.h>


class Texture
{
public:
	Texture();
	~Texture();

	bool loadFromFile(SDL_Renderer* renderer, std::string path);

	void free();

	// Render Texture with scale
	void render(SDL_Renderer* renderer, int x, int y);
	// Render texture without scale but with given width and height
	void render(SDL_Renderer* renderer, int x, int y, int width, int height);
	
	void setScale(float scale);

	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;

	
	int mWidth;
	int mHeight;

	float mScale;
	


};

