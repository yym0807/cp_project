#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "LTexture.h"
#include "information_page.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//Globally used font
extern TTF_Font* gFont;


void information_page(){
	Text title;
	Text mode[9];
	Text back; 
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
			mode[i].loadFromRenderedText(mode_text[i], 28);
			mode[i].render(15, SCREEN_HEIGHT / 12 + title.getHeight() + i * mode[i].getHeight() * 2);
		}
		
		back.loadFromRenderedText("Home", 30);
		double back_orix =  SCREEN_WIDTH  - back.getWidth() * 1.25, back_oriy = SCREEN_HEIGHT - back.getHeight() * 1.5;
		SDL_Rect backRect = {back_orix, back_oriy, back.getWidth(), back.getHeight()};
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderDrawRect( gRenderer, &backRect );
		back.render(back_orix, back_oriy);
		
		
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
				else if( e.type == SDL_MOUSEMOTION){
					int mx, my;
					SDL_GetMouseState(&mx, &my);
					int bx = (mx - back_orix) / back.getWidth(), by = (my - back_oriy) / back.getHeight();
					if(mx >= back_orix && bx == 0 && my >= back_oriy && by == 0){
						SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
					}
					else{
						SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					}
					SDL_RenderDrawRect( gRenderer, &backRect);
					back.render(back_orix, back_oriy);
					SDL_RenderPresent( gRenderer );
				}
				else if( e.type == SDL_MOUSEBUTTONDOWN){
					int mx, my;
					SDL_GetMouseState(&mx, &my);
					int bx = (mx - back_orix) / back.getWidth(), by = (my - back_oriy) / back.getHeight();
					if(mx >= back_orix && bx == 0 && my >= back_oriy && by == 0){
						SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
						SDL_RenderClear( gRenderer );
						return;
					}
				}
			}				
		}
	}
	//Free resources and close SDL
	close();	

	return ;
}
