#include <iostream>
#include <cstdlib> 
#include <cstring>
#include "game.cpp"
using namespace std;

int main(){
	Board b;
	char s[3];
	int xi, yi;
	const char side[2][6] = {"white", "black"};
	while(!b.checkmate() && !b.stalemate()){
		Piece*** board = b.getboard();
		bool turn = b.getturn();
//		system("cls");
//		char m = 'Q';
		b.print();
		cout << '[' << side[turn] << "] Location of your piece: ";
		cin >> s;
		xi = '8' - s[1];
		yi = s[0] - 'a';
		bool vm[8][8] = {};
		if(!board[xi][yi]->valid_moves(vm, b)){
			continue;
		}
		cout << '[' << side[turn] << "] Your move: ";
		cin >> s;
		if(!strcmp(s, "OOO")){
			if(*board[xi][yi] == KING && vm[7-board[xi][yi]->getside() * 7][2]){
				b.move(xi, yi, xi, 2);
			}
			else{
				cout << "Invalid\n";
				cout << "Press enter to choose again...";
				getchar();
				getchar();
				continue;
			}
		}
		else if(!strcmp(s, "OO")){
			if(*board[xi][yi] == KING && vm[7 - board[xi][yi]->getside() * 7][6]){
				b.move(xi, yi, xi, 6);
			}
			else{
				cout << "Invalid\n";
				cout << "Press enter to choose again...";
				getchar();
				getchar();
				continue;
			}
		}
		else{
			if(!vm['8'-s[1]][s[0]-'a']){
				cout << "Invalid\n";
				cout << "Press enter to choose again...";
				getchar();
				getchar();
				continue;
			}
			b.move(xi, yi, '8'-s[1], s[0]-'a');
//			if(board[xi][yi] == 'K' || board[xi][yi] == 'k'){
//				oo[board[xi][yi]=='K'] = 0;
//				ooo[board[xi][yi]=='K'] = 0;
//			}
//			if(board[xi][yi] == 'R' || board[xi][yi] == 'r'){
//				if(xi == 0 && yi == 0) ooo[0] = 0;
//				if(xi == 0 && yi == 7) oo[0] = 0;
//				if(xi == 7 && yi == 0) ooo[1] = 0;
//				if(xi == 7 && yi == 7) oo[1] = 0;
//			}
//			if(xi == 1 && s[1] == '5' && board[xi][yi] == 'I' || xi == 6 && s[1] == '4' && board[xi][yi] == 'i'){
//				passant = yi;
//			}
//			else{
//				passant = 100;
//			}
//			if(xi == 6 && board[xi][yi] == 'I' || xi == 1 && board[xi][yi] == 'i'){
//				cout << '[' << side[turn] << "] Pawn promotion(Q, B, N, R): ";
//				char p;
//				cin >> p;
//				board['8'-s[1]][s[0]-'a'] = (char)(p + !turn * 32);
//				board[xi][yi] = '*';
//			}
//			else if((board[xi][yi] == 'I' || board[xi][yi] == 'i') && yi != s[0]-'a' && board['8'-s[1]][s[0]-'a'] == '*'){
//				//passant
//				board['8'-s[1]][s[0]-'a'] = board[xi][yi];
//				board[xi][yi] = '*';
//				board[xi][s[0]-'a'] = '*';
//			}
//			else{
//				board['8'-s[1]][s[0]-'a'] = board[xi][yi];
//				board[xi][yi] = '*';
//			}
		}
//		turn = !turn;
	}
	if(b.checkmate()){
		cout << "Winner is " << side[!b.getturn()] << '\n';
	}
	else if(b.stalemate()){
		cout << "It's a stalemate\n";
	}
	else{
		cout << "ERROR\n";
	}
	return 0;
}
