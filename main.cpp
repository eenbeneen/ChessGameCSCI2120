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
	int xPos;
	int yPos;
	//Boolean thats true if the piece is white, false if black
	bool isWhite;
	
	//Set the x position and y position, of the piece and if it's white (t/f)
	Piece(int x, int y, bool white) {
		xPos = x;
		yPos = y;
		isWhite = white;
	}

	//Get moves function that currently returns an empty vector of int vectors
	std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;
		return possibleMoves;
	}

};

class Pawn: public Piece {
public:

	Pawn(int x, int y, bool white) : Piece(x, y, white) {}

	std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;

	}
};

class Rook : public Piece {
public:

	Rook(int x, int y, bool white) : Piece(x, y, white) {}

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

	Bishop(int x = 3, int y = 3, bool white = true) : Piece(x, y, white) {}


	std::vector<std::vector<int>> getMoves(char board[][8]) {

		std::vector<std::vector<int>> possibleMoves;

		//The first two for loops switch between the + or minus x and y direction, 4 loops total
		for (int xdir = -1; xdir <= 1; xdir += 2) {

			for (int ydir = -1; ydir <= 1; ydir += 2) {
				
				//For each direction, i the number of spaces away from the current position
				for (int i = 1; i < 8 && i >= 0; i++) {
					//Check if the space 'i' spaces away in the correct direction is empty
					if (board[xPos + i*xdir][yPos + i*ydir] == 0 ) {
						//The space is empty, therefore add it to possible moves
						possibleMoves.push_back(convertToVector(xPos + i*xdir, yPos + i*ydir));
					}
					else {
						//The space is not empty, end the current path
						//Add capture case later
						break;
					}
				}
			}

			
		}

		return possibleMoves;


	}
};

class Queen : public Piece {
public:

	Queen(int x, int y, bool white) : Piece(x, y, white) {}


	std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;

		//Simply use the getMoves function of both the bishop and the rook and combine them
		Rook rook(xPos, yPos, isWhite);
		Bishop bishop(xPos, yPos, isWhite);
		possibleMoves = rook.getMoves(board);
		std::vector<std::vector<int>> bishopMoves = bishop.getMoves(board);
		possibleMoves.insert(possibleMoves.end(), bishopMoves.begin(), bishopMoves.end());

		return possibleMoves;

	}
};

class King : public Piece {
public:

	King(int x = 0, int y = 0, bool white = true) : Piece(x, y, white) {}


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

		Board goes from top left (0, 0) to bottom right (7, 7)
	*/
	char board[8][8] = {

		{0, 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
		{0, 0, 'p', 'p', 'p', 'p', 'p', 'p'},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},

	};


	showBoard(board);

	//Testing rook at position (3, 3)
	Rook rook(3, 3, true);

	std::vector<std::vector<int>> rookMoves = rook.getMoves(board);
	std::cout << "\n ROOKMOVES SIZE: " << rookMoves.size() << " \n";

	for (int i = 0; i < rookMoves.size(); i++) {
		
		std::cout << "(";
		std::cout << rookMoves.at(i).at(0) << ", " << rookMoves.at(i).at(1);
		std::cout << ")\n";
		
	}

	//Testing bishop at position (3, 3)
	Bishop bishop(3, 3, true);

	std::vector<std::vector<int>> bishopMoves = bishop.getMoves(board);
	std::cout << "\n BISHOPMOVES SIZE: " << bishopMoves.size() << " \n";

	for (int i = 0; i < bishopMoves.size(); i++) {

		std::cout << "(";
		std::cout << bishopMoves.at(i).at(0) << ", " << bishopMoves.at(i).at(1);
		std::cout << ")\n";

	}

	//Testing queen at position (3, 3)
	Queen queen(3, 3, true);

	std::vector<std::vector<int>> queenMoves = queen.getMoves(board);
	std::cout << "\n QUEENMOVES SIZE: " << queenMoves.size() << " \n";

	for (int i = 0; i < queenMoves.size(); i++) {

		std::cout << "(";
		std::cout << queenMoves.at(i).at(0) << ", " << queenMoves.at(i).at(1);
		std::cout << ")\n";

	}



	return 0;
}