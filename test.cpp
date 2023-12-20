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
		}
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
