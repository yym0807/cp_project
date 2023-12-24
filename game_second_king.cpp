#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "game_second_king.h"
#include "Ltexture.h" 
//#define Piece1 Piece11
//#define Board1 Board11
//#define King1 King11
//#define Queen1 Queen11
//#define Rook1 Rook11
//#define Knight1 Knight11
//#define Bishop1 Bishop11
//#define pawn1 pawn11
//#define Air1 Air11 
using namespace std;

//  
enum {Air1, King1, Queen1, Rook1, Knight1, Bishop1, Pawn1};

//
 
const int dirs[8][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1}}; // URDLUrDrDlUl
const int h[8][2] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
const char side[2][6] = {"white", "black"};
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern SDL_Renderer* gRenderer;
const double bo_w = (double)SCREEN_HEIGHT / 10 * 8, bo_h = (double)SCREEN_HEIGHT / 10 * 8;
const double ori_x = (SCREEN_WIDTH - bo_w) / 2, ori_y = (SCREEN_HEIGHT - bo_h) / 2;
const double gr_w = bo_w / 8, gr_h = bo_h / 8;
const double l_w = 1; // line width

// class Piece1
Piece1::Piece1(int xi, int yi, int pi, int si): x(xi), y(yi), p(pi), side(si){
	string str;
	string fs[2] = {"w.png", "b.png"};
	switch(pi){
		case King1:
			str = "img/king_" + fs[si];
			break;
		case Queen1:
			str = "img/queen_" + fs[si];
			break;
		case Rook1:
			str = "img/rook_" + fs[si];
			break;
		case Bishop1:
			str = "img/bishop_" + fs[si];
			break;
		case Knight1:
			str = "img/knight_" + fs[si];
			break;
		case Pawn1:
			str = "img/pawn_" + fs[si];
			break;
	}
//	cout << str << '\n';
	if(pi != Air1){
		img.loadFromFile(str);
		SDL_Rect pRect = { ori_x + gr_w * yi + l_w, ori_y + gr_h * xi + l_w, gr_w - l_w, gr_h - l_w };
		img.render(ori_x + gr_w * yi + l_w, ori_y + gr_h * xi + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
//		SDL_RenderPresent( gRenderer );
	}
}

Piece1::~Piece1(){}

bool Piece1::operator==(int c){
	return (p == c);
}

void Piece1::setxy(int xf, int yf){
	x = xf;
	y = yf;
}

int Piece1::getside(){
	return side;
}

//int Piece1::getname(){
//	return p;
//}

void Piece1::rerender(){
	if(p != Air1){
		SDL_Rect pRect = { ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, gr_w - l_w, gr_h - l_w };
		img.render(ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
	}
}

//void Piece1::renderxy(int mx, int my){
//	SDL_Rect pRect = { mx - gr_w / 2, my - gr_h * x / 2, gr_w - l_w, gr_h - l_w };
//	img.render(mx - gr_w / 2, my - gr_h * x / 2, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
//}

King1::King1(int xi, int yi, int si): Piece1(xi, yi, King1, si){}

bool King1::valid_moves(bool vm[][8], Board1 &b){
	Piece1*** board = b.getboard();
	bool* oo = b.getoo();
	bool* ooo = b.getooo();
	bool flag = 0;
	for(int i = 0; i < 8; i++){
		if(x+dirs[i][0] >= 0 && x+dirs[i][0] < 8 && y+dirs[i][1] >= 0 && y+dirs[i][1] < 8 && board[x+dirs[i][0]][y+dirs[i][1]]->getside() != side && !b.check_if_move(x, y, x+dirs[i][0], y+dirs[i][1])){
			flag = 1;
			vm[x+dirs[i][0]][y+dirs[i][1]] = 1;
		}
	}
	if(ooo[side]){
		if(x == 7 - 7 * side && y == 4 && !b.checked(side) && *board[x][1] == Air1 && *board[x][2] == Air1 && !b.check_if_move(x, 4, x, 2) && *board[x][3] == Air1 && !b.check_if_move(x, 4, x, 3)){
			flag = 1;
			vm[x][2] = 1;
		}
	}
	if(oo[side]){
		if(x == 7 - 7 * side && y == 4 && !b.checked(side) && *board[x][5] == Air1 && !b.check_if_move(x, 4, x, 5) && *board[x][6] == Air1 && !b.check_if_move(x, 4, x, 6)){
			flag = 1;
			vm[x][6] = 1;
		}
	}
	return flag;
}

//loadResultImage
void King1::loadResultImage(std::string str){
	img.loadFromFile(str);
//	SDL_Rect pRect{}
//	img.render(ori_x + gr_w * y + l_w, ori_y + gr_h * x + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
}

Queen1::Queen1(int xi, int yi, int si): Piece1(xi, yi, Queen1, si){}

bool Queen1::valid_moves(bool vm[][8], Board1 &b){
	Piece1*** board = b.getboard();
	bool flag = 0;
	for(int i = 0; i < 8; i++){
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == Air1){
				if(!b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t)){
					flag = 1;
					vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
				}
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t]->getside() != side && !b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t)){
				flag = 1;
				vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
			}
			break;
		}
	}
	return flag;
}

Rook1::Rook1(int xi, int yi, int si): Piece1(xi, yi, Rook1, si){}

bool Rook1::valid_moves(bool vm[][8], Board1 &b){
	Piece1*** board = b.getboard();
	bool flag = 0;
	for(int i = 0; i < 4; i++){
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == Air1){
				if(!b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t)){
					flag = 1;
					vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
				}
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t]->getside() != side && !b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t)){
				flag = 1;
				vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
			}
			break;
		}
	}
	return flag;
}

Knight1::Knight1(int xi, int yi, int si): Piece1(xi, yi, Knight1, si){}

bool Knight1::valid_moves(bool vm[][8], Board1 &b){
	Piece1*** board = b.getboard();
	bool flag = 0;
	for(int i = 0; i < 8; i++){
		if(x+h[i][0] >= 0 && x+h[i][0] < 8 && y+h[i][1] >= 0 && y+h[i][1] < 8 && board[x+h[i][0]][y+h[i][1]]->getside() != side && !b.check_if_move(x, y, x+h[i][0], y+h[i][1])){
			flag = 1;
			vm[x+h[i][0]][y+h[i][1]] = 1;
		}
	}
	return flag;
}

Bishop1::Bishop1(int xi, int yi, int si): Piece1(xi, yi, Bishop1, si){}

bool Bishop1::valid_moves(bool vm[][8], Board1 &b){
	Piece1*** board = b.getboard();
	bool flag = 0;
	for(int i = 4; i < 8; i++){
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == Air1){
				if(!b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t)){
					flag = 1;
					vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
				}
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t]->getside() != side && !b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t)){
				flag = 1;
				vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
			}
			break;
		}
	}
	return flag;
}

Pawn1::Pawn1(int xi, int yi, int si): Piece1(xi, yi, Pawn1, si){}

bool Pawn1::valid_moves(bool vm[][8], Board1 &b){
	Piece1*** board = b.getboard();
	int passant = b.getpassant();
	bool flag = 0;
	if(side){
		if(*board[x+1][y] == Air1){
			if(!b.check_if_move(x, y, x+1, y)){
				flag = 1;
				vm[x+1][y] = 1;
			}
			if(x == 1 && *board[x+2][y] == Air1 && !b.check_if_move(x, y, x+2, y)){
				flag = 1;
				vm[x+2][y] = 1;
			}
		}
		if(y < 7 && board[x+1][y+1]->getside() == 0 && !b.check_if_move(x, y, x+1, y+1)){
			flag = 1;
			vm[x+1][y+1] = 1;
		}
		if(y > 0 && board[x+1][y-1]->getside() == 0 && !b.check_if_move(x, y, x+1, y-1)){
			flag = 1;
			vm[x+1][y-1] = 1;
		}
		if(x == 4 && (passant == y + 1 || passant == y - 1) && !b.check_if_move(x, y, x+1, passant)){
			flag = 1;
			vm[x+1][passant] = 1;
		}
	}
	else{
		if(*board[x-1][y] == Air1){
			if(!b.check_if_move(x, y, x-1, y)){
				flag = 1;
				vm[x-1][y] = 1;
			}
			if(x == 6 && *board[x-2][y] == Air1 && !b.check_if_move(x, y, x-2, y)){
				flag = 1;
				vm[x-2][y] = 1;
			}
		}
		if(y < 7 && board[x-1][y+1]->getside() == 1 && !b.check_if_move(x, y, x-1, y+1)){
			flag = 1;
			vm[x-1][y+1] = 1;
		}
		if(y > 0 && board[x-1][y-1]->getside() == 1 && !b.check_if_move(x, y, x-1, y-1)){
			flag = 1;
			vm[x-1][y-1] = 1;
		}
		if(x == 3 && (passant == y + 1 || passant == y - 1) && !b.check_if_move(x, y, x-1, passant)){
			flag = 1;
			vm[x-1][passant] = 1;
		}
	}
	return flag;
}

Air1::Air1(int xi, int yi): Piece1(xi, yi, Air1, -1){}

bool Air1::valid_moves(bool vm[][8], Board1 &b){
	return 0;
}

// class Board1
Board1::Board1(){
	oo[0] = 1;
	oo[1] = 1;
	ooo[0] = 1;
	ooo[1] = 1;
	turn = 0;
	passant = -2;
	board = new Piece1** [8];
	for(int i = 0; i < 8; i++){
		board[i] = new Piece1* [8];
	}
	board[0][4] = new King1(0, 4, 1);
	board[7][4] = new King1(7, 4, 0);
	board[0][0] = new Rook1(0, 0, 1);
	board[0][1] = new Knight1(0, 1, 1);
	board[0][2] = new Bishop1(0, 2, 1);
	board[0][3] = new Queen1(0, 3, 1);
	board[0][5] = new Bishop1(0, 5, 1);
	board[0][6] = new Knight1(0, 6, 1);
	board[0][7] = new Rook1(0, 7, 1);
	board[7][0] = new Rook1(7, 0, 0);
	board[7][1] = new Knight1(7, 1, 0);
	board[7][2] = new Bishop1(7, 2, 0);
	board[7][3] = new Queen1(7, 3, 0);
	board[7][5] = new Bishop1(7, 5, 0);
	board[7][6] = new Knight1(7, 6, 0);
	board[7][7] = new Rook1(7, 7, 0);
	for(int i = 0; i < 8; i++){
		board[1][i] = new Pawn1(1, i, 1);
		board[6][i] = new Pawn1(6, i, 0);
		for(int j = 2; j < 6; j++){
			board[j][i] = new Air1(j, i);
		}
	}
	srand(time(0));
	int a = rand() % 7;
	a += (a >= 4);
	delete board[0][a];
	delete board[7][a];
	SDL_Rect cRect = { ori_x + gr_w * a + l_w, ori_y + l_w, gr_w - l_w, gr_h - l_w };
	if(a & 1) SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );
	else SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderFillRect( gRenderer, &cRect );
	cRect = { ori_x + gr_w * a + l_w, ori_y + gr_h * 7 + l_w, gr_w - l_w, gr_h - l_w };
	if(a & 1) SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	else SDL_SetRenderDrawColor( gRenderer, 0xB0, 0xD0, 0xEE, 0xFF );
	SDL_RenderFillRect( gRenderer, &cRect );
	board[0][a] = new King1(0, a, 1);
	board[7][a] = new King1(7, a, 0);
	
	pm[0].loadFromFile("img/queen_w.png");
	pm[1].loadFromFile("img/rook_w.png");
	pm[2].loadFromFile("img/knight_w.png");
	pm[3].loadFromFile("img/bishop_w.png");
	pm[4].loadFromFile("img/queen_b.png");
	pm[5].loadFromFile("img/rook_b.png");
	pm[6].loadFromFile("img/knight_b.png");
	pm[7].loadFromFile("img/bishop_b.png");
}

Board1::~Board1(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			delete [] board[i][j];
		}
		delete [] board[i];
	}
	delete [] board;
}

Piece1***& Board1::getboard(){
	return board;
}

bool Board1::move(int xi, int yi, int xf, int yf){ // return 1 if promotion
	if(*board[xi][yi] == King1 && (yi-yf) * (yi-yf) > 1){ // OO or OOO
		if(yf == 6){ // OO
			board[7-7*turn][4]->setxy(7-7*turn, 6);
			board[7-7*turn][7]->setxy(7-7*turn, 5);
			delete board[7-7*turn][6];
			delete board[7-7*turn][5];
			board[7-7*turn][6] = board[7-7*turn][4];
			board[7-7*turn][5] = board[7-7*turn][7];
			board[7-7*turn][4] = new Air1(7-7*turn, 4);
			board[7-7*turn][7] = new Air1(7-7*turn, 7);
		}
		else if(yf == 2){ // OOO
			board[7-7*turn][4]->setxy(7-7*turn, 2);
			board[7-7*turn][0]->setxy(7-7*turn, 3);
			delete board[7-7*turn][2];
			delete board[7-7*turn][3];
			board[7-7*turn][2] = board[7-7*turn][4];
			board[7-7*turn][3] = board[7-7*turn][0];
			board[7-7*turn][4] = new Air1(7-7*turn, 4);
			board[7-7*turn][0] = new Air1(7-7*turn, 0);
		}
		else{
			cout << "ERROR\n";
			return 0;
		}
		oo[turn] = 0;
		ooo[turn] = 0;
		turn = !turn;
		return 0;
	}
	if(*board[xi][yi] == Pawn1 && xi == 1 + turn * 5){
		delete board[xi][yi];
		board[xi][yi] = new Air1(xi, yi);
		delete board[xf][yf];
		board[xf][yf] = new Pawn1(xf, yf, turn);
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
		return 1;
	}
	if(*board[xi][yi] == Pawn1 && yi != yf && *board[xf][yf] == Air1){
		//passant
		delete board[xf][yf];
		delete board[xi][yi];
		delete board[xi][yf];
		board[xf][yf] = new Pawn1(xf, yf, turn); 
		board[xi][yi] = new Air1(xi, yi);
		board[xi][yf] = new Air1(xi, yf);
		turn = !turn;
		return 0;
	}
	if(*board[xi][yi] == King1){
		if(xi == 7 - 7 * turn && yi == 4){
			oo[turn] = 0;
			ooo[turn] = 0;
		}
	}
	else if(*board[xi][yi] == Rook1){
		if(xi == 0 && yi == 0) ooo[0] = 0;
		if(xi == 0 && yi == 7) oo[0] = 0;
		if(xi == 7 && yi == 0) ooo[1] = 0;
		if(xi == 7 && yi == 7) oo[1] = 0;
	}
	else if(*board[xi][yi] == Pawn1 && xi == 6 - board[xi][yi]->getside() * 5 && xf == 4 - board[xi][yi]->getside()){
		passant = yi;
	}
	else{
		passant = 100;
	}
	board[xi][yi]->setxy(xf, yf);
	delete board[xf][yf];
	board[xf][yf] = board[xi][yi];
	board[xi][yi] = new Air1(xi, yi);
	turn = !turn;
	return 0;
}

bool Board1::checked(bool side){
	int x[2], y[2];
	for(x[0] = 0; x[0] < 8; x[0]++){//找到國王位置 
		for(y[0] = 0; y[0] < 8; y[0]++){
			if(*board[x[0]][y[0]] == King1 && board[x[0]][y[0]]->getside() == side) break;
		}
		if(y[0] < 8) break;
	}
	y[1] = y[0] + 1;
	for(x[1] = x[0]; x[1] < 8; x[1]++){//找到國王位置 
		for(; y[1] < 8; y[1]++){
			if(*board[x[1]][y[1]] == King1 && board[x[1]][y[1]]->getside() == side) break;
		}
		if(y[1] < 8) break;
		y[1] = 0;
	}
	for(int g = 0; g < 2; g++){
		for(int i = 0; i < 8; i++){//king check
			if(x[g]+dirs[i][0] >= 0 && x[g]+dirs[i][0] < 8 && y[g]+dirs[i][1] >= 0 && y[g]+dirs[i][1] < 8){
				if(*board[x[g]+dirs[i][0]][y[g]+dirs[i][1]] == King1 && board[x[g]+dirs[i][0]][y[g]+dirs[i][1]]->getside() == 1-side){
					return 1;
				}	
			}
		}
		if(x[g] > 0 && !side || x[g] < 7 && side){//pawn check 
			if(y[g] > 0 && *board[x[g]-1+side*2][y[g]-1] == Pawn1 && board[x[g]-1+side*2][y[g]-1]->getside() == 1-side || y[g] < 7 && *board[x[g]-1+2*side][y[g]+1] == Pawn1 && board[x[g]-1+2*side][y[g]+1]->getside() == 1-side){
				return 1;
			}
		}
		for(int i = 0; i < 4; i++){//直線武器 
			int t = 1;
			while(x[g]+dirs[i][0]*t >= 0 && x[g]+dirs[i][0]*t < 8 && y[g]+dirs[i][1]*t >= 0 && y[g]+dirs[i][1]*t < 8){
				if(*board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == Air1){
					t++;
					continue;
				}
				if(board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t]->getside() == 1-side && (*board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == Queen1 || *board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == Rook1)){
					return 1;
				}
				break;//有東西擋著就break 
			}
		}
		for(int i = 4; i < 8; i++){//斜線武器 
			int t = 1;
			while(x[g]+dirs[i][0]*t >= 0 && x[g]+dirs[i][0]*t < 8 && y[g]+dirs[i][1]*t >= 0 && y[g]+dirs[i][1]*t < 8){
				if(*board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == Air1){
					t++;
					continue;
				}
				if(board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t]->getside() == 1-side && (*board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == Queen1 || *board[x[g]+dirs[i][0]*t][y[g]+dirs[i][1]*t] == Bishop1)){
					return 1;
				}
				break;
			}
		}
		for(int i = 0; i < 8; i++){//knight check 
			if(x[g]+h[i][0] >= 0 && x[g]+h[i][0] < 8 && y[g]+h[i][1] >= 0 && y[g]+h[i][1] < 8){
				if(*board[x[g]+h[i][0]][y[g]+h[i][1]] == Knight1 && board[x[g]+h[i][0]][y[g]+h[i][1]]->getside() == 1-side){
					return 1;
				}
			}
		}	
	}
	return 0;
}

bool Board1::check_if_move(int xi, int yi, int xf, int yf){//判斷是否自殺 
	Piece1* tem = board[xf][yf];
	board[xf][yf] = board[xi][yi];
	board[xi][yi] = new Air1(xi, yi);
	bool c = checked(turn);
	delete board[xi][yi];
	board[xi][yi] = board[xf][yf];
	board[xf][yf] = tem;
	return c;
}

bool Board1::any_valid(){
	bool vm[8][8] = {};
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(board[i][j]->getside() == turn){
				if(board[i][j]->valid_moves(vm, *this)){ 
					return 1;
				}
			}
		}
	}
	return 0;
}

bool Board1::checkmate(){
	if(!any_valid() && checked(turn)) return 1;
	return 0;
}

bool Board1::stalemate(){
	if(!any_valid() && !checked(turn)) return 1;
	bool N[2] = {}, B[2] = {};
	bool bcolor, Bcolor;
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(*board[i][j] == Air1 || *board[i][j] == King1) continue;
			else if(*board[i][j] == Knight1){
				if(N[0] || N[1] || B[0] || B[1]) return 0;
				N[board[i][j]->getside()] = 1;
			}
			else if(*board[i][j] == Bishop1){
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

bool* Board1::getooo(){
	return ooo;
}

bool* Board1::getoo(){
	return oo;
}

bool Board1::getturn(){
	return turn;
}

int Board1::getpassant(){
	return passant;
}

//void Board1::print(){
//	for(int i = 0; i < 8; i++){
//			cout << 8 - i << " |";
//			for(int j = 0; j < 8; j++){
//				cout << board[i][j]->getname() << ' ';
//			}
//			cout << '\n';
//		}
//		cout << "  ________________\n   a b c d e f g h\n";
//}

void Board1::renderpieces(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			board[i][j]->rerender();
		}
	}
}

void Board1::promotion(int x, int y, int s){
	delete board[x][y];
	switch(s){
		case 0:
			board[x][y] = new Queen1(x, y, turn);
			break;
		case 1:
			board[x][y] = new Rook1(x, y, turn);
			break;
		case 2:
			board[x][y] = new Bishop1(x, y, turn);
			break;
		case 3:
			board[x][y] = new Knight1(x, y, turn);
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

void renderpm(Board1& b, int i){
	SDL_Rect pRect = { ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * i + l_w, gr_w - l_w, gr_h - l_w };
	b.pm[i].render(ori_x + gr_w * 8.5 + l_w, ori_y + gr_h * i + l_w, NULL, 0.0, NULL, SDL_FLIP_NONE, &pRect);
}
