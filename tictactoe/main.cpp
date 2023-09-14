#include <iostream>
#include <conio.h>

class Board {
/*
 *  Main Board Class
 */
public:
	void run() {
		while (running) {
			if (!this->winCase()) {
				this->buildBoard();
				this->input();
			} else {
				if (this->turn == 2) std::cout << "X";
				if (this->turn == 1) std::cout << "O";
				std::cout << " has won!\nWould you like to play again? (y/n)" << std::endl;
				int input = getch();
				if (input == 121) {
					for (auto & row : this->board) {
						for (auto & col : row) {
							col = 0;
						}
					}
					this->cursor[0] = 0;
					this->cursor[1] = 0;
					continue;
				} else this->running = false;
			}
		}
	}

private:
	int xWins = 0;
	int oWins = 0;
	int cursor[2] = {0, 0};
	bool running = true;
	int prevInput = 0;
	int board[3][3] {
			{0, 0, 0},
			{0, 0, 0},
			{0, 0, 0}
	};
	int turn = 1;

	void input() {
		int input = getch();
		if (input == 3) { // Checks for ctrl-c
			exit(0);
		} else if (this->prevInput == 224) { // Checks for arrow presses
			if (input == 72) { // Up arrow
				if (this->cursor[1] > 0) this->cursor[1]--;
			} else if (input == 80) { // Down arrow
				if (this->cursor[1] < 2) this->cursor[1]++;
			} else if (input == 75) { // Left arrow
				if (this->cursor[0] > 0) this->cursor[0]--;
			} else if (input == 77) { // Right arrow
				if (this->cursor[0] < 2) this->cursor[0]++;
			}
		} else if (input == 113) { // q
			this->running = false;
		} else if (input == 13) { // Enter
			// If the space is empty put the turn in the board
			// at the cursor then flip the turn
			if (this->board[this->cursor[1]][this->cursor[0]] == 0) {
				this->board[this->cursor[1]][this->cursor[0]] = this->turn;
				if (this->turn == 1) { this->turn = 2; }
				else { this->turn = 1; }
			}
		}
		this->prevInput = input;
	}

	void buildBoard() {
		system("cls");
		for (int x = 0; x < 3; x++) {
			for (int y = 0; y < 3; y++) {
				// Put a white background for cursor
				if (this->cursor[1] == x && this->cursor[0] == y) std::cout  << "\e[0;30m" << "\e[47m";
				// Puts the spaces x's and o's
				if (this->board[x][y] == 0) std::cout << ' ';
				else if (this->board[x][y] == 1) std::cout << 'X';
				else if (this->board[x][y] == 2) std::cout << 'O';
				// Reset background from cursor
				std::cout << "\e[40m\e[0;37m";
				// Puts the lines and creates new lines
				if (y < 2) std::cout << " | ";
				else std::cout << std::endl;
			}
		}
		std::cout << "X has " << this->xWins << " wins" << std::endl;
		std::cout << "O has " << this->oWins << " wins" << std::endl;
		if (this->turn == 1) std::cout << "X";
		else std::cout << "O";
		std::cout << "'s turn" << std::endl;
	}

	bool winCase() {
		// Horizontals
		for (int row = 0; row < 3; row++) {
			if (this->board[row][0] == 1 && this->board[row][1] == 1 && this->board[row][2] == 1) {
				this->xWins++;
				return true;
			}
			if (this->board[row][0] == 2 && this->board[row][1] == 2 && this->board[row][2] == 2) {
				this->oWins++;
				return true;
			}
		}
		// Verticals
		for (int col = 0; col < 3; col++) {
			if (this->board[0][col] == 1 && this->board[1][col] == 1 && this->board[2][col] == 1) {
				this->xWins++;
				return true;
			}
			if (this->board[0][col] == 2 && this->board[1][col] == 2 && this->board[2][col] == 2) {
				this->oWins++;
				return true;
			}
		}
		// Diagonals
		if (this->board[0][0] == 1 && this->board[1][1] == 1 && this->board[2][2] == 1) {
			this->xWins++;
			return true;
		}
		if (this->board[2][0] == 1 && this->board[1][1] == 1 && this->board[0][2] == 1) {
			this->xWins++;
			return true;
		}
		if (this->board[0][0] == 2 && this->board[1][1] == 2 && this->board[2][2] == 2) {
			this->oWins++;
			return true;
		}
		if (this->board[2][0] == 2 && this->board[1][1] == 2 && this->board[0][2] == 2) {
			this->oWins++;
			return true;
		}

		return false;
	}
};

int main() {
	Board board;

	board.run();

	return 0;
}
