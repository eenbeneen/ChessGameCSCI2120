#include <iostream>

//This function prints the board to the output window in proper format
void showBoard(char board[][8]) {
	
	for (int i = 0; i < 8; i++) {
		
		std::cout << 8-i << "  ";
		for (int j = 0; j < 8; j++) {
			std::cout << " " << board[i][j];

		}
		std::cout << std::endl;

	}
	std::cout << std::endl << "    ";
	for (int i = 0; i < 8; i++) {
		char c = 'A' + i;
		std::cout << c << " ";
	}
	

}

int main() {

	//The starting chess board
	/*
		Uppercase: white
		Lowercase: black
		P = pawn
		R = rook
		N = knight
		B = bishop
		Q = queen
		K = king
	*/
	char board[8][8] = {

		{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},

	};


	showBoard(board);




	return 0;
}