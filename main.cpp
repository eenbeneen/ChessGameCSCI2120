#include <iostream>
#include <vector>
#include <iomanip> // For std::setw

#define white_square 0xDB
#define black_square 0xFF

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

		//Check if The pawn is white, and if it isn't at the end of the board
		if (isWhite && yPos > 0) {
			//Run this if the pawn is white and not at the end

			if (board[yPos - 1][xPos] == ' ') {

				//The spot in front of the pawn is open, so it can move there
				possibleMoves.push_back(convertToVector(xPos, yPos - 1));

				//If the pawn is at the starting square, it can move 2 spaces if its open
				if (yPos == 6 && board[yPos - 2][xPos] == ' ') {
					possibleMoves.push_back(convertToVector(xPos, yPos - 2));
				}

			}

			//Add en passant and capturing here later

		}
		else if (!isWhite && yPos < 7) {
			//Run this if the pawn is black and not at the end
			//More or less the same code with different numbers
			if (board[yPos + 1][xPos] == ' ') {

				possibleMoves.push_back(convertToVector(xPos, yPos + 1));

				if (yPos == 1 && board[yPos + 2][xPos] == ' ') {
					possibleMoves.push_back(convertToVector(xPos, yPos + 2));
				}
			}
		}

		return possibleMoves;

		//Note, promotion belongs in the main function

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
				
				if (board[i][yPos] == ' ') {
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
				if (board[xPos][i] == ' ') {
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
					if (board[xPos + i*xdir][yPos + i*ydir] == ' ') {
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

		//Start a loop to check every space next to the king
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {

				//If both i and j are 0, the space is the one the king is\
				standing on and doesn't need to be checked
				if (i == 0 && j == 0) {
					continue;
				}

				//If the space is empty, add it to the possible moves
				if (board[xPos + i][yPos + j] == ' ') {
					possibleMoves.push_back(convertToVector(xPos + i, yPos + j));
				}
			}
		}

		return possibleMoves;

	}
};

	
//This function prints the board to the output window in proper format
// Function to display the chessboard with alternating black and white squares
void showBoard(char board[][8]) {
	const int square_width = 5; // Fixed width for each square (including borders and padding)

	for (int i = 0; i < 8; i++) {
		std::cout << 8 - i << " "; // Row number
		for (int j = 0; j < 8; j++) {
			// Determine the square type (alternating black and white squares)
			if ((i + j) % 2 == 0) {
				// White square with black text
				std::cout << "\033[47m\033[30m "; // White background, black text using ANSI escape
				std::cout << std::setw(square_width - 2) << board[i][j]; // Center the piece
				std::cout << " \033[0m"; // Reset colors
			}
			else {
				// Black square with white text
				std::cout << "\033[40m\033[37m "; // Black background, white text using ANSI escape
				std::cout << std::setw(square_width - 2) << board[i][j]; // Center the piece
				std::cout << " \033[0m"; // Reset colors
			}
		}
		std::cout << std::endl;
	}

	// Display column labels
	std::cout << "   ";
	for (int i = 0; i < 8; i++) {
		char c = 'A' + i;
		std::cout << "  " << c << "  ";
	}
	std::cout << std::endl;
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

		{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
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

	//Testing black pawn at (1, 1)
	Pawn pawn(1, 1, false);

	std::vector<std::vector<int>> pawnMoves = pawn.getMoves(board);
	std::cout << "\n PAWNMOVES SIZE: " << pawnMoves.size() << " \n";

	for (int i = 0; i < pawnMoves.size(); i++) {

		std::cout << "(";
		std::cout << pawnMoves.at(i).at(0) << ", " << pawnMoves.at(i).at(1);
		std::cout << ")\n";

	}

	//Testing king at (3, 3)
	King king(3, 3, true);

	std::vector<std::vector<int>> kingMoves = king.getMoves(board);
	std::cout << "\n KINGMOVES SIZE: " << kingMoves.size() << " \n";

	for (int i = 0; i < kingMoves.size(); i++) {

		std::cout << "(";
		std::cout << kingMoves.at(i).at(0) << ", " << kingMoves.at(i).at(1);
		std::cout << ")\n";

	}

	

	return 0;
}

/* ANSI wscape
Foreground colors:
	Black: \033[30m
	Red: \033[31m
	Green: \033[32m
	Yellow: \033[33m
	Blue: \033[34m
	Magenta: \033[35m
	Cyan: \033[36m
	White: \033[37m

Background colors:
	Black: \033[40m
	Red: \033[41m
	Green: \033[42m
	Yellow: \033[43m
	Blue: \033[44m
	Magenta: \033[45m
	Cyan: \033[46m
	White: \033[47m

*/