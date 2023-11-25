#include <iostream>
#include <cstdlib> 
#include <cstring>
using namespace std;

char board[8][8] = {
	'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
	'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I',
	'*', '*', '*', '*', '*', '*', '*', '*',
	'*', '*', '*', '*', '*', '*', '*', '*',
	'*', '*', '*', '*', '*', '*', '*', '*',
	'*', '*', '*', '*', '*', '*', '*', '*',
	'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i',
	'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r',
};

bool oo[2] = {1, 1};
bool ooo[2] = {1, 1};
bool turn = 0; // 0: white 1: black
const int dirs[8][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1}}; // URDLUrDrDlUl
const int h[8][2] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
int passant = -2;

int check(){
	int checkcount = 0;
	int x, y;
	for(x = 0; x < 8; x++){//找到國王位置 
		for(y = 0; y < 8; y++){
			if(board[x][y] == "kK"[turn]) break;
		}
		if(y < 8) break;
	}
	for(int i = 0; i < 8; i++){//king check
		if(x+dirs[i][0] >= 0 && x+dirs[i][0] < 8 && y+dirs[i][1] >= 0 && y+dirs[i][1] < 8){
			if(board[x+dirs[i][0]][y+dirs[i][1]] == "Kk"[turn]){
				checkcount++;
				break;
			}	
		}
	}
	if(x > 0 && !turn || x < 7 && turn){//pawn check 
		if(!turn && (y > 0 && board[x-1][y-1] == 'I' || y < 7 && board[x-1][y+1] == 'I') || turn && (y > 0 && board[x+1][y-1] == 'i' || y < 7 && board[x+1][y+1] == 'i')){
			checkcount++;
		}
	}
	for(int i = 0; i < 4; i++){//直線武器 
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t] == '*'){
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t] == "Qq"[turn] || board[x+dirs[i][0]*t][y+dirs[i][1]*t] == "Rr"[turn]){
				checkcount++;
			}
			break;//有東西擋著就break 
		}
	}
	for(int i = 4; i < 8; i++){//斜線武器 
		int t = 1;
		while(x+dirs[i][0]*t >= 0 && x+dirs[i][0]*t < 8 && y+dirs[i][1]*t >= 0 && y+dirs[i][1]*t < 8){
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t] == '*'){
				t++;
				continue;
			}
			if(board[x+dirs[i][0]*t][y+dirs[i][1]*t] == "Qq"[turn] || board[x+dirs[i][0]*t][y+dirs[i][1]*t] == "Bb"[turn]){
				checkcount++;
			}
			break;
		}
	}
	for(int i = 0; i < 8; i++){//knight check 
		if(x+h[i][0] >= 0 && x+h[i][0] < 8 && y+h[i][1] >= 0 && y+h[i][1] < 8){
			if(board[x+h[i][0]][y+h[i][1]] == "Nn"[turn]){
				checkcount++;
			}
		}
	}
	return checkcount;
}

bool check_if_move(int xi, int yi, int xf, int yf){//判斷是否自殺 
	char tem = board[xf][yf];
	board[xf][yf] = board[xi][yi];
	board[xi][yi] = '*';
	int c = check();
	board[xi][yi] = board[xf][yf];
	board[xf][yf] = tem;
	return c;
}

bool valid_moves(int xi, int yi, bool vm[][8], bool output = 1){//輸出有哪些合法走法 
	if(xi * (xi-7) > 0 || yi * (yi-7) > 0){ 
		cout << "Invalid\n";
		cout << "Press enter to choose again...";
		getchar();
		getchar();
		return 0;
	}
	if(board[xi][yi] == '*' || board[xi][yi] < 'a' && !turn || board[xi][yi] > 'Z' && turn){//選到無子/對手旗子 
		cout << "Invalid\n";
		cout << "Press enter to choose again...";
		getchar();
		getchar();
		return 0;
	}
	if(output) cout << "Valid moves: ";
	bool flag = 0;
	bool ch = 0;
	switch(board[xi][yi]){
		case 'i':
			if(board[xi-1][yi] == '*'){
				if(!check_if_move(xi, yi, xi-1, yi)){
					flag = 1;
					vm[xi-1][yi] = 1;
					if(output) cout << (char)('a' + yi) << 8 - (xi-1) << ' ';
				}
				if(xi == 6 && board[xi-2][yi] == '*'&& !check_if_move(xi, yi, xi-2, yi)){
					flag = 1;
					vm[xi-2][yi] = 1;
					if(output) cout << (char)('a' + yi) << 8 - (xi-2) << ' ';
				}
			}
			if(yi < 7 && board[xi-1][yi+1] < 'Z' && board[xi-1][yi+1] > 'A' && !check_if_move(xi, yi, xi-1, yi+1)){
				flag = 1;
				vm[xi-1][yi+1] = 1;
				if(output) cout << (char)('a' + yi + 1) << 8 - (xi-1) << ' ';
			}
			if(yi > 0 && board[xi-1][yi-1] < 'Z' && board[xi-1][yi-1] > 'A' && !check_if_move(xi, yi, xi-1, yi-1)){
				flag = 1;
				vm[xi-1][yi-1] = 1;
				if(output) cout << (char)('a' + yi - 1) << 8 - (xi-1) << ' ';
			}
			if(xi == 3 && (passant == yi + 1 || passant == yi - 1)){
				flag = 1;
				vm[xi-1][passant] = 1;
				if(output) cout << (char)('a' + passant) << 8 - (xi-1) << ' ';
			}
			break;
		case 'I':
			if(board[xi+1][yi] == '*'){
				if(!check_if_move(xi, yi, xi+1, yi)){
					flag = 1;
					vm[xi+1][yi] = 1;
					if(output) cout << (char)('a' + yi) << 8 - (xi+1) << ' ';
				}
				if(xi == 1 && board[xi+2][yi] == '*' && !check_if_move(xi, yi, xi+2, yi)){
					flag = 1;
					vm[xi+2][yi] = 1;
					if(output) cout << (char)('a' + yi) << 8 - (xi+2) << ' ';
				}
			}
			if(yi < 7 && board[xi+1][yi+1] < 'z' && board[xi+1][yi+1] > 'a' && !check_if_move(xi, yi, xi+1, yi+1)){
				flag = 1;
				vm[xi+1][yi+1] = 1;
				if(output) cout << (char)('a' + yi + 1) << 8 - (xi+1) << ' ';
			}
			if(yi > 0 && board[xi+1][yi-1] < 'z' && board[xi+1][yi-1] > 'a' && !check_if_move(xi, yi, xi+1, yi-1)){
				flag = 1;
				vm[xi+1][yi-1] = 1;
				if(output) cout << (char)('a' + yi - 1) << 8 - (xi+1) << ' ';
			}
			if(xi == 4 && (passant == yi + 1 || passant == yi - 1)){
				flag = 1;
				vm[xi+1][passant] = 1;
				if(output) cout << (char)('a' + passant) << 8 - (xi+1) << ' ';
			}
			break;
		case 'K':
		case 'k':
			for(int i = 0; i < 8; i++){
				if(xi+dirs[i][0] >= 0 && xi+dirs[i][0] < 8 && yi+dirs[i][1] >= 0 && yi+dirs[i][1] < 8 && (board[xi+dirs[i][0]][yi+dirs[i][1]] < board[xi][yi] - 10 || board[xi+dirs[i][0]][yi+dirs[i][1]] > board[xi][yi] + 15) && !check_if_move(xi, yi, xi+dirs[i][0], yi+dirs[i][1])){
					flag = 1;
					vm[xi+dirs[i][0]][yi+dirs[i][1]] = 1;
					if(output) cout << 'K' << (char)('a' + yi + dirs[i][1]) << 8 - (xi+dirs[i][0]) << ' ';
				}
			}
			if(ooo[turn]){
				if(!check() && board[xi][1] == '*' && board[xi][2] == '*' && !check_if_move(xi, 4, xi, 2) && board[xi][3] == '*' && !check_if_move(xi, 4, xi, 3)){
					flag = 1;
					vm[xi][2] = 1;
					if(output) cout << "OOO ";
				}
			}
			if(oo[turn]){
				if(!check() && board[xi][5] == '*' && !check_if_move(xi, 4, xi, 5) && board[xi][6] == '*' && !check_if_move(xi, 4, xi, 6)){
					flag = 1;
					vm[xi][6] = 1;
					if(output) cout << "OO ";
				}
			}
			break;
		case 'Q':
		case 'q':
			for(int i = 0; i < 8; i++){
				int t = 1;
				while(xi+dirs[i][0]*t >= 0 && xi+dirs[i][0]*t < 8 && yi+dirs[i][1]*t >= 0 && yi+dirs[i][1]*t < 8){
					if(board[xi+dirs[i][0]*t][yi+dirs[i][1]*t] == '*' && !check_if_move(xi, yi, xi+dirs[i][0]*t, yi+dirs[i][1]*t)){
						flag = 1;
						vm[xi+dirs[i][0]*t][yi+dirs[i][1]*t] = 1;
						if(output) cout << 'Q' << (char)('a' + yi+dirs[i][1]*t) << 8 - (xi+dirs[i][0]*t) << ' ';
						t++;
						continue;
					}
					if((board[xi+dirs[i][0]*t][yi+dirs[i][1]*t] < board[xi][yi] - 16 || board[xi+dirs[i][0]*t][yi+dirs[i][1]*t] > board[xi][yi] + 9) && !check_if_move(xi, yi, xi+dirs[i][0]*t, yi+dirs[i][1]*t)){
						flag = 1;
						vm[xi+dirs[i][0]*t][yi+dirs[i][1]*t] = 1;
						if(output) cout << 'Q' << (char)('a' + yi+dirs[i][1]*t) << 8 - (xi+dirs[i][0]*t) << ' ';
					}
					break;
				}
			}
			break;
		case 'B':
		case 'b':
			for(int i = 4; i < 8; i++){
				int t = 1;
				while(xi+dirs[i][0]*t >= 0 && xi+dirs[i][0]*t < 8 && yi+dirs[i][1]*t >= 0 && yi+dirs[i][1]*t < 8 && !check_if_move(xi, yi, xi+dirs[i][0]*t, yi+dirs[i][1]*t)){
					if(board[xi+dirs[i][0]*t][yi+dirs[i][1]*t] == '*'){
						flag = 1;
						vm[xi+dirs[i][0]*t][yi+dirs[i][1]*t] = 1;
						if(output) cout << 'B' << (char)('a' + yi+dirs[i][1]*t) << 8 - (xi+dirs[i][0]*t) << ' ';
						t++;
						continue;
					}
					if((board[xi+dirs[i][0]*t][yi+dirs[i][1]*t] < board[xi][yi] - 1 || board[xi+dirs[i][0]*t][yi+dirs[i][1]*t] > board[xi][yi] + 24) && !check_if_move(xi, yi, xi+dirs[i][0]*t, yi+dirs[i][1]*t)){
						flag = 1;
						vm[xi+dirs[i][0]*t][yi+dirs[i][1]*t] = 1;
						if(output) cout << 'B' << (char)('a' + yi+dirs[i][1]*t) << 8 - (xi+dirs[i][0]*t) << ' ';
					}
					break;
				}
			}
			break;
		case 'N':
		case 'n':
			for(int i = 0; i < 8; i++){
				if(xi+h[i][0] >= 0 && xi+h[i][0] < 8 && yi+h[i][1] >= 0 && yi+h[i][1] < 8 && (board[xi+h[i][0]][yi+h[i][1]] < board[xi][yi] - 13 || board[xi+h[i][0]][yi+h[i][1]] > board[xi][yi] + 12) && !check_if_move(xi, yi, xi+h[i][0], yi+h[i][1])){
					flag = 1;
					vm[xi+h[i][0]][yi+h[i][1]] = 1;
					if(output) cout << 'N' << (char)('a' + yi+h[i][1]) << 8 - (xi+h[i][0]) << ' ';
				}
			}
			break;
		case 'R':
		case 'r':
			for(int i = 0; i < 4; i++){
				int t = 1;
				while(xi+dirs[i][0]*t >= 0 && xi+dirs[i][0]*t < 8 && yi+dirs[i][1]*t >= 0 && yi+dirs[i][1]*t < 8 && !check_if_move(xi, yi, xi+dirs[i][0]*t, yi+dirs[i][1]*t)){
					if(board[xi+dirs[i][0]*t][yi+dirs[i][1]*t] == '*'){
						flag = 1;
						vm[xi+dirs[i][0]*t][yi+dirs[i][1]*t] = 1;
						if(output) cout << 'R' << (char)('a' + yi+dirs[i][1]*t) << 8 - (xi+dirs[i][0]*t) << ' ';
						t++;
						continue;
					}
					if((board[xi+dirs[i][0]*t][yi+dirs[i][1]*t] < board[xi][yi] - 17 || board[xi+dirs[i][0]*t][yi+dirs[i][1]*t] > board[xi][yi] + 8) && !check_if_move(xi, yi, xi+dirs[i][0]*t, yi+dirs[i][1]*t)){
						flag = 1;
						vm[xi+dirs[i][0]*t][yi+dirs[i][1]*t] = 1;
						if(output) cout << 'R' << (char)('a' + yi+dirs[i][1]*t) << 8 - (xi+dirs[i][0]*t) << ' ';
					}
					break;
				}
			}
			break;
		default:
			cout << "ERROR\n";
			break;
	}
	if(flag == 0){
		if(output){
			cout << "None\n";
			cout << "Press enter to choose again...";
			getchar();
			getchar();
		}
		return 0;
	}
	if(output) cout << '\n';
	return 1;
}


bool any_valid(){
	bool vm[8][8] = {};
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(board[i][j] >= "aA"[turn] && board[i][j] <= "zZ"[turn]){
				if(valid_moves(i, j, vm, 0)){
					return 1;
				}
			}
		}
	}
	return 0;
}

bool checkmate(){
	if(!any_valid() && check()) return 1;
	return 0;
}

bool stalemate(){
	if(!any_valid() && !check()) return 1;
	bool n = 0, N = 0, b = 0, B = 0;
	bool bcolor, Bcolor;
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(board[i][j] == '*' || board[i][j] == 'K' || board[i][j] == 'k') continue;
			else if(board[i][j] == 'N'){
				if(N || B || n || b) return 0;
				N = 1;
			}
			else if(board[i][j] == 'n'){
				if(N || B || n || b) return 0;
				n = 1;
			}
			else if(board[i][j] == 'B'){
				if(N || B || n || b && bcolor != ((i + j) & 1)) return 0;
				B = 1;
				Bcolor = (i + j) & 1;
			}
			else if(board[i][j] == 'b'){
				if(N || b || n || B && Bcolor != ((i + j) & 1)) return 0;
				b = 1;
				bcolor = (i + j) & 1;
			}
			else{
				return 0;
			}
		}
	}
	return 1;
}

int main(){
	int xi, yi;
	char s[3];
	const char side[2][6] = {"white", "black"};
	while(!checkmate() && !stalemate()){
//		system("cls");
		char m = 'Q';
		for(int i = 0; i < 8; i++){
			cout << 8 - i << " |";
			for(int j = 0; j < 8; j++){
				cout << board[i][j] << ' ';
			}
			cout << '\n';
		}
		cout << "  ________________\n   a b c d e f g h\n";
		cout << '[' << side[turn] << "] Location of your piece: ";
		cin >> s;
		xi = '8' - s[1];
		yi = s[0] - 'a';
		bool vm[8][8] = {};
		if(!valid_moves(xi, yi, vm)){
			continue;
		}
		cout << '[' << side[turn] << "] Your move: ";
		cin >> s;
		if(!strcmp(s, "OOO")){
			if(board[xi][yi] == 'k' && vm[7][2]){
				board[7][0] = '*';
				board[7][2] = 'k';
				board[7][3] = 'r';
				board[7][4] = '*';
			}
			else if(board[xi][yi] == 'K' && vm[0][2]){
				board[0][0] = '*';
				board[0][2] = 'K';
				board[0][3] = 'R';
				board[0][4] = '*';
			}
			else{
				cout << "Invalid\n";
				cout << "Press enter to choose again...";
				getchar();
				getchar();
				continue;
			}
			oo[turn] = 0;
			ooo[turn] = 0;
		}
		else if(!strcmp(s, "OO")){
			if(board[xi][yi] == 'k' && vm[7][6]){
				board[7][4] = '*';
				board[7][5] = 'r';
				board[7][6] = 'k';
				board[7][7] = '*';
			}
			else if(board[xi][yi] == 'K' && vm[0][6]){
				board[0][4] = '*';
				board[0][5] = 'R';
				board[0][6] = 'K';
				board[0][7] = '*';
			}
			else{
				cout << "Invalid\n";
				cout << "Press enter to choose again...";
				getchar();
				getchar();
				continue;
			}
			oo[turn] = 0;
			ooo[turn] = 0;
		}
		else{
			if(!vm['8'-s[1]][s[0]-'a']){
				cout << "Invalid\n";
				cout << "Press enter to choose again...";
				getchar();
				getchar();
				continue;
			}
			if(board[xi][yi] == 'K' || board[xi][yi] == 'k'){
				oo[board[xi][yi]=='K'] = 0;
				ooo[board[xi][yi]=='K'] = 0;
			}
			if(board[xi][yi] == 'R' || board[xi][yi] == 'r'){
				if(xi == 0 && yi == 0) ooo[0] = 0;
				if(xi == 0 && yi == 7) oo[0] = 0;
				if(xi == 7 && yi == 0) ooo[1] = 0;
				if(xi == 7 && yi == 7) oo[1] = 0;
			}
			if(xi == 1 && s[1] == '5' && board[xi][yi] == 'I' || xi == 6 && s[1] == '4' && board[xi][yi] == 'i'){
				passant = yi;
			}
			else{
				passant = 100;
			}
			if(xi == 6 && board[xi][yi] == 'I' || xi == 1 && board[xi][yi] == 'i'){
				cout << '[' << side[turn] << "] Pawn promotion(Q, B, N, R): ";
				char p;
				cin >> p;
				board['8'-s[1]][s[0]-'a'] = (char)(p + !turn * 32);
				board[xi][yi] = '*';
			}
			else if((board[xi][yi] == 'I' || board[xi][yi] == 'i') && yi != s[0]-'a' && board['8'-s[1]][s[0]-'a'] == '*'){
				//passant
				board['8'-s[1]][s[0]-'a'] = board[xi][yi];
				board[xi][yi] = '*';
				board[xi][s[0]-'a'] = '*';
			}
			else{
				board['8'-s[1]][s[0]-'a'] = board[xi][yi];
				board[xi][yi] = '*';
			}
		}
		turn = !turn;
	}
	if(checkmate()){
		cout << "Winner is " << side[!turn] << '\n';
	}
	else if(stalemate()){
		cout << "It's a stalemate\n";
	}
	else{
		cout << "ERROR\n";
	}
	return 0;
}  
