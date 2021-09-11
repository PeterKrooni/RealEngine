#include "RenderableComponent.h"
#include <iostream>

RenderableComponent::RenderableComponent(VX_C_Window* w, RenderLayer layer)
{	
	this->mTargetWindow = w;
	this->setRenderLayer(layer);
}

RenderableComponent::RenderableComponent()
{
}

RenderableComponent::~RenderableComponent()
{
	this->mTargetWindow = NULL;
}

void RenderableComponent::loadTexture(std::string filePath)
{
	if (mTargetWindow == NULL) {
		std::cout << "Could not load texture from " << filePath << ". Target window was null." << std::endl;
	}
	else {
		this->mTexture.loadFromFile(filePath, mTargetWindow->fetchRender());
		textureW = mTexture.getWidth();
		textureH = mTexture.getHeight();
	}
}

void RenderableComponent::loadTexture(VX_C_Texture texture)
{
	this->mTexture = texture;
}

void RenderableComponent::loadTextToTexture(std::string content, SDL_Color color)
{
	if (mTargetWindow == NULL) {
		std::cout << "Could not load texture from text. Target window was null." << std::endl;
	}
	else {
		this->mTexture.loadFromText(mTargetWindow->fetchRender(), mTargetWindow->fetchFont(), content, color);
		textureW = mTexture.getWidth();
		textureH = mTexture.getHeight();
	}
}

void RenderableComponent::setRenderLayer(RenderLayer layer)
{	
	this->layer = layer;
}

RenderLayer RenderableComponent::getRenderLayer()
{
	return this->layer;
}

void RenderableComponent::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	this->mTexture.setColor(red, green, blue);
}

void RenderableComponent::setBlendMode(SDL_BlendMode blending)
{
	this->mTexture.setBlendMode(blending);
}

void RenderableComponent::setAlpha(Uint8 alpha)
{
	this->mTexture.setAlpha(alpha);
}

void RenderableComponent::setClip(SDL_Rect* clip)
{
	this->mTexture.clip = clip;
}

void RenderableComponent::setAngle(double angle)
{
	this->mTexture.angle = angle;
}

void RenderableComponent::setCenter(SDL_Point* center)
{
	this->mTexture.center = center;
}

void RenderableComponent::setFlip(SDL_RendererFlip flip)
{
	this->mTexture.flip = flip;
}

VX_C_Texture* RenderableComponent::getTexture()
{
	return &this->mTexture;
}

bool RenderableComponent::targetIsSet()
{
	return this->mTargetWindow != NULL;
}

void RenderableComponent::toggleDebug()
{
	this->debug = !this->debug;
}
