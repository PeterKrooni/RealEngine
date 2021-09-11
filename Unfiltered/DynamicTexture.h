#pragma once
#include "Texture.h"

/*
*	Used for texture containing text that changes on a per frame basis
*	NOTE: tried using templates here but it turned out being too much of a hassle to integrate 
*		into the rest of the rendering system, so just using overloading for now. It makes for
*		messier code here but I think the overall impact on the system is lower this way.
*/

class VX_C_DynamicTexture
{
public:
	VX_C_DynamicTexture(){}
	VX_C_DynamicTexture(float* content, SDL_Color col) {
		f_contentPtr = content;
		this->color = col;
	}
	VX_C_DynamicTexture(int* content, SDL_Color col) {
		typeCheck = 2;
		i_contentPtr = content;
		this->color = col;
	}
	VX_C_DynamicTexture(bool* content, SDL_Color col) {
		typeCheck = 3;
		b_contentPtr = content;
		this->color = col;
	}
	VX_C_DynamicTexture(std::string* content, SDL_Color col) {
		typeCheck = 4;
		s_contentPtr = content;
		this->color = col;
	}
	~VX_C_DynamicTexture(){}

	void update(VX_C_Window* targetWindow) {
		switch (typeCheck) {
		case 1:
			if (f_contentPtr != NULL) {
				if (*f_contentPtr != f_content) {
					this->f_content = *f_contentPtr;
					texture.loadFromText(targetWindow->fetchRender(), targetWindow->fetchFont(), pretext + std::to_string(f_content), color);
				}
			}
			break;
		case 2:
			if (i_contentPtr != NULL) {
				if (*i_contentPtr != i_content) {
					this->i_content = *i_contentPtr;
					texture.loadFromText(targetWindow->fetchRender(), targetWindow->fetchFont(), pretext + std::to_string(i_content), color);

				}
			}
			break;
		case 3:
			if (b_contentPtr != NULL) {
				if (*b_contentPtr != b_content) {
					this->b_content = *b_contentPtr;
					texture.loadFromText(targetWindow->fetchRender(), targetWindow->fetchFont(), pretext + std::to_string(b_content), color);

				}
			}
			break;
		case 4:
			if (s_contentPtr != NULL) {
				if (*s_contentPtr != s_content) {
					this->s_content = *s_contentPtr;
					texture.loadFromText(targetWindow->fetchRender(), targetWindow->fetchFont(), pretext + s_content, color);

				}
			}
			break;
		default:
			break;
		}
	}

	// Description text before data
	void setPretext(std::string pretext){
		this->pretext = pretext;
	}

	VX_C_Texture texture;
	int x = 0;
	int y = 0;

private:
	SDL_Color color = { 255, 255, 255, 255 };

	std::string pretext = "";

	float* f_contentPtr = NULL;
	int* i_contentPtr = NULL;
	bool* b_contentPtr = NULL;
	std::string* s_contentPtr = NULL;

	float f_content = 0;
	int i_content = 0;
	bool b_content = false;
	std::string s_content = "";

	// order: float, int, bool, string
	int typeCheck = 1;
};
