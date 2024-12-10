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

// Unicode chess pieces
#define WHITE_PAWN   L'\u2659'
#define WHITE_ROOK   L'\u2656'
#define WHITE_KNIGHT L'\u2658'
#define WHITE_BISHOP L'\u2657'
#define WHITE_QUEEN  L'\u2655'
#define WHITE_KING   L'\u2654'
#define BLACK_PAWN   L'\u265F'
#define BLACK_ROOK   L'\u265C'
#define BLACK_KNIGHT L'\u265E'
#define BLACK_BISHOP L'\u265D'
#define BLACK_QUEEN  L'\u265B'
#define BLACK_KING   L'\u265A'

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
	virtual std::vector<std::vector<int>> getMoves(char board[][8]) {
		std::vector<std::vector<int>> possibleMoves;
		return possibleMoves;
	}

};

class Pawn : public Piece {
public:

	Pawn(int x, int y, bool white) : Piece(x, y, white) {}

	std::vector<std::vector<int>> getMoves(char board[][8]) override {

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

	std::vector<std::vector<int>> getMoves(char board[][8]) override {


		std::vector<std::vector<int>> possibleMoves;

		//This code checks the rook's x (horizontal) path
		//The for loop runs twice, once with dir = 1 and once with dir = -1
		for (int dir = 1; dir >= -1; dir -= 2) {
			//This for loop checks every space in front of the rook when dir = 1
			//and every space behind the rook when dir = -1
			for (int i = xPos + dir; i < 8 && i >= 0; i += dir) {

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
			for (int i = yPos + dir; i < 8 - yPos && i >= 0; i += dir) {
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

	std::vector<std::vector<int>> getMoves(char board[][8]) override {
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


	std::vector<std::vector<int>> getMoves(char board[][8]) override {

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
					if (board[yPos + i * ydir][xPos + i * xdir] == ' ') {

						//The space is empty, therefore add it to possible moves
						possibleMoves.push_back(convertToVector(xPos + i * xdir, yPos + i * ydir));
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


	std::vector<std::vector<int>> getMoves(char board[][8]) override {
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


	std::vector<std::vector<int>> getMoves(char board[][8]) override {
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

	bool inCheckAt(char board[][8], int x, int y) {

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				//Check every space that is not empty, the opposite color, and not the king
				//If the space is where the king is, continue
				if (i == y && j == x) {
					continue;
				}
				if (board[i][j] != ' ' && isupper(board[i][j]) != isWhite) {
					Piece* currentPiece = nullptr;
					//Assign the piece to the correct type
					switch (toupper(board[i][j])) {
					case 'P':
						currentPiece = new Pawn(j, i, !isWhite);
						break;
					case 'Q':
						currentPiece = new Queen(j, i, !isWhite);
						break;
					case 'K':
						currentPiece = new King(j, i, !isWhite);
						break;
					case 'B':
						currentPiece = new Bishop(j, i, !isWhite);
						break;
					case 'R':
						currentPiece = new Rook(j, i, !isWhite);
						break;
					case 'N':
						currentPiece = new Knight(j, i, !isWhite);
						break;
					}

					//Check the possible moves of the piece
					std::vector<std::vector<int>> possibleMoves = currentPiece->getMoves(board);
					for (auto move : possibleMoves) {
						//If the piece can move to the king's space, the king is in check
						if (move[0] == x && move[1] == y) {
							return true;
						}
					}
				}

			}
		}

		//No pieces can capture the king, so it is not in check
		return false;
	}

	bool inCheckmate(char board[][8]) {
		//If the king is not in check, it cannot be checkmate
		if (!inCheckAt(board, xPos, yPos)) {
			return false;
		}

		//make a 'test board' that can be modified
		char testBoard[8][8];
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				testBoard[i][j] = board[i][j];
			}
		}

		//If the king can move somewhere where its not in check, it cannot be checkmate
		std::vector<std::vector<int>> myPossibleMoves = getMoves(board);
		for (auto move : myPossibleMoves) {
			//Make the move on the testboard and test if the king is still in check
			//Make a temp character storing the empty char or the piece on the space the king moves to
			char temp = testBoard[move[1]][move[0]];
			testBoard[move[1]][move[0]] = testBoard[yPos][xPos];
			testBoard[yPos][xPos] = ' ';
			//If the king is not in check it cannot be checkmate
			if (!inCheckAt(testBoard, move[0], move[1])) {
				return false;
			}
			//Reset the testboard using the temp char
			testBoard[yPos][xPos] = testBoard[move[1]][move[0]];
			testBoard[move[1]][move[0]] = temp;
		}

		//If any piece can move somewhere where the king stops being in check, it cannot be checkmate

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] != ' ' && isupper(board[i][j]) == isWhite && !(i == yPos && j == xPos)) {
					Piece* currentPiece = nullptr;
					switch (toupper(board[i][j])) {
					case 'P':
						currentPiece = new Pawn(j, i, isWhite);
						break;
					case 'Q':
						currentPiece = new Queen(j, i, isWhite);
						break;
					case 'K':
						currentPiece = new King(j, i, isWhite);
						break;
					case 'B':
						currentPiece = new Bishop(j, i, isWhite);
						break;
					case 'R':
						currentPiece = new Rook(j, i, isWhite);
						break;
					case 'N':
						currentPiece = new Knight(j, i, isWhite);
						break;
					}


					//Check the possible moves of the piece
					std::vector<std::vector<int>> possibleMoves = currentPiece->getMoves(board);
					for (auto move : possibleMoves) {
						//Make the move on the test board and check if the king is still in check
						//Make a temp character storing the empty char or the piece on the space the king moves to
						char temp = testBoard[move[1]][move[0]];
						testBoard[move[1]][move[0]] = testBoard[i][j];
						testBoard[i][j] = ' ';
						if (!inCheckAt(testBoard, xPos, yPos)) {
							//If a move results in the king no longer being in check, it cannot be checkmate
							return false;
						}
						//Reset the testboard using the temp char
						testBoard[i][j] = testBoard[move[1]][move[0]];
						testBoard[move[1]][move[0]] = temp;
					}
				}

			}
		}

		//If none of the above tests pass, it is checkmate
		return true;
	}
};
void showBoard(wchar_t boardDisplay[8][8]) {
	//std::wcout.imbue(std::locale("en_US.UTF-8"));

	const int squareWidth = 3;
	for (int i = 0; i < 8; i++) {
		std::wcout << 8 - i << L" "; // Row label
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0) {
				std::wcout << L"\033[40m "; // White square, black text
				std::wcout << std::setw(squareWidth - 2) << boardDisplay[i][j] << L" ";
				std::wcout << L"\033[0m";
			}
			else {
				std::wcout << L"\033[46m "; // Black square, white text
				std::wcout << std::setw(squareWidth - 2) << boardDisplay[i][j] << L" ";
				std::wcout << L"\033[0m";
			}
		}
		std::wcout << L" " << 8 - i << L"\n"; // Row label on the right
	}

	// Column labels
	std::wcout << L"  ";
	for (int j = 0; j < 8; j++) {
		std::wcout << L" " << wchar_t('A' + j) << L" ";
	}
	std::wcout << std::endl;
}
/*
//This function prints the board to the output window in proper format
// Function to display the chessboard with alternating black and white squares
void showBoard(char board[][8]) {
	const int square_width = 4; // Fixed width for each square (including borders and padding)

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
		std::cout << " " << c << "  ";
	}
	std::cout << std::endl;
}
*/
void convertBoard(char board[8][8], wchar_t boardDisplay[8][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (board[i][j]) {
			case 'p': boardDisplay[i][j] = BLACK_PAWN; break;
			case 'r': boardDisplay[i][j] = BLACK_ROOK; break;
			case 'n': boardDisplay[i][j] = BLACK_KNIGHT; break;
			case 'b': boardDisplay[i][j] = BLACK_BISHOP; break;
			case 'q': boardDisplay[i][j] = BLACK_QUEEN; break;
			case 'k': boardDisplay[i][j] = BLACK_KING; break;
			case 'P': boardDisplay[i][j] = WHITE_PAWN; break;
			case 'R': boardDisplay[i][j] = WHITE_ROOK; break;
			case 'N': boardDisplay[i][j] = WHITE_KNIGHT; break;
			case 'B': boardDisplay[i][j] = WHITE_BISHOP; break;
			case 'Q': boardDisplay[i][j] = WHITE_QUEEN; break;
			case 'K': boardDisplay[i][j] = WHITE_KING; break;
			default:  boardDisplay[i][j] = L' '; break;
			}
		}
	}
	/*for (int i = 0; i < 8; i++) {
		if (i % 2 == 0) {
			boardDisplay[0][i] = BLACK_KNIGHT;
		}
		else {
			boardDisplay[0][i] = WHITE_KNIGHT;
		}
	}*/
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
	// Enable Unicode output for Windows (required for wide characters)
	_setmode(_fileno(stdout), _O_U16TEXT);

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

	wchar_t boardDisplay[8][8];

	// Explain notation for users
	std::wcout << L"Welcome to Chess! Please note the following information:\nUppercase pieces are white, and lowercase pieces are black.\nThe pieces are as follows:\nP = pawn, R = rook, N = knight, B = bishop, Q = queen, K = king\n\n";

	bool whiteTurn = true;
	while (true) {
		convertBoard(board, boardDisplay);
		showBoard(boardDisplay);
		// Get user input for move and validate input string format
		std::wcout << (whiteTurn ? L"White's turn." : L"Black's turn.") << L" Enter move (ex. E2 E4), I for info, or D to offer draw: ";
		std::string input;
		std::getline(std::cin, input);

		if (input.size() == 1) {
			if (input[0] == 'I') { // Display information if user wants to see it again
				std::wcout << L"Uppercase pieces are white, and lowercase pieces are black.\nThe pieces are as follows:\nP = pawn, R = rook, N = knight, B = bishop, Q = queen, K = king\n\n";
				continue;
			}
			else if (input[0] == 'D') { // Handle logic for offering a draw
				while (true) {
					std::wcout << (whiteTurn ? L"White" : L"Black") << L" is offering a draw. Do you accept (Y/N): ";
					std::getline(std::cin, input);
					if (input[0] == 'Y') {
						std::wcout << L"The game has ended in a tie!";
						return 0;
					}
					else if (input[0] == 'N') {
						std::wcout << (whiteTurn ? L"Black" : L"White") << L" has rejected the draw offer!\n\n";
						break;
					}
					else {
						std::wcout << L"Invalid input. Please try again!\n\n";
						continue;
					}
				}
				continue;
			}
		}
		else if (input.size() != 5 || input[2] != ' ') { // Validate user move input
			std::wcout << L"Invalid move input. Please try again!\n";
			continue;
		}

		// Parse user input into variables
		int fromX = input[0] - L'A';			// Column ('E' -> 4)
		int fromY = 8 - (input[1] - L'0');	// Row (2 -> 6)
		int toX = input[3] - L'A';
		int toY = 8 - (input[4] - L'0');

		std::vector<int> from = convertToVector(fromX, fromY);
		std::vector<int> to = convertToVector(toX, toY);

		// Make sure user's move is within chess board, otherwise prompt to try again
		if (!coordInBounds(fromX, fromY) || !coordInBounds(toX, toY)) {
			std::wcout << L"Coordinates out of bounds. Please try again!\n";
			continue;
		}

		char piece = board[fromY][fromX];
		// Make sure piece is valid, and white/black depending on turn
		if (piece == ' ' || (whiteTurn && islower(piece)) || (!whiteTurn && isupper(piece))) {
			std::wcout << L"Invalid piece selection. Please try again!\n";
			continue;
		}

		// Get piece from coordinates
		Piece* currentPiece = nullptr;

		switch (toupper(piece)) {
		case 'P':
			currentPiece = new Pawn(fromX, fromY, isupper(piece));
			break;
		case 'R':
			currentPiece = new Rook(fromX, fromY, isupper(piece));
			break;
		case 'B':
			currentPiece = new Bishop(fromX, fromY, isupper(piece));
			break;
		case 'Q':
			currentPiece = new Queen(fromX, fromY, isupper(piece));
			break;
		case 'K':
			currentPiece = new King(fromX, fromY, isupper(piece));
			break;
		case 'N':
			currentPiece = new Knight(fromX, fromY, isupper(piece));
			break;
		}

		if (!currentPiece) {
			std::wcout << L"Piece type invalid. Please try again!\n";
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
			std::wcout << L"Invalid move. Please try again!\n";
			delete currentPiece;
			continue;
		}

		// Promotion criteria, change the piece character that "to" position gets set to
		if (((currentPiece->isWhite && toY == 0) || (!(currentPiece->isWhite) && toY == 7)) && (tolower(piece) == 'p')) {
			while (true) {
				std::wcout << L"Enter piece to be promoted to (Q, R, B, N): ";
				std::getline(std::cin, input);
				char selection = input[0];
				if (selection == 'Q' || selection == 'R' || selection == 'B' || selection == 'N') {
					(currentPiece->isWhite) ? (piece = selection) : (piece = tolower(selection));
					break;
				}
				std::wcout << L"Invalid selection. Please try again!\n";
			}
		}

		// Move piece, then alternate turn
		board[toY][toX] = piece;
		board[fromY][fromX] = ' ';
		delete currentPiece;

		whiteTurn = !whiteTurn;
		std::wcout << L"\n";

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

