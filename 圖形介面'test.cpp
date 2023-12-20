#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "LTexture.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 750;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;

LTexture gTextTexture1;
LTexture gTextTexture2;

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
			double bo_w = SCREEN_HEIGHT / 10 * 8, bo_h = SCREEN_HEIGHT / 10 * 8;
			double ori_x = (SCREEN_WIDTH - bo_w) / 2, ori_y = (SCREEN_HEIGHT - bo_h) / 2;
			double gr_w = bo_w / 8, gr_h = bo_h / 8;
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

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				
				//Render yellow filled quad
//				SDL_Rect fillRect = { ori_x, ori_y, bo_w, bo_h };
//				SDL_SetRenderDrawColor( gRenderer, 255, 255, 224, 0xFF );		
//				SDL_RenderFillRect( gRenderer, &fillRect );
				for(int i = 0; i < 8; i++){
					for(int j = 0; j < 8; j++){
						if(i + j & 1){
							SDL_Rect fillRect = { ori_x + i * gr_w, ori_y + j * gr_h, gr_w, gr_h };
							SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );		
							SDL_RenderFillRect( gRenderer, &fillRect );
						}
					}
				}
				
				//Draw black horizontal lines
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				for(int i = 0; i < 9; i++){
					SDL_RenderDrawLine( gRenderer, ori_x, ori_y + i * gr_h, ori_x + bo_w, ori_y + i * gr_h );	
				}
				
				//Draw black vertical lines
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				for(int i = 0; i < 9; i++){
					SDL_RenderDrawLine( gRenderer, ori_x + i * gr_w, ori_y, ori_x + i * gr_w, ori_y + bo_h);	
				}	
				
				//type
				type(gTextTexture1, "123", {0, 0, 0}, 50, ( SCREEN_WIDTH - gTextTexture1.getWidth() ) / 2 + 100, ( SCREEN_HEIGHT - gTextTexture1.getHeight() ) / 2);
				type(gTextTexture2, "abc", {0, 0, 0}, 50, ( SCREEN_WIDTH - gTextTexture1.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture1.getHeight() ) / 2);
				
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
