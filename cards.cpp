#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "game_cards.h"
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

void classic(){
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
			bool back = false;
			//Event handler
			SDL_Event e;
			
			bool vm[8][8] = {};
			bool clicked = 0;
			int clicked_x, clicked_y;
			int pointed_x = -1, pointed_y = -1;
			bool mate = 0;
			
			//Clear screen
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( gRenderer );
			
			for(int i = 0; i < 8; i++){
				for(int j = 0; j < 8; j++){
					if(i + j & 1){
						SDL_Rect fillRect = { ori_x + gr_w * j + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
						SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );		
						SDL_RenderFillRect( gRenderer, &fillRect );
					}
				}
			}
			//Draw black horizontal lines
			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
			for(int i = 0; i < 9; i++){
				SDL_RenderDrawLine( gRenderer, ori_x, ori_y + gr_h * i, ori_x + bo_w, ori_y + gr_h * i );	
			}
			
			//Draw black vertical lines
			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
			for(int i = 0; i < 9; i++){
				SDL_RenderDrawLine( gRenderer, ori_x + gr_w * i, ori_y, ori_x + gr_w * i, ori_y + bo_h);	
			}
			
			//add a~h 1~8 
			Text alph[8], num[8];
//			LTexture alph[8], num[8];
			std::string alph_tb[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
			std::string num_tb[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
			for(int i = 0; i < 8; i++){
				alph[i].loadFromRenderedText(alph_tb[i], 30);
				alph[i].render(ori_x + gr_w * i + (gr_w - alph[i].getWidth()) / 2, ori_y + bo_h);
				num[i].loadFromRenderedText(num_tb[i], 30);
				num[i].render(ori_x - num[i].getWidth() * 3 / 2, ori_y + gr_h * (7 - i) + (gr_h - alph[i].getHeight()) / 2);
			}
			
			Board b;
			
			Text result; 
			
			//Update screen
			SDL_RenderPresent( gRenderer );
			
			while(!quit && !back && !mate){
				//Handle events on queue
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
									// promotion
									int mmx, mmy, bbx, bby, last_bbx = -1, last_written = 0;
									bool selected = 0;
									while(!selected){
										while( SDL_PollEvent( &e ) != 0 ){
											if( e.type == SDL_MOUSEBUTTONDOWN ){
												SDL_GetMouseState(&mmx, &mmy);
												bbx = (mmy - ori_y) / gr_h;
												if((mmx - ori_x) * 2 >= 17 * gr_w && (mmx - ori_x) * 2 <= 19 * gr_w && bbx - 4 * b.getturn() >= 0 && bbx - 4 * b.getturn() <= 3){
													selected = 1;
												}
											}
											else if( e.type == SDL_MOUSEMOTION ){
												SDL_GetMouseState(&mmx, &mmy);
												bbx = (mmy - ori_y) / gr_h;
												if(bbx != last_bbx || last_written != ((mmx - ori_x) * 2 >= 17 * gr_w && (mmx - ori_x) * 2 <= 19 * gr_w)){
													if(last_written){
														SDL_Rect pRect = { ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * last_bbx + l_w, gr_w - l_w, gr_h - l_w };
														SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
														SDL_RenderFillRect( gRenderer, &pRect );
														renderpm(b, last_bbx);
//															b.renderpm(last_bbx);
													}
													if((mmx - ori_x) * 2 >= 17 * gr_w && (mmx - ori_x) * 2 <= 19 * gr_w && bbx - 4 * b.getturn() >= 0 && bbx - 4 * b.getturn() <= 3){
														last_written = 1;
														SDL_Rect pRect = { ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * bbx + l_w, gr_w - l_w, gr_h - l_w };
														SDL_SetRenderDrawColor( gRenderer, 0xAA, 0xDD, 0xAA, 0xFF );
														SDL_RenderFillRect( gRenderer, &pRect );
														renderpm(b, bbx);
//															b.renderpm(bbx);
													}
													else{
														last_written = 0;
													}
													SDL_RenderPresent( gRenderer );
												}
												last_bbx = bbx;
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
								if(b.move(clicked_x, clicked_y, bx, by)){
									// promotion
									int mmx, mmy, bbx, bby, last_bbx = -1, last_written = 0;
									bool selected = 0;
									while(!selected){
										while( SDL_PollEvent( &e ) != 0 ){
											if( e.type == SDL_MOUSEBUTTONDOWN ){
												SDL_GetMouseState(&mmx, &mmy);
												bbx = (mmy - ori_y) / gr_h;
												if((mmx - ori_x) * 2 >= 17 * gr_w && (mmx - ori_x) * 2 <= 19 * gr_w && bbx - 4 * b.getturn() >= 0 && bbx - 4 * b.getturn() <= 3){
													selected = 1;
												}
											}
											else if( e.type == SDL_MOUSEMOTION ){
												SDL_GetMouseState(&mmx, &mmy);
												bbx = (mmy - ori_y) / gr_h;
												if(bbx != last_bbx || last_written != ((mmx - ori_x) * 2 >= 17 * gr_w && (mmx - ori_x) * 2 <= 19 * gr_w)){
													if(last_written){
														SDL_Rect pRect = { ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * last_bbx + l_w, gr_w - l_w, gr_h - l_w };
														SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
														SDL_RenderFillRect( gRenderer, &pRect );
														renderpm(b, last_bbx);
//															b.renderpm(last_bbx);
													}
													if((mmx - ori_x) * 2 >= 17 * gr_w && (mmx - ori_x) * 2 <= 19 * gr_w && bbx - 4 * b.getturn() >= 0 && bbx - 4 * b.getturn() <= 3){
														last_written = 1;
														SDL_Rect pRect = { ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * bbx + l_w, gr_w - l_w, gr_h - l_w };
														SDL_SetRenderDrawColor( gRenderer, 0xAA, 0xDD, 0xAA, 0xFF );
														SDL_RenderFillRect( gRenderer, &pRect );
														renderpm(b, bbx);
//															b.renderpm(bbx);
													}
													else{
														last_written = 0;
													}
													SDL_RenderPresent( gRenderer );
												}
												last_bbx = bbx;
											}
										}
									}
									b.promotion(bx, by, bbx - 4 * b.getturn());
								}
								clicked = 0;
								for(int i = 0; i < 8; i++){
									for(int j = 0; j < 8; j++){
										vm[i][j] = 0;
									}
								}
								if(b.checkmate() || b.stalemate()) mate = 1;
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
						if(bx != pointed_x || by != pointed_y){
							if(bx >= 0 && bx < 8 && by >= 0 && by < 8){
								SDL_Rect cRect = { ori_x + gr_w * by + l_w, ori_y + gr_h * bx + l_w, gr_w - l_w, gr_h - l_w };
								SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x99, 0x99, 0xFF );
								SDL_RenderFillRect( gRenderer, &cRect );
								b.getboard()[bx][by]->rerender();
							}
							if(pointed_x >= 0 && pointed_x < 8 && pointed_y >= 0 && pointed_y < 8){
								SDL_Rect cRect = { ori_x + gr_w * pointed_y + l_w, ori_y + gr_h * pointed_x + l_w, gr_w - l_w, gr_h - l_w };
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
								b.getboard()[pointed_x][pointed_y]->rerender();
							}
							SDL_RenderPresent( gRenderer );
							pointed_x = bx;
							pointed_y = by;
						}
					}
				}
			}
			if(b.checkmate()){
//					const char side[2][6] = {"white", "black"};
//					printf("Winner is %s\n", side[!b.getturn()]);
				const std::string side[2] = {"white", "black"};
				const std::string side_a[2] = {"w", "b"};
				result.loadFromRenderedText("Winner is " + side[!b.getturn()], 35);
				result.render((SCREEN_WIDTH - result.getWidth()) / 2, SCREEN_HEIGHT / 20 - result.getHeight() / 2);				
				int x, y;
				for(x = 0; x < 8; x++){//找到國王位置 
					for(y = 0; y < 8; y++){
						if(*b.getboard()[x][y] == KING && b.getboard()[x][y]->getside() == !b.getturn()) break;
					}
					if(y < 8) break;
				}
				b.getboard()[x][y]->loadResultImage("img/king_" + side_a[!b.getturn()] + "_win.png");
				b.getboard()[x][y]->rerender();
				for(x = 0; x < 8; x++){//找到國王位置 
					for(y = 0; y < 8; y++){
						if(*b.getboard()[x][y] == KING && b.getboard()[x][y]->getside() == b.getturn()) break;
					}
					if(y < 8) break;
				}
				b.getboard()[x][y]->loadResultImage("img/king_" + side_a[b.getturn()] + "_lose.png");
				b.getboard()[x][y]->rerender();
				SDL_RenderPresent( gRenderer );	
			}
			else if(b.stalemate()){
//					printf("It's a stalemate\n");
				result.loadFromRenderedText("It's a draw", 35);
				result.render((SCREEN_WIDTH - result.getWidth()) / 2, SCREEN_HEIGHT / 20 - result.getHeight() / 2);
				SDL_RenderPresent( gRenderer );
			}
			while(!quit && !back){
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 ){
					//User requests quit
					if( e.type == SDL_QUIT ){
						quit = true;
					}
					// detect back
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
