#include <iostream>
#include <vector>

//Function to easily convert 2 coordinates to a vector
std::vector<int> convertToVector(int x, int y) {
	std::vector<int> vec = { x, y };
	return vec;
}

class Piece {
public:
	//Position variables that can be 0-7
	int xPos = 0;
	int yPos = 0;
	//Boolean thats true if the piece is white, false if black
	bool isWhite;
	//Get moves function that currently returns an empty vector of int vectors
	std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;
		return possibleMoves;
	}

};

class Pawn: public Piece {
public:
	std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;

	}
};

class Rook : public Piece {
public:
	std::vector<std::vector<int>> getMoves(char board[][8]) {

		
		std::vector<std::vector<int>> possibleMoves;
		
		//This code checks the rook's y (vertical) path
		//The for loop runs twice, once with dir = 1 and once with dir = -1
		for (int dir = 1; dir >= -1; dir -= 2) {
			//This for loop checks every space in front of the rook when dir = 1
			//and every space behind the rook when dir = -1
			for (int i = xPos + dir; i < 8 && i >= 0; i+=dir) {
				
				if (board[i][yPos] == 0) {
					//If the space is 0, then the rook can move there, add it to the pos. moves
					possibleMoves.push_back(convertToVector(i, yPos));
				}
				else {
					//If not, break the loop
					//Add checking for captures later
					break;
				}

			}
		}

		//Same here, but the for loop checks the rooks path along x
		for (int dir = 1; dir >= -1; dir -= 2) {
			for (int i = yPos + dir; i < 8 && i >= 0; i+=dir) {
				if (board[xPos][i] == 0) {
					possibleMoves.push_back(convertToVector(xPos, i));
				}
				else {
					break;
				}

			}
		}

		return possibleMoves;
		
	}
};

class Knight : public Piece {
public:
	std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;

	}
};

class Bishop : public Piece {
public:
	std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;

	}
};

class Queen : public Piece {
public:
	std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;

	}
};

class King : public Piece {
public:
	std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;

	}
};

	
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
		UPPERCASE: WHITE
		lowercase: black
		P = pawn
		R = rook
		N = knight	
		B = bishop
		Q = queen
		K = king
	*/
	char board[8][8] = {

		{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
		{0, 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},

	};


	showBoard(board);

	Rook rook;

	std::vector<std::vector<int>> rookMoves = rook.getMoves(board);
	std::cout << "\n ROOKMOVES SIZE: " << rookMoves.size() << " \n";
	for (int i = 0; i < rookMoves.size(); i++) {
		
		for (int j = 0; j < rookMoves.at(i).size(); j++) {
			std::cout << "(";
			std::cout << rookMoves.at(i).at(0) << ", " << rookMoves.at(i).at(1);
			std::cout << ")\n";
		}
		
	}





	return 0;
}