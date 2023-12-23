#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "LTexture.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 750;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;

LTexture title, description;
LTexture mode[6] = {};

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				SDL_Rect modeRect[6] = {};
				std::string mode_rule[6] = {"Classic", "3 Check", "Second King", "Card Chess", "Blindfolded", "Color Blindness"};
				double mode_w = SCREEN_WIDTH / 2, mode_h = SCREEN_HEIGHT / 9;
				double mode_orix = (SCREEN_WIDTH - mode_w) / 2, mode_oriy = SCREEN_HEIGHT / 4;
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				type(title, "Chess", {0, 0, 0}, 80, ( SCREEN_WIDTH - title.getWidth() ) / 2, SCREEN_HEIGHT / 20, 1);
				type(description, "Please choose the game mode", {0, 0, 0}, 35, ( SCREEN_WIDTH - description.getWidth() ) / 2, SCREEN_HEIGHT / 6, 1);
				for(int i = 0; i < 6; i++){
					modeRect[i] = { mode_orix, mode_oriy + mode_h * i, mode_w,  mode_h};
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderDrawRect( gRenderer, &modeRect[i] );
					type(mode[i], mode_rule[i], {0, 0, 0}, 40, ( SCREEN_WIDTH - mode[i].getWidth() ) / 2, mode_oriy + mode_h * i + mode_h / 2 - mode[i].getHeight() / 2, 1);
				}
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
