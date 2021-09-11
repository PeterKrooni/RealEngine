#include "BackendInitializer.h"

BackendInitializer::BackendInitializer()
{
}

bool BackendInitializer::initSDL()
{	/*	TODO: Settings
	*	Initializes all SDL subsystems
	*		- Video, Audio, Controller, Image, Fonts
	*	Returns false upon failing to init any subsystem
	*/

	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL video failed to initialize! %s\n", SDL_GetError());
		success = false;
	}
	else {

		// Audio
		if (SDL_Init(SDL_INIT_AUDIO) < 0) {
			printf("SDL audio failed to initialize! %s\n", SDL_GetError());
			success = false;
		}

		// Joystick 
		if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
			printf("Warning: SDL joystick (controller support) failed to initialize! %s\n", SDL_GetError());
			// Maybe not have success flag here.
		}
		/*
		else {
			if (!SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
				printf ("Warning: linear texture filtering not enabled!");
			}
			if (SDL_NumJoysticks () < 1) {
				printf ("Warning: No joysticks connected! \n");
			}
			else {
				gGameController = SDL_JoystickOpen (0);
				if (gGameController == NULL) {
					printf ("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError ());
				}
				else {
					// Get controller haptic device
					gControllerHaptic = SDL_HapticOpenFromJoystick (gGameController);
					if (gControllerHaptic) {
						printf ("Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError ());
					}
					else {
						// Get initialized rumble
						if (SDL_HapticRumbleInit (gControllerHaptic) < 0) {
							printf ("Warning: Unable to initialize rumble! SDL_Error: %s\n", SDL_GetError ());
						}
					}
				}
			}

			// Audio (SDL_Mixer)
			if (Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
				printf ("SDL_Mixer (audio library) could not initialize! SDL_Mixer Error: %s\n", Mix_GetError ());
				success = false;
			}
		}*/
		if (SDL_Init(SDL_INIT_HAPTIC) < 0) {
			printf("Warning: SDL haptics failed to initialize! %s\n", SDL_GetError());
			// Maybe not have success flag here.
		}

		// Image loading (PNG)
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) {
			printf("SDL_Image failed to initialize! %s\n", IMG_GetError());
			success = false;
		}

		// TTF (true type fonts)
		if (TTF_Init() == -1) {
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
	}

	return success;
}

