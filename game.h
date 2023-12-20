#ifndef GAME_H
#define GAME_H

enum {AIR, KING, QUEEN, ROOK, KNIGHT, BISHOP, PAWN};

class Piece;

class Board{
	private:
		bool oo[2];
		bool ooo[2];
		bool turn; // 0: white 1: black
		int passant;
		Piece*** board;
	public:
		Board();
		~Board();
		Piece*** getboard();
		void move(int, int, int, int);
//		void Omove(int);
		bool checked(bool side);
		bool check_if_move(int, int, int, int);
		bool any_valid();
		bool checkmate();
		bool stalemate();
		bool* getooo();
		bool* getoo();
		bool getturn();
		int getpassant();
		void print();
};

class Piece{
	protected:
		int p;
		 // K = king, Q = queen, R = rook, N = knight, B = bishop, I = pawn, * = air uppercase for black, lowercase for white
		int side; // 0 for white, 1 for black, -1 for air
		int x; // 8 ~ 1
		int y; // a ~ h
	public:
		Piece(int, int, int, int);
		~Piece();
		virtual bool valid_moves(bool [][8], Board&){return 0;}
		bool operator==(int);
		bool operator>=(int);
		bool operator<=(int);
//		bool operator<(int);
//		bool operator>(int);
		void setxy(int, int);
		int getside();
		int getname();
};



class King: public Piece{
	public:
		King(int, int, int);
		bool valid_moves(bool [][8], Board&);
};

class Queen: public Piece{
	public:
		Queen(int, int, int);
		bool valid_moves(bool [][8], Board&);
};

class Rook: public Piece{
	public:
		Rook(int, int, int);
		bool valid_moves(bool [][8], Board&);
};

class Knight: public Piece{
	public:
		Knight(int, int, int);
		bool valid_moves(bool [][8], Board&);
};

class Bishop: public Piece{
	public:
		Bishop(int, int, int);
		bool valid_moves(bool [][8], Board&);
};

class Pawn: public Piece{
	public:
		Pawn(int, int, int);
		bool valid_moves(bool [][8], Board&);
};

class Air: public Piece{
	public:
		Air(int, int);
		bool valid_moves(bool [][8], Board&);
};

#endif
