#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "game_all.h"
#include "Ltexture.h" 
using namespace std;

const int dirs[8][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1}}; // URDLUrDrDlUl
const int h[8][2] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
const char side[2][6] = {"white", "black"};
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern SDL_Renderer* gRenderer;
extern int choosing;
int choosing = NONE;
const double bo_w = (double)SCREEN_HEIGHT / 10 * 8, bo_h = (double)SCREEN_HEIGHT / 10 * 8;
const double ori_x = (SCREEN_HEIGHT - bo_w) / 2, ori_y = (SCREEN_HEIGHT - bo_h) / 2;
const double gr_w = bo_w / 8, gr_h = bo_h / 8;
const double l_w = 1; // line width

// class Piece
Piece::Piece(int xi, int yi, int pi, int si): x(xi), y(yi), p(pi), side(si), freeze(0){
	string str;
	string fs[2] = {"w.png", "b.png"};
	switch(pi){
		case KING:
			str = "img/king_" + fs[si];
			break;
		case QUEEN:
			str = "img/queen_" + fs[si];
			break;
		case ROOK:
			str = "img/rook_" + fs[si];
			break;
		case BISHOP:
			str = "img/bishop_" + fs[si];
			break;
		case KNIGHT:
			str = "img/knight_" + fs[si];
			break;
		case PAWN:
			str = "img/pawn_" + fs[si];
			break;
	}
	if(pi != AIR){
		img.loadFromFile(str);
		SDL_Rect pRect = { ori_x + gr_w * yi + l_w, ori_y + gr_h * xi + l_w, gr_w - l_w, gr_h - l_w };
		img.render(ori_x + gr_w * yi + l_w, ori_y + gr_h * xi + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
	}
}

Piece::~Piece(){}

bool Piece::operator==(int c){
	return (p == c);
}

void Piece::setxy(int xf, int yf){
	x = xf;
	y = yf;
}

int Piece::getside(){
	return side;
}

void Piece::rerender(){
	if(p != AIR){
		SDL_Rect pRect = { ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, gr_w - l_w, gr_h - l_w };
		img.render(ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
	}
}

void Piece::reloadImage(std::string str){
	img.loadFromFile(str);
}


bool Piece::getfreeze(){
	return freeze;
}
bool Piece::freezed(){
	string str;
	string fs[2] = {"w_frozen.png", "b_frozen.png"};
	switch(p){
		case KING:
			str = "img/king_" + fs[side];
			break;
		case QUEEN:
			str = "img/queen_" + fs[side];
			break;
		case ROOK:
			str = "img/rook_" + fs[side];
			break;
		case BISHOP:
			str = "img/bishop_" + fs[side];
			break;
		case KNIGHT:
			str = "img/knight_" + fs[side];
			break;
		case PAWN:
			str = "img/pawn_" + fs[side];
			break;
	}
	if(p != AIR){
		img.loadFromFile(str);
		SDL_Rect pRect = { ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, gr_w - l_w, gr_h - l_w };
		img.render(ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
	}
	freeze = 1;
}
bool Piece::unfreezed(){
	if(freeze){
		freeze = 0;
		string str;
		string fs[2] = {"w.png", "b.png"};
		switch(p){
			case KING:
				str = "img/king_" + fs[side];
				break;
			case QUEEN:
				str = "img/queen_" + fs[side];
				break;
			case ROOK:
				str = "img/rook_" + fs[side];
				break;
			case BISHOP:
				str = "img/bishop_" + fs[side];
				break;
			case KNIGHT:
				str = "img/knight_" + fs[side];
				break;
			case PAWN:
				str = "img/pawn_" + fs[side];
				break;
		}
		if(p != AIR){
			img.loadFromFile(str);
			SDL_Rect pRect = { ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, gr_w - l_w, gr_h - l_w };
			img.render(ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
		}
	}
	
}

King::King(int xi, int yi, int si): Piece(xi, yi, KING, si){}

bool King::valid_moves(bool vm[][8], Board &b, bool consider_check){
	if(freeze) return 0;
	Piece*** board = b.getboard();
	bool* oo = b.getoo();
	bool* ooo = b.getooo();
	bool flag = 0;
	for(int i = 0; i < 8; i++){
		if(x+dirs[i][0] >= 0 && x+dirs[i][0] < 8 && y+dirs[i][1] >= 0 && y+dirs[i][1] < 8 && board[x+dirs[i][0]][y+dirs[i][1]]->getside() != side && !(consider_check && b.check_if_move(x, y, x+dirs[i][0], y+dirs[i][1]))){
			flag = 1;
			vm[x+dirs[i][0]][y+dirs[i][1]] = 1;
		}
	}
	if(ooo[side]){
		if(!b.checked(side) && *board[x][1] == AIR && *board[x][2] == AIR && !(consider_check && b.check_if_move(x, 4, x, 2)) && *board[x][3] == AIR && !(consider_check && b.check_if_move(x, 4, x, 3))){
			flag = 1;
			vm[x][2] = 1;
		}
	}
	if(oo[side]){
		if(!b.checked(side) && *board[x][5] == AIR && !(consider_check && b.check_if_move(x, 4, x, 5)) && *board[x][6] == AIR && !(consider_check && b.check_if_move(x, 4, x, 6))){
			flag = 1;
			vm[x][6] = 1;
		}
	}
	return flag;
}

//loadResultImage
void King::loadResultImage(std::string str){
	img.loadFromFile(str);
}

Queen::Queen(int xi, int yi, int si): Piece(xi, yi, QUEEN, si){}

bool Queen::valid_moves(bool vm[][8], Board &b, bool consider_check){
	if(freeze) return 0;
	Piece*** board = b.getboard();
	bool flag = 0;
	for(int i = 0; i < 8; i++){
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == AIR){
				if(!(consider_check && b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t))){
					flag = 1;
					vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
				}
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t]->getside() != side && !(consider_check && b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t))){
				flag = 1;
				vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
			}
			if(choosing != PENETRATE) break;
			else t++;
		}
	}
	return flag;
}

Rook::Rook(int xi, int yi, int si): Piece(xi, yi, ROOK, si){}

bool Rook::valid_moves(bool vm[][8], Board &b, bool consider_check){
	if(freeze) return 0;
	Piece*** board = b.getboard();
	bool flag = 0;
	for(int i = 0; i < 4; i++){
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == AIR){
				if(!(consider_check && b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t))){
					flag = 1;
					vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
				}
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t]->getside() != side && !(consider_check && b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t))){
				flag = 1;
				vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
			}
			if(choosing != PENETRATE) break;
			else t++;
		}
	}
	return flag;
}

Knight::Knight(int xi, int yi, int si): Piece(xi, yi, KNIGHT, si){}

bool Knight::valid_moves(bool vm[][8], Board &b, bool consider_check){
	if(freeze) return 0;
	Piece*** board = b.getboard();
	bool flag = 0;
	for(int i = 0; i < 8; i++){
		if(x+h[i][0] >= 0 && x+h[i][0] < 8 && y+h[i][1] >= 0 && y+h[i][1] < 8 && board[x+h[i][0]][y+h[i][1]]->getside() != side && !(consider_check && b.check_if_move(x, y, x+h[i][0], y+h[i][1]))){
			flag = 1;
			vm[x+h[i][0]][y+h[i][1]] = 1;
		}
	}
	return flag;
}

Bishop::Bishop(int xi, int yi, int si): Piece(xi, yi, BISHOP, si){}

bool Bishop::valid_moves(bool vm[][8], Board &b, bool consider_check){
	if(freeze) return 0;
	Piece*** board = b.getboard();
	bool flag = 0;
	for(int i = 4; i < 8; i++){
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == AIR){
				if(!(consider_check && b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t))){
					flag = 1;
					vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
				}
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t]->getside() != side && !(consider_check && b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t))){
				flag = 1;
				vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
			}
			if(choosing != PENETRATE) break;
			else t++;
		}
	}
	return flag;
}

Pawn::Pawn(int xi, int yi, int si): Piece(xi, yi, PAWN, si){}

bool Pawn::valid_moves(bool vm[][8], Board &b, bool consider_check){
	if(freeze) return 0;
	Piece*** board = b.getboard();
	int passant = b.getpassant();
	bool flag = 0;
	if(side){
		if(*board[x+1][y] == AIR){
			if(!(consider_check && b.check_if_move(x, y, x+1, y))){
				flag = 1;
				vm[x+1][y] = 1;
			}
			if(x == 1 && *board[x+2][y] == AIR && !(consider_check && b.check_if_move(x, y, x+2, y))){
				flag = 1;
				vm[x+2][y] = 1;
			}
		}
		if(choosing == PENETRATE && x == 1 && *board[x+2][y] == AIR && !(consider_check && b.check_if_move(x, y, x+2, y))){
			flag = 1;
			vm[x+2][y] = 1;
		}
		if(y < 7 && board[x+1][y+1]->getside() == 0 && !(consider_check && b.check_if_move(x, y, x+1, y+1))){
			flag = 1;
			vm[x+1][y+1] = 1;
		}
		if(y > 0 && board[x+1][y-1]->getside() == 0 && !(consider_check && b.check_if_move(x, y, x+1, y-1))){
			flag = 1;
			vm[x+1][y-1] = 1;
		}
		if(x == 4 && (passant == y + 1 || passant == y - 1) && !(consider_check && b.check_if_move(x, y, x+1, passant))){
			flag = 1;
			vm[x+1][passant] = 1;
		}
	}
	else{
		if(*board[x-1][y] == AIR){
			if(!(consider_check && b.check_if_move(x, y, x-1, y))){
				flag = 1;
				vm[x-1][y] = 1;
			}
			if(x == 6 && *board[x-2][y] == AIR && !(consider_check && b.check_if_move(x, y, x-2, y))){
				flag = 1;
				vm[x-2][y] = 1;
			}
		}
		if(choosing == PENETRATE && x == 6 && *board[x-2][y] == AIR && !(consider_check && b.check_if_move(x, y, x-2, y))){
			flag = 1;
			vm[x-2][y] = 1;
		}
		if(y < 7 && board[x-1][y+1]->getside() == 1 && !(consider_check && b.check_if_move(x, y, x-1, y+1))){
			flag = 1;
			vm[x-1][y+1] = 1;
		}
		if(y > 0 && board[x-1][y-1]->getside() == 1 && !(consider_check && b.check_if_move(x, y, x-1, y-1))){
			flag = 1;
			vm[x-1][y-1] = 1;
		}
		if(x == 3 && (passant == y + 1 || passant == y - 1) && !(consider_check && b.check_if_move(x, y, x-1, passant))){
			flag = 1;
			vm[x-1][passant] = 1;
		}
	}
	return flag;
}

Air::Air(int xi, int yi): Piece(xi, yi, AIR, -1){}

bool Air::valid_moves(bool vm[][8], Board &b, bool consider_check){
	return 0;
}

// class Board
Board::Board(){
	choosing = NONE;
	oo[0] = 1;
	oo[1] = 1;
	ooo[0] = 1;
	ooo[1] = 1;
	turn = 0;
	passant = -2;
	board = new Piece** [8];
	for(int i = 0; i < 8; i++){
		board[i] = new Piece* [8];
	}
	board[0][4] = new King(0, 4, 1);
	board[7][4] = new King(7, 4, 0);
	board[0][0] = new Rook(0, 0, 1);
	board[0][1] = new Knight(0, 1, 1);
	board[0][2] = new Bishop(0, 2, 1);
	board[0][3] = new Queen(0, 3, 1);
	board[0][5] = new Bishop(0, 5, 1);
	board[0][6] = new Knight(0, 6, 1);
	board[0][7] = new Rook(0, 7, 1);
	board[7][0] = new Rook(7, 0, 0);
	board[7][1] = new Knight(7, 1, 0);
	board[7][2] = new Bishop(7, 2, 0);
	board[7][3] = new Queen(7, 3, 0);
	board[7][5] = new Bishop(7, 5, 0);
	board[7][6] = new Knight(7, 6, 0);
	board[7][7] = new Rook(7, 7, 0);
	for(int i = 0; i < 8; i++){
		board[1][i] = new Pawn(1, i, 1);
		board[6][i] = new Pawn(6, i, 0);
		for(int j = 2; j < 6; j++){
			board[j][i] = new Air(j, i);
		}
	}
	pm[0].loadFromFile("img/queen_w.png");
	pm[1].loadFromFile("img/rook_w.png");
	pm[2].loadFromFile("img/knight_w.png");
	pm[3].loadFromFile("img/bishop_w.png");
	pm[4].loadFromFile("img/queen_b.png");
	pm[5].loadFromFile("img/rook_b.png");
	pm[6].loadFromFile("img/knight_b.png");
	pm[7].loadFromFile("img/bishop_b.png");
}

Board::~Board(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			delete board[i][j];
		}
		delete [] board[i];
	}
	delete [] board;
}

Piece***& Board::getboard(){
	return board;
}

void Board::unloadimage(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			board[i][j]->img.free();
		}
	}
}


bool Board::move(int xi, int yi, int xf, int yf){ // return 1 if promotion
	if(*board[xi][yi] == KING && (yi-yf) * (yi-yf) > 1){ // OO or OOO
		if(yf == 6){ // OO
			board[7-7*turn][4]->setxy(7-7*turn, 6);
			board[7-7*turn][7]->setxy(7-7*turn, 5);
			delete board[7-7*turn][6];
			delete board[7-7*turn][5];
			board[7-7*turn][6] = board[7-7*turn][4];
			board[7-7*turn][5] = board[7-7*turn][7];
			board[7-7*turn][4] = new Air(7-7*turn, 4);
			board[7-7*turn][7] = new Air(7-7*turn, 7);
		}
		else if(yf == 2){ // OOO
			board[7-7*turn][4]->setxy(7-7*turn, 2);
			board[7-7*turn][0]->setxy(7-7*turn, 3);
			delete board[7-7*turn][2];
			delete board[7-7*turn][3];
			board[7-7*turn][2] = board[7-7*turn][4];
			board[7-7*turn][3] = board[7-7*turn][0];
			board[7-7*turn][4] = new Air(7-7*turn, 4);
			board[7-7*turn][0] = new Air(7-7*turn, 0);
		}
		else{
			cout << "ERROR\n";
			return 0;
		}
		oo[turn] = 0;
		ooo[turn] = 0;
		turn = !turn;
		if(choosing == BOMB){
			delete board[xf][yf];
			board[xf][yf] = new Air(xf, yf);
			for(int i = 0; i < 8; i++){
				if(xf+dirs[i][0] >= 0 && xf+dirs[i][0] < 8 && yf+dirs[i][1] >= 0 && yf+dirs[i][1] < 8){
					delete board[xf+dirs[i][0]][yf+dirs[i][1]];
					board[xf+dirs[i][0]][yf+dirs[i][1]] = new Air(xf+dirs[i][0], yf+dirs[i][1]);
				}
			} 
		}
		return 0;
	}
	if(*board[xi][yi] == PAWN && xi == 1 + turn * 5){
		delete board[xi][yi];
		board[xi][yi] = new Air(xi, yi);
		delete board[xf][yf];
		board[xf][yf] = new Pawn(xf, yf, turn);
		for(int i = turn * 4; i < 4 + turn * 4; i++){
			SDL_Rect pRect = { ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
			pm[i].render(ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * i + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
		}
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderDrawLine( gRenderer, ori_x + gr_w * 8.5, ori_y + gr_h * turn * 4, ori_x + gr_w * 8.5, ori_y + gr_h * (4 + turn * 4));
		SDL_RenderDrawLine( gRenderer, ori_x + gr_w * 9.5, ori_y + gr_h * turn * 4, ori_x + gr_w * 9.5, ori_y + gr_h * (4 + turn * 4));
		SDL_RenderDrawLine( gRenderer, ori_x + gr_w * 8.5, ori_y + gr_h * turn * 4, ori_x + gr_w * 9.5, ori_y + gr_h * turn * 4);
		SDL_RenderDrawLine( gRenderer, ori_x + gr_w * 8.5, ori_y + gr_h * (4 + turn * 4), ori_x + gr_w * 9.5, ori_y + gr_h * (4 + turn * 4));
		renderpieces();
		SDL_RenderPresent( gRenderer );
		if(choosing == BOMB){
			delete board[xf][yf];
			board[xf][yf] = new Air(xf, yf);
			for(int i = 0; i < 8; i++){
				if(xf+dirs[i][0] >= 0 && xf+dirs[i][0] < 8 && yf+dirs[i][1] >= 0 && yf+dirs[i][1] < 8){
					delete board[xf+dirs[i][0]][yf+dirs[i][1]];
					board[xf+dirs[i][0]][yf+dirs[i][1]] = new Air(xf+dirs[i][0], yf+dirs[i][1]);
				}
			}
			return 0;
		}
		else return 1;
	}
	if(*board[xi][yi] == PAWN && yi != yf && *board[xf][yf] == AIR){
		//passant
		delete board[xf][yf];
		delete board[xi][yi];
		delete board[xi][yf];
		board[xf][yf] = new Pawn(xf, yf, turn); 
		board[xi][yi] = new Air(xi, yi);
		board[xi][yf] = new Air(xi, yf);
		turn = !turn;
		if(choosing == BOMB){
			delete board[xf][yf];
			board[xf][yf] = new Air(xf, yf);
			for(int i = 0; i < 8; i++){
				if(xf+dirs[i][0] >= 0 && xf+dirs[i][0] < 8 && yf+dirs[i][1] >= 0 && yf+dirs[i][1] < 8){
					delete board[xf+dirs[i][0]][yf+dirs[i][1]];
					board[xf+dirs[i][0]][yf+dirs[i][1]] = new Air(xf+dirs[i][0], yf+dirs[i][1]);
				}
			} 
		}
		return 0;
	}
	if(*board[xi][yi] == KING){
		oo[turn] = 0;
		ooo[turn] = 0;
	}
	else if(*board[xi][yi] == ROOK){
		if(xi == 0 && yi == 0) ooo[0] = 0;
		if(xi == 0 && yi == 7) oo[0] = 0;
		if(xi == 7 && yi == 0) ooo[1] = 0;
		if(xi == 7 && yi == 7) oo[1] = 0;
	}
	else if(*board[xi][yi] == PAWN && xi == 6 - board[xi][yi]->getside() * 5 && xf == 4 - board[xi][yi]->getside()){
		passant = yi;
	}
	else{
		passant = 100;
	}
	board[xi][yi]->setxy(xf, yf);
	delete board[xf][yf];
	board[xf][yf] = board[xi][yi];
	board[xi][yi] = new Air(xi, yi);
	turn = !turn;
	if(choosing == BOMB){
		delete board[xf][yf];
		board[xf][yf] = new Air(xf, yf);
		for(int i = 0; i < 8; i++){
			if(xf+dirs[i][0] >= 0 && xf+dirs[i][0] < 8 && yf+dirs[i][1] >= 0 && yf+dirs[i][1] < 8){
				delete board[xf+dirs[i][0]][yf+dirs[i][1]];
				board[xf+dirs[i][0]][yf+dirs[i][1]] = new Air(xf+dirs[i][0], yf+dirs[i][1]);
			}
		} 
	}
	return 0;
}

bool Board::checked(bool side){
	int x[2] = {}, y[2] = {};
	int kc = 0;
	for(x[kc] = 0; x[kc] < 8; x[kc]++){//找到國王位置 
		for(y[kc] = 0; y[kc] < 8; y[kc]++){
			if(*board[x[kc]][y[kc]] == KING && board[x[kc]][y[kc]]->getside() == side){
				kc++;
				if(kc == 2) break;
				x[kc] = x[kc-1];
				y[kc] = y[kc-1];
			}
		}
		if(kc == 2) break;
	}
	for(int g = 0; g < kc; g++){
		for(int i = 0; i < 8; i++){//king check
			if(x[g]+dirs[i][0] >= 0 && x[g]+dirs[i][0] < 8 && y[g]+dirs[i][1] >= 0 && y[g]+dirs[i][1] < 8){
				if(*board[x[g]+dirs[i][0]][y[g]+dirs[i][1]] == KING && board[x[g]+dirs[i][0]][y[g]+dirs[i][1]]->getside() == 1-side){
					return 1;
				}	
			}
		}
		if(x[g] > 0 && !side || x[g] < 7 && side){//pawn check 
			if(y[g] > 0 && *board[x[g]-1+side*2][y[g]-1] == PAWN && board[x[g]-1+side*2][y[g]-1]->getside() == 1-side || y[g] < 7 && *board[x[g]-1+2*side][y[g]+1] == PAWN && board[x[g]-1+2*side][y[g]+1]->getside() == 1-side){
				return 1;
			}
		}
		for(int i = 0; i < 4; i++){//直線武器 
			int t = 1;
			while(x[g]+dirs[i][0]*t >= 0 && x[g]+dirs[i][0]*t < 8 && y[g]+dirs[i][1]*t >= 0 && y[g]+dirs[i][1]*t < 8){
				if(*board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == AIR){
					t++;
					continue;
				}
				if(board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t]->getside() == 1-side && (*board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == QUEEN || *board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == ROOK)){
					return 1;
				}
				break;//有東西擋著就break 
			}
		}
		for(int i = 4; i < 8; i++){//斜線武器 
			int t = 1;
			while(x[g]+dirs[i][0]*t >= 0 && x[g]+dirs[i][0]*t < 8 && y[g]+dirs[i][1]*t >= 0 && y[g]+dirs[i][1]*t < 8){
				if(*board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == AIR){
					t++;
					continue;
				}
				if(board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t]->getside() == 1-side && (*board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == QUEEN || *board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == BISHOP)){
					return 1;
				}
				break;
			}
		}
		for(int i = 0; i < 8; i++){//knight check 
			if(x[g]+h[i][0] >= 0 && x[g]+h[i][0] < 8 && y[g]+h[i][1] >= 0 && y[g]+h[i][1] < 8){
				if(*board[x[g]+h[i][0]][y[g]+h[i][1]] == KNIGHT && board[x[g]+h[i][0]][y[g]+h[i][1]]->getside() == 1-side){
					return 1;
				}
			}
		}	
	}
	return 0;
}

bool Board::check_if_move(int xi, int yi, int xf, int yf){//判斷是否自殺 
	Piece* tem = board[xf][yf];
	board[xf][yf] = board[xi][yi];
	board[xi][yi] = new Air(xi, yi);
	bool c = checked(turn);
	delete board[xi][yi];
	board[xi][yi] = board[xf][yf];
	board[xf][yf] = tem;
	return c;
}

bool Board::any_valid(){
	bool vm[8][8] = {};
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(board[i][j]->getside() == turn){
				if(board[i][j]->valid_moves(vm, *this, 1)){ 
					return 1;
				}
			}
		}
	}
	return 0;
}

bool Board::checkmate(){
	if(!any_valid() && checked(turn)) return 1;
	return 0;
}

bool Board::stalemate(){
	if(!any_valid() && !checked(turn)) return 1;
	bool N[2] = {}, B[2] = {};
	bool bcolor, Bcolor;
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(*board[i][j] == AIR || *board[i][j] == KING) continue;
			else if(*board[i][j] == KNIGHT){
				if(N[0] || N[1] || B[0] || B[1]) return 0;
				N[board[i][j]->getside()] = 1;
			}
			else if(*board[i][j] == BISHOP){
				if(N[0] || N[1] || B[0] && (!board[i][j]->getside() || Bcolor != ((i + j) & 1)) || B[1] && (board[i][j]->getside() || Bcolor != ((i + j) & 1))) return 0;
				B[board[i][j]->getside()] = 1;
				Bcolor = (i + j) & 1;
			}
			else{
				return 0;
			}
		}
	}
	return 1;
}

bool* Board::getooo(){
	return ooo;
}

bool* Board::getoo(){
	return oo;
}

bool Board::getturn(){
	return turn;
}

int Board::getpassant(){
	return passant;
}

void Board::renderpieces(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			board[i][j]->rerender();
		}
	}
}

void Board::promotion(int x, int y, int s){
	delete board[x][y];
	switch(s){
		case 0:
			board[x][y] = new Queen(x, y, turn);
			break;
		case 1:
			board[x][y] = new Rook(x, y, turn);
			break;
		case 2:
			board[x][y] = new Bishop(x, y, turn);
			break;
		case 3:
			board[x][y] = new Knight(x, y, turn);
			break;
	}
	SDL_Rect pRect = { ori_x + gr_w * 8.5, ori_y, gr_w + l_w, gr_h * 8 + l_w};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderFillRect( gRenderer, &pRect );
	pRect = { ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, gr_w - l_w, gr_h - l_w };
	if(x + y & 1) SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );
	SDL_RenderFillRect( gRenderer, &pRect );
	turn = !turn;
}

bool Board::king_died(bool side){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(*board[i][j] == KING && board[i][j]->getside() == side){
				return 0;
			}
		}
	}
	return 1;
}

void Board::changepmtowhite(){
	pm[4].loadFromFile("img/queen_w.png");
	pm[5].loadFromFile("img/rook_w.png");
	pm[6].loadFromFile("img/knight_w.png");
	pm[7].loadFromFile("img/bishop_w.png");
}

Card::Card(){
	for(int i = 0; i < 6; i++) used[i] = 0;
	std::string card_str[3] = {"img/bomb.png", "img/freeze.png", "img/penetrate.png"};
	for(int i = 0; i < 3; i++){
		cardimg[i].loadFromFile(card_str[i]);
		SDL_Rect pRect = { ori_x + gr_w * 9 + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
		cardimg[i].render(ori_x + gr_w * 9 + l_w, ori_y + gr_h * i + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
		cardimg[i+3].loadFromFile(card_str[i]);
		pRect = { ori_x + gr_w * 9 + l_w, ori_y + gr_h * (i+5) + l_w, gr_w - l_w, gr_h - l_w };
		cardimg[i+3].render(ori_x + gr_w * 9 + l_w, ori_y + gr_h * (i+5) + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
	}
}

void Card::rendercards(){
	std::string card_str[3] = {"img/bomb.png", "img/freeze.png", "img/penetrate.png"};
	for(int i = 0; i < 3; i++){
		if(!used[i]){
			cardimg[i].loadFromFile(card_str[i]);
			SDL_Rect pRect = { ori_x + gr_w * 9 + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
			cardimg[i].render(ori_x + gr_w * 9 + l_w, ori_y + gr_h * i + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
		}
		if(!used[i+3]){
			cardimg[i+3].loadFromFile(card_str[i]);
			SDL_Rect pRect = { ori_x + gr_w * 9 + l_w, ori_y + gr_h * (i+5) + l_w, gr_w - l_w, gr_h - l_w };
			cardimg[i+3].render(ori_x + gr_w * 9 + l_w, ori_y + gr_h * (i+5) + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
		}
	}
}

void Card::use(int i){
	used[i] = 1;
}

bool Card::valid_card(int i){
	return !used[i];
}

void renderpm(Board& b, int i){
	SDL_Rect pRect = { ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
	b.pm[i].render(ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * i + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
}

void randomsecondking(Board& b){
	srand(time(0));
	int a = rand() % 7;
	a += (a >= 4);
	delete b.board[0][a];
	delete b.board[7][a];
	SDL_Rect cRect = { ori_x + gr_w * a + l_w, ori_y + l_w, gr_w - l_w, gr_h - l_w };
	if(a & 1) SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );
	else SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderFillRect( gRenderer, &cRect );
	cRect = { ori_x + gr_w * a + l_w, ori_y + gr_h * 7 + l_w, gr_w - l_w, gr_h - l_w };
	if(a & 1) SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	else SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );
	SDL_RenderFillRect( gRenderer, &cRect );
	b.board[0][a] = new King(0, a, 1);
	b.board[7][a] = new King(7, a, 0);
}
