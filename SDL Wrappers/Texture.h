#pragma once
#include <string>
#include <memory>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Window.h"



/*
*	Texture wrapper
*	TODO: https://gamedev.stackexchange.com/questions/70876/sdl2-sprite-batching-and-texture-atlases
*		^ texture/sprite batching
*/
class VX_C_Texture {
public:
	VX_C_Texture ();
	~VX_C_Texture ();

	// Load a texture with text
	bool loadFromText(SDL_Renderer* renderer, TTF_Font* font, std::string textureText, SDL_Color textColor);
	bool loadFromFile (std::string path, SDL_Renderer* renderer);
	void createTexture(SDL_Renderer* renderer, Uint32 format, int access, int w, int h);

	void free ();

	// Set color modulation
	void setColor (Uint8 red, Uint8 green, Uint8 blue);

	// Set blending
	void setBlendMode (SDL_BlendMode blending);

	// Set alpha modulation
	void setAlpha (Uint8 alpha);

	// render texture at given point
	void render (VX_C_Window* target, int x, int y, bool isCentered = false, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	// Get hardware texture
	SDL_Texture* getTexture ();

	// Bake texture in a chunk to a single texture, int w, h chunck width and height or tile width and height.. either one of these...
	void fuseTexture (VX_C_Window* target, VX_C_Texture textureToFuse, SDL_Rect* srcRect, SDL_Rect* dstRect);

	int getWidth ();
	int getHeight ();

	int x, y;
	SDL_Rect* clip;
	double angle;
	SDL_Point* center;
	SDL_RendererFlip flip;

private:

	// Actual hardware texture
	SDL_Texture* mTexture;

	// Image dimensions;
	int mWidth, mHeight;

};