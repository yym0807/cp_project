#include <iostream>
#include "game.h"
using namespace std;

const int dirs[8][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1}}; // URDLUrDrDlUl
const int h[8][2] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
const char side[2][6] = {"white", "black"};
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
// class Piece
Piece::Piece(int xi, int yi, int pi, int si): x(xi), y(yi), p(pi), side(si){
	// ���͹Ϥ�
}

Piece::~Piece(){
	// �����Ϥ�
}

bool Piece::operator==(int c){
	return (p == c);
}

bool Piece::operator>=(int c){
	return (p >= c);
}

bool Piece::operator<=(int c){
	return (p <= c);
}

void Piece::setxy(int xf, int yf){
	x = xf;
	y = yf;
}

int Piece::getside(){
	return side;
}

int Piece::getname(){
	return p;
}

King::King(int xi, int yi, int si): Piece(xi, yi, KING, si){}

bool King::valid_moves(bool vm[][8], Board &b){
	Piece*** board = b.getboard();
	bool* oo = b.getoo();
	bool* ooo = b.getooo();
	bool flag = 0;
	for(int i = 0; i < 8; i++){
		if(x+dirs[i][0] >= 0 && x+dirs[i][0] < 8 && y+dirs[i][1] >= 0 && y+dirs[i][1] < 8 && board[x+dirs[i][0]][y+dirs[i][1]]->getside() != side && !b.check_if_move(x, y, x+dirs[i][0], y+dirs[i][1])){
			vm[x+dirs[i][0]][y+dirs[i][1]] = 1;
		}
	}
	if(ooo[side]){
		if(!b.checked(side) && *board[x][1] == AIR && *board[x][2] == AIR && !b.check_if_move(x, 4, x, 2) && *board[x][3] == AIR && !b.check_if_move(x, 4, x, 3)){
			flag = 1;
			vm[x][2] = 1;
		}
	}
	if(oo[side]){
		if(!b.checked(side) && *board[x][5] == AIR && !b.check_if_move(x, 4, x, 5) && *board[x][6] == AIR && !b.check_if_move(x, 4, x, 6)){
			flag = 1;
			vm[x][6] = 1;
		}
	}
	return flag;
}

Queen::Queen(int xi, int yi, int si): Piece(xi, yi, QUEEN, si){}

bool Queen::valid_moves(bool vm[][8], Board &b){
	Piece*** board = b.getboard();
	bool flag = 0;
	for(int i = 0; i < 8; i++){
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == AIR && !b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t)){
				flag = 1;
				vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
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

Rook::Rook(int xi, int yi, int si): Piece(xi, yi, ROOK, si){}

bool Rook::valid_moves(bool vm[][8], Board &b){
	Piece*** board = b.getboard();
	bool flag = 0;
	for(int i = 0; i < 4; i++){
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8 && !b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t)){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == AIR){
				flag = 1;
				vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
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

Knight::Knight(int xi, int yi, int si): Piece(xi, yi, KNIGHT, si){}

bool Knight::valid_moves(bool vm[][8], Board &b){
	Piece*** board = b.getboard();
	bool flag = 0;
	for(int i = 0; i < 8; i++){
		if(x+h[i][0] >= 0 && x+h[i][0] < 8 && y+h[i][1] >= 0 && y+h[i][1] < 8 && board[x+h[i][0]][y+h[i][1]]->getside() != side && !b.check_if_move(x, y, x+h[i][0], y+h[i][1])){
			flag = 1;
			vm[x+h[i][0]][y+h[i][1]] = 1;
		}
	}
	return flag;
}

Bishop::Bishop(int xi, int yi, int si): Piece(xi, yi, BISHOP, si){}

bool Bishop::valid_moves(bool vm[][8], Board &b){
	Piece*** board = b.getboard();
	bool flag = 0;
	for(int i = 4; i < 8; i++){
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8 && !b.check_if_move(x, y, x+dirs[i][0]*t, y+dirs[i][1]*t)){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == AIR){
				flag = 1;
				vm[x+dirs[i][0]*t][y+dirs[i][1]*t] = 1;
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

Pawn::Pawn(int xi, int yi, int si): Piece(xi, yi, PAWN, si){}

bool Pawn::valid_moves(bool vm[][8], Board &b){
	Piece*** board = b.getboard();
	int passant = b.getpassant();
	bool flag = 0;
	if(side){
		if(*board[x+1][y] == AIR){
			if(!b.check_if_move(x, y, x+1, y)){
				flag = 1;
				vm[x+1][y] = 1;
			}
			if(x == 1 && *board[x+2][y] == AIR && !b.check_if_move(x, y, x+2, y)){
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
		if(x == 4 && (passant == y + 1 || passant == y - 1)){
			flag = 1;
			vm[x+1][passant] = 1;
		}
	}
	else{
		if(*board[x-1][y] == AIR){
			if(!b.check_if_move(x, y, x-1, y)){
				flag = 1;
				vm[x-1][y] = 1;
			}
			if(x == 6 && *board[x-2][y] == AIR && !b.check_if_move(x, y, x-2, y)){
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
		if(x == 3 && (passant == y + 1 || passant == y - 1)){
			flag = 1;
			vm[x-1][passant] = 1;
		}
	}
	return 1;
}

Air::Air(int xi, int yi): Piece(xi, yi, AIR, -1){}

bool Air::valid_moves(bool vm[][8], Board &b){
	return 0;
}

// class Board
Board::Board(){
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
}

Board::~Board(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			delete [] board[i][j];
		}
		delete [] board[i];
	}
	delete [] board;
}

Piece***& Board::getboard(){
	return board;
}

void Board::move(int xi, int yi, int xf, int yf){
	if(*board[xi][yi] == KING && (yi-yf) * (yi-yf) > 1){ // OO or OOO
		if(yf == 6){ // OO
			board[7*turn][4]->setxy(7*turn, 6);
			board[7*turn][7]->setxy(7*turn, 5);
			delete board[7*turn][6];
			delete board[7*turn][5];
			board[7*turn][6] = board[7*turn][4];
			board[7*turn][5] = board[7*turn][7];
			board[7*turn][4] = new Air(7*turn, 4);
			board[7*turn][7] = new Air(7*turn, 7);
		}
		else if(yf == 2){ // OOO
			board[7*turn][4]->setxy(7*turn, 2);
			board[7*turn][0]->setxy(7*turn, 3);
			delete board[7*turn][2];
			delete board[7*turn][3];
			board[7*turn][2] = board[7*turn][4];
			board[7*turn][3] = board[7*turn][0];
			board[7*turn][4] = new Air(7*turn, 4);
			board[7*turn][0] = new Air(7*turn, 0);
		}
		else{
			cout << "ERROR\n";
			return;
		}
		oo[turn] = 0;
		ooo[turn] = 0;
		turn = !turn;
		return;
	}
	if(*board[xi][yi] == PAWN && xi == 1 + board[xi][yi]->getside() * 5){
		// promotion
		cout << '[' << side[turn] << "] Pawn promotion(Q, B, N, R): ";
		char p;
		cin >> p;
		delete board[xi][yi];
		board[xi][yi] = new Air(xi, yi);
		delete board[xf][yf];
		switch(p){
			case 'Q':
				board[xf][yf] = new Queen(xf, yf, turn);
				break;
			case 'R':
				board[xf][yf] = new Rook(xf, yf, turn);
				break;
			case 'B':
				board[xf][yf] = new Bishop(xf, yf, turn);
				break;
			case 'N':
				board[xf][yf] = new Knight(xf, yf, turn);
				break;
			default:
				cout << "ERROR\n";
		}
		turn = !turn;
		cout << turn << '\n';
		return;
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
		return;
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
}

bool Board::checked(bool side){
	int x, y;
	for(x = 0; x < 8; x++){//�������m 
		for(y = 0; y < 8; y++){
			if(*board[x][y] == KING && board[x][y]->getside() == side) break;
		}
		if(y < 8) break;
	}
	for(int i = 0; i < 8; i++){//king check
		if(x+dirs[i][0] >= 0 && x+dirs[i][0] < 8 && y+dirs[i][1] >= 0 && y+dirs[i][1] < 8){
			if(*board[x+dirs[i][0]][y+dirs[i][1]] == KING){
				return 1;
			}	
		}
	}
	if(x > 0 && !side || x < 7 && side){//pawn check 
		if(y > 0 && *board[x-1+side*2][y-1] == PAWN && board[x-1+side*2][y-1]->getside() == 1-side || y < 7 && *board[x-1+2*side][y+1] == PAWN && board[x-1+2*side][y+1]->getside() == 1-side){
			return 1;
		}
	}
	for(int i = 0; i < 4; i++){//���u�Z�� 
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == AIR){
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t]->getside() == 1-side && (*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == QUEEN || *board[x+dirs[i][0]*t][y+dirs[i][1]*t] == ROOK)){
				return 1;
			}
			break;//���F��׵۴Nbreak 
		}
	}
	for(int i = 4; i < 8; i++){//�׽u�Z�� 
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == AIR){
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t]->getside() == 1-side && (*board[x+dirs[i][0]*t][y+dirs[i][1]*t] == QUEEN || *board[x+dirs[i][0]*t][y+dirs[i][1]*t] == BISHOP)){
				return 1;
			}
			break;
		}
	}
	for(int i = 0; i < 8; i++){//knight check 
		if(x+h[i][0] >= 0 && x+h[i][0] < 8 && y+h[i][1] >= 0 && y+h[i][1] < 8){
			if(*board[x+h[i][0]][y+h[i][1]] == KNIGHT && board[x+h[i][0]][y+h[i][1]]->getside() == 1-side){
				return 1;
			}
		}
	}
	return 0;
}

bool Board::check_if_move(int xi, int yi, int xf, int yf){//�P�_�O�_�۱� 
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
				if(board[i][j]->valid_moves(vm, *this)){
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

void Board::print(){
	for(int i = 0; i < 8; i++){
			cout << 8 - i << " |";
			for(int j = 0; j < 8; j++){
				cout << board[i][j]->getname() << ' ';
			}
			cout << '\n';
		}
		cout << "  ________________\n   a b c d e f g h\n";
}
