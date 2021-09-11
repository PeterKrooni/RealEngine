#include <SDL.h>
#include <string>
#include <sstream>

#include "Texture.h"


VX_C_Texture::VX_C_Texture () {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}
VX_C_Texture::~VX_C_Texture () {
	free ();
}

bool VX_C_Texture::loadFromText(SDL_Renderer* renderer, TTF_Font* font, std::string textureText, SDL_Color textColor) {
	// Get rid of preexisting texture
	free();

	// TODO: Rework this to cooperate with window renderer
	SDL_Surface* textSurfrace = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurfrace == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurfrace);
		if (mTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			mWidth = textSurfrace->w;
			mHeight = textSurfrace->h;
		}
		SDL_FreeSurface(textSurfrace);
	}
	return mTexture != NULL;
}

bool VX_C_Texture::loadFromFile (std::string path, SDL_Renderer* renderer) {
	// Get rid of preexisting texture
	free ();

	// TODO: Rework this to cooperate with window renderer


	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load (path.c_str ());

	if (loadedSurface == NULL) {
		printf ("Unable to load image %s! SDL_Image Error: %s\n", path.c_str (), IMG_GetError ());
	}
	else {
		SDL_SetColorKey (loadedSurface, SDL_TRUE, SDL_MapRGB (loadedSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface (renderer, loadedSurface);
		if (newTexture == NULL) {
			printf ("Unable to create texture from %s! SDL_Error: %s\n", path.c_str (), SDL_GetError ());
		}
		else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface (loadedSurface);
	}
	// Return success
	mTexture = newTexture;

	return mTexture != NULL;
}
void VX_C_Texture::createTexture (SDL_Renderer* renderer, Uint32 format, int access, int w, int h)
{
	mTexture = SDL_CreateTexture (renderer, format, access, w, h);
}

void VX_C_Texture::free () {
	if (mTexture != NULL) {
		SDL_DestroyTexture (mTexture);
		mWidth = 0;
		mHeight = 0;
	}
}
void VX_C_Texture::setColor (Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod (mTexture, red, green, blue);
}
void VX_C_Texture::setBlendMode (SDL_BlendMode blending) {
	SDL_SetTextureBlendMode (mTexture, blending);
}
void VX_C_Texture::setAlpha (Uint8 alpha) {
	SDL_SetTextureAlphaMod (mTexture, alpha);
}
void VX_C_Texture::render (VX_C_Window* target, int x, int y, bool isCentered, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	target->renderQueue (mTexture, renderQuad, isCentered, clip, angle, center, flip);
}
SDL_Texture* VX_C_Texture::getTexture ()
{
	return mTexture;
}
void VX_C_Texture::fuseTexture (VX_C_Window* target, VX_C_Texture textureToFuse, SDL_Rect* srcRect, SDL_Rect* dstRect)
{
	mTexture = SDL_CreateTexture (target->fetchRender(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dstRect->w, dstRect->h);
	// Set render target to this->mTexture (the hardware texture), this way when we call rendercopy the textures get render to this->mTexture and thus are combined (baked)?
	SDL_SetRenderTarget (target->fetchRender (), mTexture);

	// Render textureToFuse to hardware texture in *this
	SDL_RenderCopy (target->fetchRender (), textureToFuse.getTexture (), srcRect, dstRect);	
	
	// Reset render target to default buffer (screen)
	SDL_SetRenderTarget (target->fetchRender (), NULL);
}
int VX_C_Texture::getWidth () {
	return mWidth;
}
// André gjorde denne getter funksjonen mulig
int VX_C_Texture::getHeight () {
	return mHeight;
}