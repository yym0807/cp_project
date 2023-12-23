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

void classic()
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
//			double bo_w = SCREEN_HEIGHT / 10 * 8, bo_h = SCREEN_HEIGHT / 10 * 8;
//			double ori_x = (SCREEN_WIDTH - bo_w) / 2, ori_y = (SCREEN_HEIGHT - bo_h) / 2;
//			double gr_w = bo_w / 8, gr_h = bo_h / 8;
			bool vm[8][8] = {};
			bool clicked = 0;
			int clicked_x, clicked_y;
			int pointed_x = -1, pointed_y = -1;
			bool mate = 0;
			bool showwinner = 1;
			
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
						SDL_Rect fillRect = { ori_x + gr_w * j + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
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
			
			Board b;
			//Update screen
			SDL_RenderPresent( gRenderer );
			
			while(!quit){
				//Handle events on queue
				while(!mate){
					while( SDL_PollEvent( &e ) != 0 ){
						//User requests quit
						if( e.type == SDL_QUIT ){
							quit = true;
						}
						else if(e.type == SDL_MOUSEBUTTONDOWN){
							clicked = 0;
							int mx, my;
							SDL_GetMouseState(&mx, &my);
							int by = (mx - ori_x) / gr_w, bx = (my - ori_y) / gr_h;
							if(mx < ori_x) by--;
							if(my < ori_y) bx--;
//							printf("%d %d\n", bx, by);
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
									if(b.move(clicked_x, clicked_y, bx, by)){
										int mmx, mmy, bbx, bby;
										bool selected = 0;
										while(!selected){
											while( SDL_PollEvent( &e ) != 0 ){
												if( e.type == SDL_MOUSEBUTTONDOWN ){
													SDL_GetMouseState(&mmx, &mmy);
													bby = (mmx - ori_x) / gr_w;
													bbx = (mmy - ori_y) / gr_h;
													if(bby == 8 && bbx - 4 * b.getturn() >= 0 && bbx - 4 * b.getturn() <= 3){
														selected = 1;
													}
												}
											}
										}
										b.promotion(bx, by, bbx - 4 * b.getturn());
									}
									if(b.checkmate() || b.stalemate()) mate = 1;
									for(int i = 0; i < 8; i++){
										for(int j = 0; j < 8; j++){
											vm[i][j] = 0;
										}
									}
								}
								else if(b.getboard()[bx][by]->getside() == b.getturn()){
									for(int i = 0; i < 8; i++){
										for(int j = 0; j < 8; j++){
											vm[i][j] = 0;
										}
									}
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
							if(clicked){
								SDL_Rect cRect = { ori_x + gr_w * clicked_y + l_w, ori_y + gr_h * clicked_x + l_w, gr_w - l_w, gr_h - l_w };
								SDL_SetRenderDrawColor( gRenderer, 0xAA, 0xDD, 0xAA, 0xFF );
								SDL_RenderFillRect( gRenderer, &cRect );
							}
							b.renderpieces();
							SDL_RenderPresent( gRenderer );
						}
						else if(e.type == SDL_MOUSEBUTTONUP){
							int mx, my;
							SDL_GetMouseState(&mx, &my);
							int by = (mx - ori_x) / gr_w, bx = (my - ori_y) / gr_h;
							if(mx < ori_x) by--;
							if(my < ori_y) bx--;
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
									b.renderpieces();
									SDL_RenderPresent( gRenderer );
								}
							}
						}
						else if(e.type == SDL_MOUSEMOTION){
							int mx, my;
							SDL_GetMouseState(&mx, &my);
							int by = (mx - ori_x) / gr_w, bx = (my - ori_y) / gr_h;
							if(mx < ori_x) by--;
							if(my < ori_y) bx--;
							if(bx >= 0 && bx < 8 && by >= 0 && by < 8 && (bx != pointed_x || by != pointed_y)){
								SDL_Rect cRect = { ori_x + gr_w * pointed_y + l_w, ori_y + gr_h * pointed_x + l_w, gr_w - l_w, gr_h - l_w };
								if(pointed_x >= 0 && pointed_x < 8 && pointed_y >= 0 && pointed_y < 8){
									if(clicked && pointed_x == clicked_x && pointed_y == clicked_y) SDL_SetRenderDrawColor( gRenderer, 0xAA, 0xDD, 0xAA, 0xFF );
									else if(pointed_x + pointed_y & 1){
										if(vm[pointed_x][pointed_y]) SDL_SetRenderDrawColor( gRenderer, 0xDD, 0xAA, 0xDD, 0xFF );
										else SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );
									}
									else{
										if(vm[pointed_x][pointed_y]) SDL_SetRenderDrawColor( gRenderer, 0xFC, 0xDD, 0xFC, 0xFF );
										else SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
									}
									SDL_RenderFillRect( gRenderer, &cRect );
								}
								
								cRect = { ori_x + gr_w * by + l_w, ori_y + gr_h * bx + l_w, gr_w - l_w, gr_h - l_w };
								if(bx + by & 1) SDL_SetRenderDrawColor( gRenderer, 0xDD, 0xAA, 0xAA, 0xFF );
								else SDL_SetRenderDrawColor( gRenderer, 0xFC, 0xCC, 0xCC, 0xFF );
								SDL_RenderFillRect( gRenderer, &cRect );
								if(pointed_x >= 0 && pointed_y >= 0 && pointed_x < 8 && pointed_y < 8) b.getboard()[pointed_x][pointed_y]->rerender();
								b.getboard()[bx][by]->rerender();
								SDL_RenderPresent( gRenderer );
								pointed_x = bx;
								pointed_y = by;
							}
						}
	//					else if(e.type == SDL_MOUSEMOTION && is_downed){
	//						int mx, my;
	//						SDL_GetMouseState(&mx, &my);
	//						b.getboard()[clicked_x][clicked_y]->renderxy(mx, my);
	//						SDL_RenderPresent( gRenderer );
	//						// ¹Ï¤ù¸òµÛ·Æ¹« 
	//					}
					}
				}
				if(showwinner && b.checkmate()){
					showwinner = 0;
					const char side[2][6] = {"white", "black"};
					printf("Winner is %s\n", side[!b.getturn()]);
				}
				else if(showwinner && b.stalemate()){
					showwinner = 0;
					printf("It's a stalemate\n");
				}
			}
		}
	}

	//Free resources and close SDL
	close();
}

int main( int argc, char* args[] ){
	classic();
	return 0;
}
