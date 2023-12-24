#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "LTexture.h"
#include "classic.h"
#include "three_check.h"
#include "second_king.h"
#include "cards.h"
#include "blindfolded.h"
#include "information_page.h"
//enum {AIR, KING, QUEEN, ROOK, KNIGHT, BISHOP, PAWN};
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

Text title;
Text description;
Text mode[6] = {};
Text info;

SDL_Rect modeRect[6] = {};
SDL_Rect infoRect;
std::string mode_title[6] = {"Classic", "3 Check", "Second King", "Card Chess", "Blindfolded", "Color Blindness"};
double mode_w = SCREEN_WIDTH / 2, mode_h = SCREEN_HEIGHT / 9;
double mode_orix = (SCREEN_WIDTH - mode_w) / 2, mode_oriy = SCREEN_HEIGHT / 4;
double info_orix, info_oriy;


void reset(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );
	title.loadFromRenderedText("Chess", 80);
	title.render(( SCREEN_WIDTH  - title.getWidth()  ) / 2, SCREEN_HEIGHT / 20);
	description.loadFromRenderedText( "Please choose the game mode", 35);
	description.render(( SCREEN_WIDTH - description.getWidth()) / 2, SCREEN_HEIGHT / 6);
	for(int i = 0; i < 6; i++){
		modeRect[i] = { mode_orix, mode_oriy + mode_h * i, mode_w,  mode_h};
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderFillRect( gRenderer, &modeRect[i] );
		mode[i].loadFromRenderedText( mode_title[i], 40);
		mode[i].render(( SCREEN_WIDTH - mode[i].getWidth() ) / 2, mode_oriy + mode_h * i + mode_h / 2 - mode[i].getHeight() / 2);
	}
	info.loadFromRenderedText("Information", 30);
	info_orix =  SCREEN_WIDTH  - info.getWidth() * 1.25, info_oriy = SCREEN_HEIGHT - info.getHeight() * 2;
	infoRect = {info_orix, info_oriy, info.getWidth(), info.getHeight()};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderDrawRect( gRenderer, &infoRect );
	info.render(info_orix, info_oriy);
	SDL_RenderPresent( gRenderer );
}


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
			
			reset();
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
					else if( e.type == SDL_MOUSEMOTION){
						int mx, my;
						SDL_GetMouseState(&mx, &my);
						int bx = (mx - mode_orix) / mode_w, by = (my - mode_oriy) / mode_h;
						for(int i = 0; i < 6; i++){
							modeRect[i] = { mode_orix, mode_oriy + mode_h * i, mode_w,  mode_h};
							if(bx != 0 || by != i){
								SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
							}
							else if(mx >= mode_orix && bx == 0 && my >= mode_oriy && by >= 0 && by <= 5){
								SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
							}
							
							SDL_RenderFillRect( gRenderer, &modeRect[i] );
							mode[i].loadFromRenderedText( mode_title[i], 40);
							mode[i].render(( SCREEN_WIDTH - mode[i].getWidth() ) / 2, mode_oriy + mode_h * i + mode_h / 2 - mode[i].getHeight() / 2);
						}
						
						int cx = (mx - info_orix) / info.getWidth(), cy = (my - info_oriy) / info.getHeight();
						if(mx >= info_orix && cx == 0 && my >= info_oriy && cy == 0){
							SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
						}
						else{
							SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
						}
						SDL_RenderDrawRect( gRenderer, &infoRect);
						info.render(info_orix, info_oriy);
					}
					else if( e.type == SDL_MOUSEBUTTONDOWN){
						int mx, my;
						SDL_GetMouseState(&mx, &my);
						int bx = (mx - mode_orix) / mode_w, by = (my - mode_oriy) / mode_h;
						if(mx >= mode_orix && bx == 0 && my >= mode_oriy && by >= 0 && by <= 5){
							switch(by){
								case 0:
									classic();
//									printf("123");
									break;
								case 1:		
									three_check();
									break;
								case 2:
									second_king();
									break;
								case 3:
									cards();
									break;
								case 4:
									blindfolded();
									break;
													
							}
							
						}
						
						int cx = (mx - info_orix) / info.getWidth(), cy = (my - info_oriy) / info.getHeight();
						if(mx >= info_orix && cx == 0 && my >= info_oriy && cy == 0){
							information_page();
							
						}
						reset();
					}
				}				
				
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
