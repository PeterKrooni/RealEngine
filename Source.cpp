#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <memory>

#include "Unfiltered/Actor.h"
#include "SDL Wrappers/Window.h"
#include "SDL Wrappers/Texture.h"
#include "Map/GameMap.h"
#include "Map/MapReader.h"
#include "Entity/Entity.h"
#include "Blueprints/EntityBlueprints.h"
#include "SDL Wrappers/BackendInitializer.h"
#include "Managers/System Managers/GameManager.h"
#include "Tools/Debug/DebugEntity.h"
#include "Tools/Debug/DebugPerformance.h"
#include "Unfiltered/Ray.h"
#include "Components/RenderableComponent.h"
#include "Tools/Config/MapConfig.h"

#include <iostream>

SDL_Renderer* GLOBAL_RENDERER__ = NULL;

int main (int argc, char* args[]) {

	BackendInitializer backend;
	if (!backend.initSDL ()) {
		printf ("SDL subsystems have failed to init.");
	}
	int camx = 0;
	int camy = 0;

	int windowX = 1920;
	int windowY = 1080;

	// Window class
	VX_C_Window w;
	// Start window events
	w.init(GLOBAL_RENDERER__, "TESTING", windowX, windowY);
	// Set cam positioning
	w.setCameraTo(&camx, &camy);

	VX_C_GameManager gameManager(&w);
	gameManager.buildAutoEntities(&w, VX_C_MapReader::parseMap("Assets/Map/testmap.txt"));
	
	VX_C_Actor act;
	act.setSprite("Assets/Image/PLAYER2_STANDING2.png", &w);
	act.setPhysics(true);
	act.move(500, 10);
	act.setKeyStatePtr(SDL_GetKeyboardState(NULL));
	act.getEntity()->isActor = true;
	gameManager.addEntity(act.getEntity(), Managers::RENDER);
	gameManager.addEntity(act.getEntity(), Managers::PHYSICS);

	VX_C_Entity gr;
	gameManager.addEntity(&gr, Managers::RENDER);
	gameManager.addEntity(&gr, Managers::PHYSICS);
	gr.addRenderableComponent(&w, RenderLayer::ENTITY);
	gr.addPhysicsComponent();
	gr.loadTexture("Assets/Image/GROUND2020.png");
	gr.toggleGravity();
	gr.moveEntity(30, 30);
	gr.getPhysicsComponent()->setVelX(20);

	VX_C_Entity ar;
	gameManager.addEntity(&ar, Managers::RENDER);
	gameManager.addEntity(&ar, Managers::PHYSICS);
	ar.addRenderableComponent(&w, RenderLayer::ENTITY);
	ar.addPhysicsComponent();
	ar.loadTexture("Assets/Image/GROUND2020.png");
	ar.toggleGravity();
	ar.moveEntity(330, 30);
	ar.getPhysicsComponent()->setVelX(2);

	VX_C_Entity testTitleText;
	gameManager.addEntity(&testTitleText, Managers::RENDER);
	testTitleText.addRenderableComponent(&w, RenderLayer::FOREGROUND);
	testTitleText.moveEntity(40, 10);
	SDL_Color black;
	black.r = 255;
	black.g = 255;
	black.b = 255;
	black.a = 255;
	testTitleText.loadTextToTexture("ID " + std::to_string(act.getEntity()->getEntityID()), black);

	act.getEntity()->addChild(&testTitleText, 0, -30);

	gr.toggleVelocityTracer();
	gr.toggleHitboxOutline();
	gr.togglePhysicsRayDisplay();
	act.getEntity()->toggleVelocityTracer();
	act.getEntity()->toggleHitboxOutline();
	act.getEntity()->togglePhysicsRayDisplay();

	DebugEntity de;
	de.setEntity(act.getEntity());

	de.setPosition(40, 40);
	de.updateStore(&w);
	de.registerStore(&gameManager);

	DebugPerformance dp;
	dp.setPos(40, 10);
	dp.updateStore(&w);
	dp.registerStore(&gameManager);

	SDL_Event e;
	const Uint8* keyState = SDL_GetKeyboardState (NULL);
	bool quit = false;

	// Time given per frame in ms 
	float fps_60 = 16.6667;
	float fps_120 = 8.3333;

	bool freezeFrame = false;
	bool oneFrame = false;

	while (!quit) {
		// For calculating frame rate
		Uint32 frameStart = SDL_GetTicks ();

		while (SDL_PollEvent (&e) != 0) {

			// This causes the window to close every loop for some reason... Wonder what's causing this.
			//w.handleEvent (e);

			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_LSHIFT) {
					gameManager.buildAutoEntities(&w, VX_C_MapReader::parseMap("testmap.txt"));

				}
				else if (e.key.keysym.sym == SDLK_ESCAPE) {
					freezeFrame = !freezeFrame;
				}
				else if (e.key.keysym.sym == SDLK_RIGHT) {
					oneFrame = true;
				}
			}
			if (!freezeFrame) {
				act.handlePerActionInput(e);
			}
			//i.sendInput (e);
		}
		// This updates keyState array pointer
		SDL_PumpEvents ();

		if (!freezeFrame || oneFrame){
			act.handleContInput();

			gameManager.processFrame();

			w.render ();

			// Delay if frames are going faster than current limit
			Uint32 frameEnd = SDL_GetTicks ();
			float frameTime = ((float)frameEnd - (float)frameStart)/1000;
			dp.logFrame(frameTime);
			if (frameTime < fps_120){
				SDL_Delay (fps_120 - frameTime);
			}
			oneFrame = false;
		}
	}

	return 0;
}
