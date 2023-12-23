#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "game.h"
#include "LTexture.h"


extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 750;
const double bo_w = (double)SCREEN_HEIGHT / 10 * 8, bo_h = (double)SCREEN_HEIGHT / 10 * 8;
const double ori_x = (SCREEN_WIDTH - bo_w) / 2, ori_y = (SCREEN_HEIGHT - bo_h) / 2;
const double gr_w = bo_w / 8, gr_h = bo_h / 8;
const double l_w = 1; // line width

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;

////Starts up SDL and creates window
//bool init();
//
////Loads media
//bool loadMedia();
//
////Frees media and shuts down SDL
//void close();
//
////Loads individual image as texture
//SDL_Texture* loadTexture( std::string path );
//
//
//bool init()
//{
//	//Initialization flag
//	bool success = true;
//
//	//Initialize SDL
//	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
//	{
//		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
//		success = false;
//	}
//	else
//	{
//		//Set texture filtering to linear
//		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
//		{
//			printf( "Warning: Linear texture filtering not enabled!" );
//		}
//
//		//Create window
//		gWindow = SDL_CreateWindow( "chessboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE );
//		if( gWindow == NULL )
//		{
//			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
//			success = false;
//		}
//		else
//		{
//			//Create renderer for window
//			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
//			if( gRenderer == NULL )
//			{
//				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
//				success = false;
//			}
//			else
//			{
//				//Initialize renderer color
//				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
//
//				//Initialize PNG loading
//				int imgFlags = IMG_INIT_PNG;
//				if( !( IMG_Init( imgFlags ) & imgFlags ) )
//				{
//					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
//					success = false;
//				}
//			}
//		}
//	}
//
//	return success;
//}
//
//bool loadMedia()
//{
//	//Loading success flag
//	bool success = true;
//
//	//Nothing to load
//	return success;
//}
//
//void close()
//{
//	//Destroy window	
//	SDL_DestroyRenderer( gRenderer );
//	SDL_DestroyWindow( gWindow );
//	gWindow = NULL;
//	gRenderer = NULL;
//
//	//Quit SDL subsystems
//	IMG_Quit();
//	SDL_Quit();
//}
//
//SDL_Texture* loadTexture( std::string path )
//{
//	//The final texture
//	SDL_Texture* newTexture = NULL;
//
//	//Load image at specified path
//	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
//	if( loadedSurface == NULL )
//	{
//		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
//	}
//	else
//	{
//		//Create texture from surface pixels
//        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
//		if( newTexture == NULL )
//		{
//			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
//		}
//
//		//Get rid of old loaded surface
//		SDL_FreeSurface( loadedSurface );
//	}
//
//	return newTexture;
//}
//
//void draw_black_line(){
//	//Draw black horizontal lines
//	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
//	for(int i = 0; i < 9; i++){
//		SDL_RenderDrawLine( gRenderer, ori_x, ori_y + gr_h * i, ori_x + bo_w, ori_y + gr_h * i );	
//	}
//	
//	//Draw black vertical lines
//	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
//	for(int i = 0; i < 9; i++){
//		SDL_RenderDrawLine( gRenderer, ori_x + gr_w * i, ori_y, ori_x + gr_w * i, ori_y + bo_h);	
//	}
//}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
		//Load media
		if( !loadMedia() ){
			printf( "Failed to load media!\n" );
		}
		else{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			Board b;
//			double bo_w = SCREEN_HEIGHT / 10 * 8, bo_h = SCREEN_HEIGHT / 10 * 8;
//			double ori_x = (SCREEN_WIDTH - bo_w) / 2, ori_y = (SCREEN_HEIGHT - bo_h) / 2;
//			double gr_w = bo_w / 8, gr_h = bo_h / 8;
			bool is_downed = 0;
			bool vm[8][8] = {};
			bool clicked = 0;
			int clicked_x, clicked_y;
			
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
						SDL_Rect fillRect = { ori_x + gr_w * i + l_w, ori_y + gr_h * j + l_w, gr_w - l_w, gr_h - l_w };
						SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );		
						SDL_RenderFillRect( gRenderer, &fillRect );
					}
				}
			}
			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
			for(int i = 0; i < 9; i++){
				SDL_RenderDrawLine( gRenderer, ori_x, ori_y + gr_h * i, ori_x + bo_w, ori_y + gr_h * i );	
			}
			
			//Draw black vertical lines
			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
			for(int i = 0; i < 9; i++){
				SDL_RenderDrawLine( gRenderer, ori_x + gr_w * i, ori_y, ori_x + gr_w * i, ori_y + bo_h);	
			}
			
			//Update screen
			SDL_RenderPresent( gRenderer );
			
			while( !quit ){
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 ){
					//User requests quit
					if( e.type == SDL_QUIT ){
						quit = true;
					}
					else if(e.type == SDL_MOUSEBUTTONDOWN){
						is_downed = 1;
						clicked = 0;
						int mx, my;
						SDL_GetMouseState(&mx, &my);
						int by = (mx - ori_x) / gr_w, bx = (my - ori_y) / gr_h;
						for(int i = 0; i < 8; i++){
							for(int j = 0; j < 8; j++){
								SDL_Rect vmRect = { ori_x + gr_w * j + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
								if(i + j & 1){
									SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );
								}
								else{
									SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
								}
								SDL_RenderFillRect( gRenderer, &vmRect );
							}
						}
						if(bx >= 0 && bx < 8 && by >= 0 && by < 8){
							if(vm[bx][by]){
								b.move(clicked_x, clicked_y, bx, by);
							}
							for(int i = 0; i < 8; i++){
								for(int j = 0; j < 8; j++){
									vm[i][j] = 0;
								}
							}
							if(b.getboard()[bx][by]->getside() == b.getturn()){
								clicked = 1;
								clicked_x = bx;
								clicked_y = by;
								b.getboard()[bx][by]->valid_moves(vm, b);
								for(int i = 0; i < 8; i++){
									for(int j = 0; j < 8; j++){
										if(vm[i][j]){
											SDL_Rect vmRect = { ori_x + gr_w * j + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
											if(i + j & 1){
												SDL_SetRenderDrawColor( gRenderer, 0xDD, 0xAA, 0xDD, 0xFF );
											}
											else{
												SDL_SetRenderDrawColor( gRenderer, 0xFC, 0xDD, 0xFC, 0xFF );
											}
											SDL_RenderFillRect( gRenderer, &vmRect );
										}
									} 
								}
							}
						}
						SDL_RenderPresent( gRenderer );
					}
					else if(e.type == SDL_MOUSEBUTTONUP){
						is_downed = 0;
						int mx, my;
						SDL_GetMouseState(&mx, &my);
						int by = (mx - ori_x) / gr_w, bx = (my - ori_y) / gr_h;
						if(clicked){
							if(bx >= 0 && bx < 8 && by >= 0 && by < 8 && vm[bx][by]){
								b.move(clicked_x, clicked_y, bx, by);
								for(int i = 0; i < 8; i++){
									for(int j = 0; j < 8; j++){
										vm[i][j] = 0;
										SDL_Rect vmRect = { ori_x + gr_w * j + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
										if(i + j & 1){
											SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );
										}
										else{
											SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
										}
										SDL_RenderFillRect( gRenderer, &vmRect );
									}
								}
								SDL_RenderPresent( gRenderer );
							}
						}
					}
					else if(e.type == SDL_MOUSEMOTION && is_downed){
						int mx, my;
						SDL_GetMouseState(&mx, &my);
						// ¹Ï¤ù¸òµÛ·Æ¹« 
					}
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
