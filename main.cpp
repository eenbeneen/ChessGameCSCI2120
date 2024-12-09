#include <iostream>
#include <vector>
#include <iomanip> // For std::setw
#include <string>
#include <wchar.h>
#include <locale.h>
#include <fcntl.h>
#include <io.h>

#define white_square 0xDB
#define black_square 0xFF

//Function to easily convert 2 coordinates to a vector
std::vector<int> convertToVector(int x, int y) {
	std::vector<int> vec = { x, y };
	return vec;
}

//Function to quickly check if a coordinate pair is in bounds of the board
bool coordInBounds(int x, int y) {
	return x < 8 && x >= 0 && y < 8 && y >= 0;
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
	virtual std::vector<std::vector<int>> getMoves(wchar_t board[][8]) {
		std::vector<std::vector<int>> possibleMoves;
		return possibleMoves;
	}

};

class Pawn: public Piece {
public:

	Pawn(int x, int y, bool white) : Piece(x, y, white) {}

	std::vector<std::vector<int>> getMoves(wchar_t board[][8]) override {

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

			//Check the squares diagonally in front of the pawn
			for (int i = -1; i <= 1; i += 2) {

				//If coords are in bounds and piece is lowercase (black) it can be captured
				if (coordInBounds(yPos - 1, xPos + i) && islower(board[yPos - 1][xPos + i])) {

					possibleMoves.push_back(convertToVector(xPos + i, yPos - 1));

				}
			}

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

	std::vector<std::vector<int>> getMoves(wchar_t board[][8]) override {

		
		std::vector<std::vector<int>> possibleMoves;
		
		//This code checks the rook's x (horizontal) path
		//The for loop runs twice, once with dir = 1 and once with dir = -1
		for (int dir = 1; dir >= -1; dir -= 2) {
			//This for loop checks every space in front of the rook when dir = 1
			//and every space behind the rook when dir = -1
			for (int i = xPos + dir; i < 8 && i >= 0; i+=dir) {

				//Break if the end of the board is reached
				if (!coordInBounds(yPos, i)) {
					break;
				}
				
				if (board[yPos][i] == ' ') {
					//If the space is 0, then the rook can move there, add it to the pos. moves
					possibleMoves.push_back(convertToVector(i, yPos));
				}
				else {
					//If not, check if it has a piece of a different color
					if (isupper(board[yPos][i]) != isWhite) {
						//The space has a piece of a different color, so it can capture
						possibleMoves.push_back(convertToVector(i, yPos));

					}
					//The rook cannot go further, so stop the current path
					break;
				}

			}
		}

		//Same here, but the for loop checks the rooks path along y
		for (int dir = 1; dir >= -1; dir -= 2) {
			for (int i = yPos + dir; i < 8 - yPos && i >= 0; i+=dir) {
				if (board[i][xPos] == ' ') {
					possibleMoves.push_back(convertToVector(xPos, i));
				}
				else {
					if (isupper(board[i][xPos]) != isWhite) {
						possibleMoves.push_back(convertToVector(xPos, i));
					}
					break;
				}

			}
		}

		return possibleMoves;
		
	}
};

class Knight : public Piece {
public:

	Knight(int x, int y, bool white) : Piece(x, y, white) {}

	std::vector<std::vector<int>> getMoves(wchar_t board[][8]) override {
		std::vector<std::vector<int>> possibleMoves;

		/*
		The knight can move in a way such that the movement along
		the x-axis and the movement along the y-axis is either 2 or
		1 in the + and - directions. The following arrays store the
		possible combination of moves in both the x and y axis such
		that element i of both arrays combine to make a possible move
		*/
		int xMoves[] = { -2, -1, 1, 2, -2, -1, 1, 2 };
		int yMoves[] = { -1, -2, 2, 1, 1, 2, -2, -1 };

		for (int i = 0; i < 8; i++) {

			//If the space is OOB, continue
			if (!coordInBounds(xPos + xMoves[i], yPos + yMoves[i])) {
				continue;
			}
			
			char to = board[yPos + yMoves[i]][xPos + xMoves[i]];
			//If the space is empty or it has an enemy piece, add it to the possible moves
			if (to == ' ' || (to != ' ' && isupper(to) != isWhite)) {

				possibleMoves.push_back(convertToVector(xPos + xMoves[i], yPos + yMoves[i]));

			}

			
		}

		return possibleMoves;

	}
};

class Bishop : public Piece {
public:

	Bishop(int x = 3, int y = 3, bool white = true) : Piece(x, y, white) {}


	std::vector<std::vector<int>> getMoves(wchar_t board[][8]) override {

		std::vector<std::vector<int>> possibleMoves;

		//The first two for loops switch between the + or minus x and y direction, 4 loops total
		for (int xdir = -1; xdir <= 1; xdir += 2) {

			for (int ydir = -1; ydir <= 1; ydir += 2) {
				
				//For each direction, i the number of spaces away from the current position
				for (int i = 1; i < 8 && i >= 0; i++) {

					//Break if the end of the board is reached
					if (!coordInBounds(yPos + i * ydir, xPos + i * xdir)) {
						break;
					}

					//Check if the space 'i' spaces away in the correct direction is empty
					if (board[yPos + i*ydir][xPos + i*xdir] == ' ') {

						//The space is empty, therefore add it to possible moves
						possibleMoves.push_back(convertToVector(xPos + i*xdir, yPos + i*ydir));
					}
					else {

						//The space is not empty, so check if there is an enemy piece
						if (isupper(board[yPos + i * ydir][xPos + i * xdir]) != isWhite) {

							//If there is an enemy piece, it can be captured
							possibleMoves.push_back(convertToVector(xPos + i * xdir, yPos + i * ydir));
						}

						//The bishop cannot go further, so end the current path
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


	std::vector<std::vector<int>> getMoves(wchar_t board[][8]) override {
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


	std::vector<std::vector<int>> getMoves(wchar_t board[][8]) override {
		std::vector<std::vector<int>> possibleMoves;

		

		//Start a loop to check every space next to the king
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {

				//Skip spaces that are OOB
				if (!coordInBounds(xPos + j, yPos + i)) {
					continue;
				}

				//If both i and j are 0, the space is the one the king is\
				standing on and doesn't need to be checked
				if (i == 0 && j == 0) {
					continue;
				}

				char to = board[yPos + i][xPos + j];
				//If the space is empty or has an enemy piece, add it to the possible moves
				if (to == ' ' || (to != ' ' && isupper(to) != isWhite)) {
					possibleMoves.push_back(convertToVector(xPos + j, yPos + i));
				}
			}
		}

		return possibleMoves;

	}
};


// Function to display the chessboard with alternating black and white squares
void showBoard(wchar_t board[8][8]) {
	// Row labels and board rendering
	for (int i = 0; i < 8; i++) {
		std::wcout << 8 - i << L" "; // Row numbers
		for (int j = 0; j < 8; j++) {
			// Alternate black and white squares
			if ((i + j) % 2 == 0) {
				// White square
				std::wcout << L"\033[47m\033[30m " << board[i][j] << L" \033[0m";
			}
			else {
				// Black square
				std::wcout << L"\033[40m\033[37m " << board[i][j] << L" \033[0m";;
			}
		}
		std::wcout << std::endl;
	}

	// Column labels
	std::wcout << L"   a  b  c  d  e  f  g  h" << std::endl;
}


// Function to initialize the chessboard with Unicode chess symbols
void initializeBoard(wchar_t board[8][8]) {
	// White pieces
	board[0][0] = board[0][7] = L'\u2656'; // Rooks
	board[0][1] = board[0][6] = L'\u2658'; // Knights
	board[0][2] = board[0][5] = L'\u2657'; // Bishops
	board[0][3] = L'\u2655'; // Queen
	board[0][4] = L'\u2654'; // King
	for (int i = 0; i < 8; ++i) {
		board[1][i] = L'\u2659'; // Pawns
	}

	// Black pieces
	board[7][0] = board[7][7] = L'\u265C'; // Rooks
	board[7][1] = board[7][6] = L'\u265E'; // Knights
	board[7][2] = board[7][5] = L'\u265D'; // Bishops
	board[7][3] = L'\u265B'; // Queen
	board[7][4] = L'\u265A'; // King
	for (int i = 0; i < 8; ++i) {
		board[6][i] = L'\u265F'; // Pawns
	}

	// Empty squares
	for (int i = 2; i < 6; ++i) {
		for (int j = 0; j < 8; ++j) {
			board[i][j] = L' '; // Empty squares
		}
	}
}
int main() {
	// Enable Unicode output for Windows (required for wide characters)
	_setmode(_fileno(stdout), _O_U16TEXT);

	wchar_t board[8][8];
	initializeBoard(board);

	// Explain notation for users
	std::cout << "Welcome to Chess! Please note the following information:\nUppercase pieces are white, and lowercase pieces are black.\nThe pieces are as follows:\nP = pawn, R = rook, N = knight, B = bishop, Q = queen, K = king\n\n";

	bool whiteTurn = true;
	while (true) {
		showBoard(board);
		// Get user input for move and validate input string format
		std::cout << (whiteTurn ? "White's turn." : "Black's turn.") << " Enter move (ex. E2 E4), I for info, or D to offer draw: ";
		std::string input;
		std::getline(std::cin, input);

		if (input.size() == 1) {
			if (input[0] == 'I') { // Display information if user wants to see it again
				std::cout << "Uppercase pieces are white, and lowercase pieces are black.\nThe pieces are as follows:\nP = pawn, R = rook, N = knight, B = bishop, Q = queen, K = king\n\n";
				continue;
			}
			else if (input[0] == 'D') { // Handle logic for offering a draw
				while (true) {
					std::cout << (whiteTurn ? "White" : "Black") << " is offering a draw. Do you accept (Y/N): ";
					std::getline(std::cin, input);
					if (input[0] == 'Y') {
						std::cout << "The game has ended in a tie!";
						return 0;
					}
					else if (input[0] == 'N') {
						std::cout << (whiteTurn ? "Black" : "White") << " has rejected the draw offer!\n\n";
						break;
					}
					else {
						std::cout << "Invalid input. Please try again!\n\n";
						continue;
					}
				}
				continue;
			}
		}
		else if (input.size() != 5 || input[2] != ' ') { // Validate user move input
			std::cout << "Invalid move input. Please try again!\n";
			continue;
		}

		// Parse user input into variables
		int fromX = input[0] - 'A';			// Column ('E' -> 4)
		int fromY = 8 - (input[1] - '0');	// Row (2 -> 6)
		int toX = input[3] - 'A';
		int toY = 8 - (input[4] - '0');

		std::vector<int> from = convertToVector(fromX, fromY);
		std::vector<int> to = convertToVector(toX, toY);

		// Make sure user's move is within chess board, otherwise prompt to try again
		if (!coordInBounds(fromX, fromY) || !coordInBounds(toX, toY)) {
			std::cout << "Coordinates out of bounds. Please try again!\n";
			continue;
		}

		char piece = board[fromY][fromX];
		// Make sure piece is valid, and white/black depending on turn
		if (piece == ' ' || (whiteTurn && islower(piece)) || (!whiteTurn && isupper(piece))) {
			std::cout << "Invalid piece selection. Please try again!\n";
			continue;
		}

		// Get piece from coordinates
		Piece* currentPiece = nullptr;
		switch (toupper(piece)) {
		case L'\u265F':  // White Pawn
		case L'\u2659':  // Black Pawn
			currentPiece = new Pawn(fromX, fromY, isupper(piece));
			break;

		case L'\u265C':  // White Rook
		case L'\u2656':  // Black Rook
			currentPiece = new Rook(fromX, fromY, isupper(piece));
			break;

		case L'\u2657':  // White Bishop
		case L'\u265D':  // Black Bishop
			currentPiece = new Bishop(fromX, fromY, isupper(piece));
			break;

		case L'\u2655':  // White Queen
		case L'\u265B':  // Black Queen
			currentPiece = new Queen(fromX, fromY, isupper(piece));
			break;

		case L'\u265A':  // White King
		case L'\u265A':  // Black King (Fix: only one case for King)
			currentPiece = new King(fromX, fromY, isupper(piece));
			break;

		case L'\u2658':  // White Knight
		case L'\u265E':  // Black Knight
			currentPiece = new Knight(fromX, fromY, isupper(piece));
			break;
		}
		if (!currentPiece) {
			std::cout << "Piece type invalid. Please try again!\n";
			continue;
		}

		// Iterate over possible moves to check user's inputted move
		std::vector<std::vector<int>> possibleMoves = currentPiece->getMoves(board);
		bool validMove = false;
		for (auto move : possibleMoves) {
			if (move[0] == toX && move[1] == toY) {
				validMove = true;
				break;
			}
		}

		if (!validMove) {
			std::cout << "Invalid move. Please try again!\n";
			delete currentPiece;
			continue;
		}

		// Promotion criteria, change the piece character that "to" position gets set to
		if (((currentPiece->isWhite && toY == 0) || (!(currentPiece->isWhite) && toY == 7)) && (tolower(piece) == 'p')) {
			while (true) {
				std::cout << "Enter piece to be promoted to (Q, R, B, N): ";
				std::getline(std::cin, input);
				char selection = input[0];
				if (selection == 'Q' || selection == 'R' || selection == 'B' || selection == 'N') {
					(currentPiece->isWhite) ? (piece = selection) : (piece = tolower(selection));
					break;
				}
				std::cout << "Invalid selection. Please try again!\n";
			}
		}

		// Move piece, then alternate turn
		board[toY][toX] = piece;
		board[fromY][fromX] = ' ';
		delete currentPiece;

		whiteTurn = !whiteTurn;
		std::cout << "\n";
	}

	return 0
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