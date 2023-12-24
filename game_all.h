#ifndef GAME_ALL_H
#define GAME_ALL_H

#include "LTexture.h" 
enum {AIR, KING, QUEEN, ROOK, KNIGHT, BISHOP, PAWN};
enum {BOMB, FREEZE, PENETRATE, NONE};

class Piece;

class Board;

void renderpm(Board&, int);

void randomsecondking(Board&);

class Card{
	private:
		Image cardimg[6];
		bool used[6];
	public:
		Card();
		void rendercards();
		void use(int);
		bool valid_card(int);
};

class Board{
	friend void renderpm(Board&, int);
	friend void randomsecondking(Board&);
	private:
		bool oo[2];
		bool ooo[2];
		bool turn; // 0: white 1: black
		int passant;
		Piece*** board;
		Image pm[8]; // for promotion
//		LTexture pm[8];
	public:
		Board();
		~Board();
		Piece***& getboard();
		bool move(int, int, int, int);
		bool checked(bool side);
		bool check_if_move(int, int, int, int);
		bool any_valid();
		bool checkmate();
		bool stalemate();
		bool* getooo();
		bool* getoo();
		bool getturn();
		int getpassant();
//		void print();
		void renderpieces();
		void promotion(int, int, int);
		bool king_died(bool);
//		void renderpm(int);
};

class Piece{
	protected:
		int p;
		int side; // 0 for white, 1 for black, -1 for air
		int x; // 8 ~ 1
		int y; // a ~ h
		bool freeze;
		Image img;
//		LTexture img;
	public:
		Piece(int, int, int, int);
		~Piece();
		virtual bool valid_moves(bool [][8], Board&, bool consider_check = 1){return 0;}
		bool operator==(int);
		void setxy(int, int);
		int getside();
//		int getname();
		void rerender();
		virtual void loadResultImage(std::string){}
		bool getfreeze();
		bool freezed();
		bool unfreezed();
//		void renderxy(int, int);
};



class King: public Piece{
	public:
		King(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check = 1);
		void loadResultImage(std::string);
};

class Queen: public Piece{
	public:
		Queen(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check = 1);
};

class Rook: public Piece{
	public:
		Rook(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check = 1);
};

class Knight: public Piece{
	public:
		Knight(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check = 1);
};

class Bishop: public Piece{
	public:
		Bishop(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check = 1);
};

class Pawn: public Piece{
	public:
		Pawn(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check = 1);
};

class Air: public Piece{
	public:
		Air(int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check = 1);
};

#endif
