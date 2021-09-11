#include "Window.h"

VX_C_Window::VX_C_Window () {}
VX_C_Window::~VX_C_Window () {}
bool VX_C_Window::init (SDL_Renderer* render, const char* name, int SCREEN_WIDTH, int SCREEN_HEIGHT, bool VSync)
{
	mWindow = SDL_CreateWindow (name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (mWindow != NULL) {
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;

		// Create renderer based on if VSync is enabled or not
		if (VSync) {
			mRenderer = SDL_CreateRenderer (mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		}
		else {
			mRenderer = SDL_CreateRenderer (mWindow, -1, SDL_RENDERER_ACCELERATED);
		}

		// Init render colour and window ID
		if (mRenderer == NULL) {
			printf ("Renderer for window could not be created! Destroying window... SDL Error: %s\n", SDL_GetError ());
			mWindow = NULL;
		}
		else {
			SDL_SetRenderDrawColor (mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			mWindowID = SDL_GetWindowID (mWindow);
			mShown = true;
		}

		// Init TTF font
		this->mFont = TTF_OpenFont("OxygenMono-Regular.ttf", this->mFontPointSize);
		if (this->mFont == NULL) {
			printf("Could not open font font.ttf, SDL Error: &s\n", SDL_GetError());
		}
	}
	else {
		printf ("Window could not be created! SDL Error: %s\n", SDL_GetError ());
	}

	return mWindow != NULL && mRenderer != NULL && this->mFont != NULL;
}
void VX_C_Window::handleEvent (SDL_Event& e)
{
	// If event was detected for this window
	if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID) {
		bool updateCaption = false;
		switch (e.window.event) {
		case SDL_WINDOWEVENT_SHOWN:
			mShown = true;
			break;

		case SDL_WINDOWEVENT_HIDDEN:
			mShown = false;
			break;

			// Get new dimensions and repaint
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			break;

		case SDL_WINDOWEVENT_EXPOSED:
			break;

		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			updateCaption = true;
			break;

		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			updateCaption = true;

			// Keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			updateCaption = true;

		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			updateCaption = true;

		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;

		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;

		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;

		case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow (mWindow);
			break;
		}
		if (updateCaption) {
			// Can update window caption on update here
		}
	}
}
void VX_C_Window::focus ()
{
	if (!mShown) {
		SDL_ShowWindow (mWindow);
	}
	SDL_RaiseWindow (mWindow);
}
void VX_C_Window::render ()
{
	if (!mMinimized) {
		// White
		SDL_SetRenderDrawColor (mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear (mRenderer);

		for (VX_S_TextureData t : mTextures) {
			// renderer, texture, *rect clip, *rect renderquad,angle, *point center, flip
			if (t.isCentered) {
				t.renderQuad.x = mWidth/2-(t.renderQuad.w);
				t.renderQuad.y = mHeight/2;
			}
			else {
				t.renderQuad.x -= *camX;
				t.renderQuad.y -= *camY;
			}
			SDL_RenderCopyEx (mRenderer, t.texture, t.clip, &t.renderQuad, t.angle, t.center, t.flip);
		}

		for (VX_S_ShapeData s : mShapes) {
			SDL_SetRenderDrawColor(mRenderer, s.col.r, s.col.g, s.col.b, s.col.a);
			SDL_RenderDrawLine(mRenderer, s.x1, s.y1, s.x2, s.y2);
		}

		SDL_RenderPresent (mRenderer);
		mTextures.clear ();
		mShapes.clear();
	}
}
void VX_C_Window::renderQueue (SDL_Texture* texture, SDL_Rect renderQuad, bool isCentered, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	VX_S_TextureData textureData (texture, renderQuad, isCentered, clip, angle, center, flip);
	mTextures.push_back (textureData);
}
SDL_Renderer* VX_C_Window::fetchRender ()
{
	if(mRenderer == NULL){
		printf ("Warning: Tried to fetch window renderer, but renderer was null.");
		return nullptr;
	}
	return mRenderer;
}
TTF_Font* VX_C_Window::fetchFont()
{
	if (mFont == NULL) {
		printf("Warning: Tried to fetch font from window, but font was null.");
		return nullptr;
	}
	return mFont;
}
void VX_C_Window::free ()
{
	mWindow = NULL;
}
int VX_C_Window::getWidth ()
{
	return mWidth;
}
int VX_C_Window::getHeight ()
{
	return mHeight;
}
bool VX_C_Window::hasMouseFocus ()
{
	return mMouseFocus;
}
bool VX_C_Window::hasKeyboardFocus ()
{
	return mKeyboardFocus;
}
bool VX_C_Window::isMinimized ()
{
	return mMinimized;
}
bool VX_C_Window::isShown ()
{
	return mShown;
}

void VX_C_Window::setCameraTo (int* x, int* y)
{
	this->camX = x;
	this->camY = y;
}

void VX_C_Window::setLevelDimensins (int x, int y)
{
	this->levelX = x;
	this->levelY = y;
}

void VX_C_Window::drawLine(int x1, int x2, int y1, int y2, SDL_Color col)
{
	VX_S_ShapeData data(x1, x2, y1, y2, col);
	mShapes.push_back(data);
}

void VX_C_Window::clearDrawLines()
{
	mShapes.clear();
}

VX_S_TextureData::VX_S_TextureData (SDL_Texture* texture, SDL_Rect renderQuad, bool isCentered, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	this->texture = texture;
	this->renderQuad = renderQuad;
	this->isCentered = isCentered;
	this->clip = clip;
	this->angle = angle;
	this->center = center;
	this->flip = flip;
}

VX_S_TextureData::~VX_S_TextureData () {
	if (this->texture != NULL) {
		//delete this->texture;
	}
	if (this->clip != NULL) {
		// Commented out cus this causes a breakpoint and valid pointer heap execption kinda thing
		//delete this->clip;
	}
	if (this->center != NULL) {
		//delete this->center;
	}
}