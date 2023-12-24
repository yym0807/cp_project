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
	public:
		Board();
		~Board();
		Piece***& getboard();
		void unloadimage();
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
		void renderpieces();
		void promotion(int, int, int);
		bool king_died(bool);
};

class Piece{
	friend class Board;
	protected:
		int p;
		int side; // 0 for white, 1 for black, -1 for air
		int x; // 8 ~ 1
		int y; // a ~ h
		bool freeze;
		Image img;
	public:
		Piece(int, int, int, int);
		~Piece();
		virtual bool valid_moves(bool [][8], Board&, bool consider_check){return 0;}
		bool operator==(int);
		void setxy(int, int);
		int getside();
		void rerender();
		virtual void loadResultImage(std::string){}
		void reloadImage(std::string);
		bool getfreeze();
		bool freezed();
		bool unfreezed();
};



class King: public Piece{
	public:
		King(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check);
		void loadResultImage(std::string);
};

class Queen: public Piece{
	public:
		Queen(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check);
};

class Rook: public Piece{
	public:
		Rook(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check);
};

class Knight: public Piece{
	public:
		Knight(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check);
};

class Bishop: public Piece{
	public:
		Bishop(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check);
};

class Pawn: public Piece{
	public:
		Pawn(int, int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check);
};

class Air: public Piece{
	public:
		Air(int, int);
		bool valid_moves(bool [][8], Board&, bool consider_check);
};

#endif
