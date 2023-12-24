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


//int main( int argc, char* args[] )
void information_page(){
	Text title;
	Text mode[9];
	//std::string mode_title[6] = {"Classic: ", "3 Check: ", "Second King: ", "Card Chess: ", "Blindfolded: ", "Color Blindness: "};
	std::string mode_text[9] = {
		"Classic: Just like normal chess: Don't be checkmated.",
		"3 Check: Don't be checkmated. Also, don't be checked 3 times.",
		"Second King: Choose another piece to be your second king.",
		"Card Chess: You have several cards to help you beat your opponent:",
		"        1. Freeze: Freeze a piece of your opponent for one round.",
		"        2. Penetrate: One of your pieces can penetrate any pieces for one round.",
		"        3. Bomb: All the pieces on the 9 squares around it (include itself) disappear.",
		"Blindfolded: All you can see is a board. Play the game without seeing pieces.",
		"Color Blindness: All pieces have the same color."
	};
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
			
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( gRenderer );
			
			title.loadFromRenderedText("Information", 60);
			title.render(( SCREEN_WIDTH  - title.getWidth()  ) / 2, SCREEN_HEIGHT / 20);
			for(int i = 0; i < 9; i++){
				
				//std::string str;
				mode[i].loadFromRenderedText(mode_text[i], 28);
				mode[i].render(15, SCREEN_HEIGHT / 12 + title.getHeight() + i * mode[i].getHeight() * 2);
				//mode_rule[i].loadFromRenderedText(str, 30);
				//mode_rule[i].render(0, SCREEN_HEIGHT / 15 + mode_ti[i].getHeight() + i * mode_ti[i].getHeight() * 2);
				//"Just like normal chess: don't be checkmated!";
				//	case 1: str = "Don't be checkmated. Also, don't be checked 3 times!";
				//	case 2: str = "Choose a queen, rook, knight, or bishop to be your second king. Protect your kings!";
				//	case 3: str = "You have several cards to help you beat your opponent. ";
				//	case 4: str = "You are blindfolded!";
				//	case 5: str = "There are no more color differences between pieces.";
			}
			
			
			SDL_RenderPresent( gRenderer );
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
				
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
