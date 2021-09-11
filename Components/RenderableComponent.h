#pragma once
#include "Texture.h"
#include "Window.h"

enum class RenderLayer {
	NONE,
	BACKGROUND,
	FOREGROUND,
	ENTITY,
	ACTOR
};

struct Animator {
	int currentFrame = 0;
	int totalFrames = 0;
	VX_C_Texture spriteSheet;
};

class RenderableComponent
{
public:
	// draw target window
	RenderableComponent(VX_C_Window* w, RenderLayer layer = RenderLayer::NONE);
	RenderableComponent();
	~RenderableComponent();

	// Initializing texture
	void loadTexture(std::string filePath);
	void loadTexture(VX_C_Texture texture);
	void loadTextToTexture(std::string content, SDL_Color color);
	void setRenderLayer(RenderLayer layer);
	RenderLayer getRenderLayer();

	// Texture handling
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	void setClip(SDL_Rect* clip);
	void setAngle(double angle);
	void setCenter(SDL_Point* center);
	void setFlip(SDL_RendererFlip flip);

	// Get texture for render
	VX_C_Texture* getTexture();
	// Set texture
	void setTexture(VX_C_Texture t) {
		this->mTexture = t;
	}

	// Check if target window is set
	bool targetIsSet();

	// Debug
	void toggleDebug();
	bool debug = false;

	// Texture components
	int textureW = 0;
	int textureH = 0;

private:
	RenderLayer layer = RenderLayer::NONE;
	VX_C_Window* mTargetWindow = NULL;
	VX_C_Texture mTexture;
};

