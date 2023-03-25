//Using SDL and standard IO
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <random>
#include <string> 
#include "PhysicsEngine.h"
#include "ElementStore.h"
#include "FQT2.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
int main(int argc, char* args[])
{

    //The window we'll be rendering to
    SDL_Window* window = NULL;

    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    SDL_Renderer* renderer = NULL;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    //Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            screenSurface = SDL_GetWindowSurface(window);

            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            int qtWidth = 800;
            int qtHeight = 800;

            TTF_Font* SourceCodePro = TTF_OpenFont("SourceCodePro-VariableFont_wght.ttf", 24);
            SDL_Color DarkWhite = { 200, 200, 200 };
            
            Rectangle qtDimension = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            PhysicsEngine PE;

            PE.addStaticBody(qtDimension.x, qtDimension.y, qtDimension.w, 3);
            PE.addStaticBody(qtDimension.x + qtDimension.w, qtDimension.y, 3, qtDimension.h+3);
            PE.addStaticBody(qtDimension.x, qtDimension.y + qtDimension.h, qtDimension.w, 3);
            PE.addStaticBody(qtDimension.x, qtDimension.y, 3, qtDimension.h);

            //QuadTree quadTree(qtDimension, 4);

            for (int i = 0; i < 1000; i++) {

                std::random_device rd; // obtain a random number from hardware
                std::mt19937 gen(rd()); // seed the generator
                std::uniform_int_distribution<> distrX(qtDimension.x+10, (qtDimension.x) + (qtDimension.w-10)); // define the range
                std::uniform_int_distribution<> distrY(qtDimension.y+10, (qtDimension.y) + (qtDimension.h-10)); // define the range
                std::uniform_int_distribution<> distrX_vel(1, 6); // define the range
                std::uniform_int_distribution<> distrY_vel(1, 2); // define the range
                std::uniform_int_distribution<> posX(qtDimension.x + 10, qtDimension.x + 400);
                std::uniform_int_distribution<> posY(qtDimension.y + 10, qtDimension.y + 400);
                std::uniform_int_distribution<> dim(2, 4);
                int dimm = dim(gen);
                //UserData ud = { distrX(gen), distrY(gen), 5, 5, -1 };
                //PE.addRigidBody(distrX(gen), distrY(gen), distrX_vel(gen), distrY_vel(gen), 5, 5, 5);
                PE.addRigidBody(posX(gen), posY(gen), distrX_vel(gen), distrY_vel(gen), dimm, dimm, 2);
            }

            SDL_Event e;
            const Uint8* keyState = SDL_GetKeyboardState(NULL);

            bool mousePressed = false;
            int mousex, mousey;

            std::vector<Point> pts;
            /*
            FQT::AABB qtBounds = { 100, 100, 900, 900 };
            for (int i = 0; i < 100; i++) {

                std::random_device rd; // obtain a random number from hardware
                std::mt19937 gen(rd()); // seed the generator
                std::uniform_int_distribution<> distrX(qtBounds.l + 10, (qtBounds.l) + (qtBounds.r - 10)); // define the range
                std::uniform_int_distribution<> distrY(qtBounds.t + 10, (qtBounds.t) + (qtBounds.b - 10)); // define the range
                std::uniform_int_distribution<> distrX_vel(2, 4); // define the range
                std::uniform_int_distribution<> distrY_vel(2, 3); // define the range
                std::uniform_int_distribution<> posX(qtBounds.l + 10, qtBounds.l + qtBounds.r);
                std::uniform_int_distribution<> posY(qtBounds.t + 10, qtBounds.t + qtBounds.b);
                std::uniform_int_distribution<> dim(2, 2);
                //UserData ud = { distrX(gen), distrY(gen), 5, 5, -1 };
                //PE.addRigidBody(distrX(gen), distrY(gen), distrX_vel(gen), distrY_vel(gen), 5, 5, 5);

                pts.push_back(Point{ posX(gen), posY(gen), i });

                //PE.addRigidBody(posX(gen), posY(gen), distrX_vel(gen), distrY_vel(gen), dim(gen), dim(gen), 2);
            }*/

            int xw = 0; int yw = 0;
            int second = 1000;
            int framesPerSecond = 0;
            int frameCounter = 0;
            Uint32 fpsTimer = SDL_GetTicks();

            bool quit = false;
            while (!quit) {
                
                Uint32 start = SDL_GetTicks();
                if (start - fpsTimer > second) {
                    fpsTimer = SDL_GetTicks();
                    framesPerSecond = frameCounter;
                    frameCounter = 0;
                }
                frameCounter++;

                FQT2 fqt({ qtDimension.x, qtDimension.y, qtDimension.h, qtDimension.w });

                for (RigidBody r : PE.getBodies()) {
                    Store::Element e{ Store::Point{ (int)r.pos_x, (int)r.pos_y, }, r.id };
                    fqt.add(e);
                }
                /*
                for (int i = 0; i < 1115; i++) {

                    std::random_device rd; // obtain a random number from hardware
                    std::mt19937 gen(rd()); // seed the generator
                    std::uniform_int_distribution<> distx(1, 999); // define the 
                    std::uniform_int_distribution<> disty(1, 999); // define the range
                    Store::Element e{ distx(gen), disty(gen), 2 };
                    fqt.add(e);
                }
                */
                //QuadTree quadTree = QuadTree(qtDimension, 4);

                /*
                for (RigidBody b : PE.getBodies()) {
                    Point p = { b.pos_x, b.pos_y, b.id };
                    quadTree.insert(p);
                }*/

                /*
                FQT::QuadTree qt(qtBounds, 4);

                for (Point p : pts) {
                    qt.insert(p.x, p.y, p.body_id);
                }*/


                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    if (e.type == SDL_MOUSEMOTION) {
                        SDL_GetMouseState(&xw, &yw);
                        //PE.addRigidBody(xw, yw, 1, 1, 10, 10, 5);
                    }
                }
                // clear render & set color
                SDL_SetRenderDrawColor(renderer, 34, 34, 34, 255);
                SDL_RenderClear(renderer);

                SDL_Rect queryRect;
                queryRect.x = xw - 100;
                queryRect.y = yw - 100;
                queryRect.w = 200;
                queryRect.h = 200;
                std::vector<int> ids = fqt.query({ queryRect.y, queryRect.x, queryRect.h, queryRect.w }, NULL, true);
               

                SDL_SetRenderDrawColor(renderer, 34, 234, 34, 255);
                SDL_RenderDrawRect(renderer, &queryRect);

                fqt.show(renderer, NULL, true);
                PE.drawEntities(renderer);

                PE.update(fqt);
                PE.drawByExtId(renderer, ids);
                // render rigid bodies
                /*
                for (RigidBody rb : PE.getBodies()) {
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                    SDL_Rect renderableRigidBody = { rb.pos_x, rb.pos_y, rb.w, rb.h };
                    SDL_RenderFillRect(renderer, &renderableRigidBody);
                }*/

                SDL_Surface* debugMessage = TTF_RenderText_Solid(SourceCodePro, std::to_string(framesPerSecond).c_str(), DarkWhite);
                SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, debugMessage);
                SDL_Rect message_rect = { (SCREEN_WIDTH) - 80, 40, 40, 40};
                SDL_RenderCopy(renderer, messageTexture, NULL, &message_rect);
                
                // update render
                SDL_RenderPresent(renderer);
                Uint32 end = SDL_GetTicks();
                if (end - start < 16.6666) {
                    SDL_Delay(16.666 - (end - start));
                }

                SDL_FreeSurface(debugMessage);
                SDL_DestroyTexture(messageTexture);
            }
        }
    }//Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}