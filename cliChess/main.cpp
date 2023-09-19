#include <iostream>
#include <conio.h>

enum COLOR {
	WHITE, BLACK
};

enum DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum PIECE {
	PAWN = 'P',
	KNIGHT = 'N',
	BISHOP = 'B',
	ROOK = 'R',
	QUEEN = 'Q',
	KING = 'K'
};

class GamePiece {
public:
	explicit GamePiece(COLOR PieceColor) : pieceColor(PieceColor) {}

	virtual PIECE getPiece() = 0;

	COLOR getColor() {
		return pieceColor;
	}

	bool isLegalMove(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8]) {
		GamePiece *destPiece = gameBoard[destRow][destCol];
		if ((destPiece == nullptr) || (pieceColor != destPiece->getColor())) {
			return areSquaresLegal(srcRow, srcCol, destRow, destCol, gameBoard);
		}
		return false;
	}


private:
	virtual bool areSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, GamePiece *GameBoard[8][8]) = 0;

	COLOR pieceColor;
};

class PawnPiece : public GamePiece {
public:
	explicit PawnPiece(COLOR pieceColor) : GamePiece(pieceColor) {}

private:
	PIECE getPiece() override {
		return PAWN;
	}

	bool areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8]) override {
		GamePiece *qpDest = gameBoard[destRow][destCol];
		if (qpDest == nullptr) { // Destination square is unoccupied
			if (srcCol == destCol) {
				if (getColor() == WHITE && ((destRow == srcRow + 1) || (srcRow == 1 && destRow == srcRow + 2)))
					return true;
				if (getColor() == BLACK && ((destRow == srcRow - 1) || (srcRow == 6 && destRow == srcRow - 2)))
					return true;
			}
		} else { // Dest holds piece of opposite color
			if ((srcCol == destCol + 1) || (srcCol == destCol - 1)) {
				if (getColor() == WHITE && destRow == srcRow + 1) return true;
				if (getColor() == BLACK && destRow == srcRow - 1) return true;
			}
		}
		return false;
	}
};

class KnightPiece : public GamePiece {
public:
	explicit KnightPiece(COLOR pieceColor) : GamePiece(pieceColor) {}

private:
	PIECE getPiece() override {
		return KNIGHT;
	}

	bool areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8]) override {
		// Destination square is unoccupied or occupied by opposite color
		if (((srcCol == destCol + 1) || (srcCol == destCol - 1)) && (srcRow == destRow + 2) || (srcRow == destRow - 2))
			return true;

		if (((srcCol == destCol + 2) || (srcCol == destCol - 2)) &&
		    ((srcRow == destRow + 1) || (srcRow == destRow - 1)))
			return true;
		return false;
	}
};

class BishopPiece : public GamePiece {
public:
	explicit BishopPiece(COLOR pieceColor) : GamePiece(pieceColor) {}

private:
	PIECE getPiece() override {
		return BISHOP;
	}

	bool areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8]) override {
		if ((destCol - srcCol == destRow - srcRow) || (destCol - srcCol == srcRow - destRow)) {
			// Checks diagonals
			int iRowOffset = (destRow - srcRow > 0) ? 1 : -1;
			int iColOffset = (destCol - srcCol > 0) ? 1 : -1;
			int iCheckRow;
			int iCheckCol;
			for (iCheckRow = srcRow + iRowOffset, iCheckCol = srcCol + iColOffset;
			     iCheckRow != destRow;
			     iCheckRow = iCheckRow + iRowOffset, iCheckCol = iCheckCol + iColOffset) {
				if (gameBoard[iCheckRow][iCheckCol] != nullptr) return false;
			}
			return true;
		}
		return false;
	}
};

class RookPiece : public GamePiece {
public:
	explicit RookPiece(COLOR pieceColor) : GamePiece(pieceColor) {}

private:
	PIECE getPiece() override {
		return ROOK;
	}

	bool areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8]) override {
		if (srcRow == destRow) {
			// Check vertical
			int colOffset = (destCol - srcCol > 0) ? 1 : -1;
			for (int checkCol = srcCol + colOffset; checkCol != destCol; checkCol = checkCol + colOffset) {
				if (gameBoard[srcRow][checkCol] != nullptr) return false;
			}
			return true;
		} else if (destCol == srcCol) {
			// Check horizontal
			int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
			for (int checkRow = srcRow + rowOffset; checkRow != destRow; checkRow = checkRow + rowOffset) {
				if (gameBoard[checkRow][srcCol] != nullptr) return false;
			}
			return true;
		}
		return false;
	}
};

class QueenPiece : public GamePiece {
public:
	explicit QueenPiece(COLOR pieceColor) : GamePiece(pieceColor) {}

private:
	PIECE getPiece() override {
		return QUEEN;
	}

	bool areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8]) override {
		if (srcRow == destRow) {
			// Check vertical
			int colOffset = (destCol - srcCol > 0) ? 1 : -1;
			for (int checkCol = srcCol + colOffset; checkCol != destCol; checkCol = checkCol + colOffset) {
				if (gameBoard[srcRow][checkCol] != nullptr) return false;
			}
			return true;
		} else if (destCol == srcCol) {
			// Check horizontal
			int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
			for (int checkRow = srcRow + rowOffset; checkRow != destRow; checkRow = checkRow + rowOffset) {
				if (gameBoard[checkRow][srcCol] != nullptr) return false;
			}
			return true;
		} else if ((destCol - srcCol == destRow - srcRow) || (destCol - srcCol == srcRow - destRow)) {
			// Check diagonal
			int rowOffset = (destRow - srcRow > 0) ? 1 : -1;
			int colOffset = (destCol - srcCol > 0) ? 1 : -1;
			int checkRow;
			int checkCol;
			for (checkRow = srcRow + rowOffset, checkCol = srcCol + colOffset;
			     checkRow != destRow;
			     checkRow = checkRow + rowOffset, checkCol = checkCol + colOffset) {
				if (gameBoard[checkRow][checkCol] != nullptr) return false;
			}
			return true;
		}
		return false;
	}
};

class KingPiece : public GamePiece {
public:
	explicit KingPiece(COLOR pieceColor) : GamePiece(pieceColor) {}


private:
	bool hasCastled = false;

	PIECE getPiece() override {
		return KING;
	}

	bool areSquaresLegal(int srcRow, int srcCol, int destRow, int destCol, GamePiece *gameBoard[8][8]) override {
		int rowDelta = destRow - srcRow;
		int colDelta = destCol - srcCol;
		if ((rowDelta >= -1 && rowDelta <= 1) && (colDelta >= -1 && colDelta <= 1)) return true;
		if (!hasCastled && ((colDelta == 2 && rowDelta == 0) &&
		                    (gameBoard[0][7]->getPiece() == ROOK || gameBoard[7][7]->getPiece() == ROOK)) ||
		    (colDelta == -2 && rowDelta == 0) &&
		    (gameBoard[0][0]->getPiece() == ROOK || gameBoard[7][0]->getPiece() == ROOK)) {
			hasCastled = true;
			return true;
		}
		return false;
	}

};

class Game {
public:
	Game() {
		// Fill the board with nullptr
		for (auto &row: board) {
			for (auto &col: row) {
				col = nullptr;
			}
		}

		// Place the black pieces
		for (int col = 0; col < 8; col++) {
			board[6][col] = new PawnPiece(BLACK);
		}
		board[7][0] = new RookPiece(BLACK);
		board[7][1] = new KnightPiece(BLACK);
		board[7][2] = new BishopPiece(BLACK);
		board[7][3] = new QueenPiece(BLACK);
		board[7][4] = new KingPiece(BLACK);
		board[7][5] = new BishopPiece(BLACK);
		board[7][6] = new KnightPiece(BLACK);
		board[7][7] = new RookPiece(BLACK);

		// Place the white pieces
		for (int col = 0; col < 8; col++) {
			board[1][col] = new PawnPiece(WHITE);
		}
		board[0][0] = new RookPiece(WHITE);
		board[0][1] = new KnightPiece(WHITE);
		board[0][2] = new BishopPiece(WHITE);
		board[0][3] = new QueenPiece(WHITE);
		board[0][4] = new KingPiece(WHITE);
		board[0][5] = new BishopPiece(WHITE);
		board[0][6] = new KnightPiece(WHITE);
		board[0][7] = new RookPiece(WHITE);
	}

	void run() {
		while (running) {
			printBoard();
			input();
		}
	}

private:
	bool running = true;
	GamePiece *board[8][8]{};
	int cursor[2] = {0, 0}; // Col, Row
	int prevInput = 0;
	int startRow = 20;
	int startCol = 0;
	COLOR turn = WHITE;

	void printBoard() {
		// constants
		const std::string white = "\033[48;5;15m";
		const std::string black = "\033[48;5;0m";
		const std::string whiteText = "\033[38;5;15m";
		const std::string blackText = "\033[38;5;0m";
		const std::string redText = "\033[38;5;3m";
		const std::string bold = "\033[1m";
		const std::string reset = "\033[0m";

		// Clear then draw the board
		system("cls");

		for (int row = 7; row >= 0; row--) {
			std::cout << row + 1 << " ";

			for (int col = 0; col < 8; col++) {
				if ((row % 2 != 0 && col % 2 != 0) ||
				    (row % 2 == 0 && col % 2 == 0)) {
					std::cout << black << whiteText;
				} else if ((row % 2 == 0 && col % 2 != 0) ||
				           (row % 2 != 0 && col % 2 == 0)) {
					std::cout << white << blackText;
				}
				if (board[row][col] != nullptr) {
					if (board[row][col]->getColor() == BLACK) std::cout << redText;
					if (cursor[1] == row && cursor[0] == col)
						std::cout << "[" << (char) board[row][col]->getPiece() << "]";
					else std::cout << " " << (char) board[row][col]->getPiece() << " ";
				} else {
					if (cursor[1] == row && cursor[0] == col) std::cout << "[ ]";
					else std::cout << "   ";
				}
				std::cout << reset;
			}
			std::cout << std::endl;
		}
		std::cout << "   A  B  C  D  E  F  G  H" << std::endl;
		if (turn == WHITE) std::cout << "It is whites turn" << std::endl;
		if (turn == BLACK) std::cout << "It is blacks turn" << std::endl;
		std::cout << "Keybinds: q=quit, c=clear selected piece, enter=select space, arrow keys=move cursor" << std::endl;
	}

	bool isInCheck(COLOR pieceColor) {
		// Find the king
		int kingRow;
		int kindCol;
		for (int row = 0; row < 8; ++row) {
			for (int col = 0; col < 8; ++col) {
				if ((board[row][col] != nullptr) &&
				    (board[row][col]->getColor() == pieceColor) &&
				    (board[row][col]->getPiece() == KING)) {
					kingRow = row;
					kindCol = col;
				}
			}
		}
		// Run through the opponent's pieces and see if any can take the king
		for (int row = 0; row < 8; ++row) {
			for (int col = 0; col < 8; ++col) {
				if ((board[row][col] != nullptr) &&
				    (board[row][col]->getColor() != pieceColor) &&
				    (board[row][col]->isLegalMove(row, col, kingRow, kindCol, board)))
					return false;

			}
		}
		return true;
	}

	void move() {
		if (startRow == 20) {
			if (board[cursor[1]][cursor[0]] == nullptr) return;
			startRow = cursor[1];
			startCol = cursor[0];
			return;
		}

		int endRow = cursor[1];
		int endCol = cursor[0];

		if (startRow == endRow && startCol == endCol) {
			startRow = 20;
			return;
		}

		GamePiece *currentPiece = board[startRow][startCol];

		if (currentPiece->getPiece() != 0 && currentPiece->getColor() != turn) {
			startRow = 20;
			return;
		}
		if (!currentPiece->isLegalMove(startRow, startCol, endRow, endCol, board)) {
			startRow = 20;
			return;
		}
		if (!isInCheck(turn)) {
			startRow = 20;
			return;
		}

		if (board[startRow][startCol]->getPiece() == KING) {
			if (endCol == 6 && board[startRow][startCol]->getPiece() == KING &&
			    (board[endRow][7]->getPiece() == ROOK || board[endRow][7]->getPiece() == ROOK)) {
				board[endRow][endCol] = board[startRow][startCol];
				board[startRow][startCol] = nullptr;
				board[endRow][endCol - 1] = board[endRow][7];
				board[endRow][7] = nullptr;

				startRow = 20;
				if (turn == WHITE) turn = BLACK;
				else turn = WHITE;
				return;
			}
			if (endCol == 2 && board[startRow][startCol]->getPiece() == KING &&
			    (board[endRow][0]->getPiece() == ROOK || board[endRow][0]->getPiece() == ROOK)) {
				board[endRow][endCol] = board[startRow][startCol];
				board[startRow][startCol] = nullptr;
				board[endRow][endCol + 1] = board[endRow][0];
				board[endRow][0] = nullptr;

				startRow = 20;
				if (turn == WHITE) turn = BLACK;
				else turn = WHITE;
				return;
			}
		}

		board[endRow][endCol] = board[startRow][startCol];
		board[startRow][startCol] = nullptr;

		startRow = 20;
		if (turn == WHITE) turn = BLACK;
		else turn = WHITE;
	}

	void input() {
		int input = getch();

		if (this->prevInput == 224) { // Arrows
			if (input == 72 && cursor[1] < 7) cursor[1]++; // Up Arrow
			if (input == 80 && cursor[1] > 0) cursor[1]--; // Down Arrow
			if (input == 75 && cursor[0] > 0) cursor[0]--; // Left Arrow
			if (input == 77 && cursor[0] < 7) cursor[0]++; // Right Arrow
		}
		if (input == 113) running = false; // q
		if (input == 13) move(); // Enter
		if (input == 99) startRow = 20;
		if (input == 3) exit(0); // ctrl-c

		prevInput = input;
	}
};

int main() {
	Game game;
	game.run();

	return 0;
}