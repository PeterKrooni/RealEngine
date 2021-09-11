#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

/*
*	Texture data helper
*/
struct VX_S_TextureData {
	VX_S_TextureData (SDL_Texture* texture, SDL_Rect renderQuad, bool isCentered = false, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	~VX_S_TextureData ();

	SDL_Texture* texture;
	SDL_Rect renderQuad;
	bool isCentered;
	SDL_Rect* clip;
	double angle;
	SDL_Point* center;
	SDL_RendererFlip flip;
};

/*
*	Data helper for drawing lines
*/
struct VX_S_ShapeData {
	VX_S_ShapeData(int x1, int x2, int y1, int y2, SDL_Color col) : x1(x1), x2(x2), y1(y1), y2(y2), col(col) {}

	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	SDL_Color col;
};

/*
*	Window Wrapper
*
*	Contains renderer
*/
class VX_C_Window {
public:
	VX_C_Window ();
	~VX_C_Window ();

	// VSync enabled by default, returns false if window and renderer failed to load
	bool init (SDL_Renderer* render, const char* name, int SCREEN_WIDTH = 0, int SCREEN_HEIGHT = 0, bool VSync = true);
	void handleEvent (SDL_Event& e);
	void focus ();
	void render ();
	void renderQueue (SDL_Texture* texture, SDL_Rect renderQuad, bool isCentered = false, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	SDL_Renderer* fetchRender ();
	TTF_Font* fetchFont();
	void free ();
	int getWidth ();
	int getHeight ();
	bool hasMouseFocus ();
	bool hasKeyboardFocus ();
	bool isMinimized ();
	bool isShown ();			
	
	// TODO: Curb camera motion, or physics smoothing?
	// https://docs.google.com/document/d/1iNSQIyNpVGHeak6isbP6AHdHD50gs8MNXF1GCf08efg/pub?embedded=true
	// Point render camera to coordinate pointers
	void setCameraTo (int* x, int* y);
	// Set map size to calulate camera
	void setLevelDimensins (int x, int y);
	// Draw line at (x1, y1) -> (x2, y2) NB!! intended for debugging use
	void drawLine(int x1, int x2, int y1, int y2, SDL_Color col);
	void clearDrawLines();

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	TTF_Font* mFont = NULL;
	std::vector<VX_S_TextureData> mTextures;
	std::vector<VX_S_ShapeData> mShapes;

	int* camX = NULL;
	int* camY = NULL;
	int levelX;
	int levelY;

	int mWindowID;
	int mWidth;
	int mHeight;
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
	bool mShown;

	const int mFontPointSize = 20;
};