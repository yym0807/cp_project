#ifndef GAME_S_K_H
#define GAME_S_K_H

#include "LTexture.h" 
//enum {AIR, KING, QUEEN, ROOK, KNIGHT, BISHOP, PAWN};

class Piece1;

class Board1;

void renderpm(Board1&, int);

class Board1{
	friend void renderpm(Board1&, int);
	private:
		bool oo[2];
		bool ooo[2];
		bool turn; // 0: white 1: black
		int passant;
		Piece1*** board;
		Image pm[8]; // for promotion
	public:
		Board1();
		~Board1();
		Piece1***& getboard();
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
};

class Piece1{
	protected:
		int p;
		int side; // 0 for white, 1 for black, -1 for air
		int x; // 8 ~ 1
		int y; // a ~ h
		Image img;
	public:
		Piece1(int, int, int, int);
		~Piece1();
		virtual bool valid_moves(bool [][8], Board1&){return 0;}
		bool operator==(int);
		void setxy(int, int);
		int getside();
//		int getname();
		void rerender();
		virtual void loadResultImage(std::string){}
//		void renderxy(int, int);
};



class King1: public Piece1{
	public:
		King1(int, int, int);
		bool valid_moves(bool [][8], Board1&);
		void loadResultImage(std::string);
};

class Queen1: public Piece1{
	public:
		Queen1(int, int, int);
		bool valid_moves(bool [][8], Board1&);
};

class Rook1: public Piece1{
	public:
		Rook1(int, int, int);
		bool valid_moves(bool [][8], Board1&);
};

class Knight1: public Piece1{
	public:
		Knight1(int, int, int);
		bool valid_moves(bool [][8], Board1&);
};

class Bishop1: public Piece1{
	public:
		Bishop1(int, int, int);
		bool valid_moves(bool [][8], Board1&);
};

class Pawn1: public Piece1{
	public:
		Pawn1(int, int, int);
		bool valid_moves(bool [][8], Board1&);
};

class Air1: public Piece1{
	public:
		Air1(int, int);
		bool valid_moves(bool [][8], Board1&);
};

#endif
