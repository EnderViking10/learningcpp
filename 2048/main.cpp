#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>

enum direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Game {
public:
	Game() {
		this->newNum();
	}

	void run() {
		while (running) {
			this->drawMap();
			this->input();
		}
	}
	void debug(int direction, int debugMap[4][4]) {
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				this->map[row][col] = debugMap[row][col];
			}
		}
		this->move(direction);
		this->drawMap();
	}

private:
	bool running = true;
	int prevInput = 0;

	int map[4][4] = {
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
	};
	int oldMap[4][4];

	void drawMap() {
		system("cls");
		for (auto &row: map) {
			for (auto &col: row) {
				if (col != 0) std::cout << "\033[38;5;16m";
				if (col == 2) std::cout << "\033[48;5;213m";
				if (col == 4) std::cout << "\033[48;5;176m";
				if (col == 8) std::cout << "\033[48;5;139m";
				if (col == 16) std::cout << "\033[48;5;102m";
				if (col == 32) std::cout << "\033[48;5;65m";
				if (col == 64) std::cout << "\033[48;5;28m";
				if (col == 128) std::cout << "\033[48;5;22m";
				if (col == 256) std::cout << "\033[48;5;196m";
				if (col == 512) std::cout << "\033[48;5;160m";
				if (col == 1024) std::cout << "\033[48;5;124m";
				if (col == 2048) std::cout << "\033[48;5;88m";
				if (col >= 4096) std::cout << "\033[48;5;52m";
				std::cout << col << "\t";
				std::cout << "\033[0m";
			}
			std::cout << std::endl << std::endl;
		}
	}

	void newNum() {
		unsigned seed = time(nullptr);
		srand(seed);

		int location[2];

		do {
			location[0] = rand() % 4;
			location[1] = rand() % 4;
		} while (this->map[location[0]][location[1]] != 0);

		this->map[location[0]][location[1]] = (rand() % 2 + 1) * 2;
	}

	void move(int direction) {
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col ++) {
				this->oldMap[row][col] = this->map[row][col];
			}
		}

		bool combined[4] = {false, false, false, false};
		bool moved = false;

		if (direction == LEFT) {
			// Move
			/*
			 * There are 2 sections of move for cases like 2 2 0 4 turning into 4 0 4 0
			 * This happens because 2 2 0 4 -> 2 2 4 0 -> 4 0 4 0 and the function is done
			 * With the second move it checks again for any possible moves like 4 0 4 0 -> 4 4 0 0
			 */
			for (int i = 0; i < 2; i++) {
				for (int row = 0; row < 4; row++) {
					for (int col = 3; col > 0; col--) {
						if (this->map[row][col] == 0) continue;
						if (this->map[row][col - 1] != 0) continue;

						this->map[row][col - 1] = this->map[row][col];
						this->map[row][col] = 0;
						moved = true;
					}
				}
			}
			// Combine
			for (int row = 0; row < 4; row++) {
				for (int col = 3; col > 0; col--) {
					if (combined[row]) {
						/*
						 * This is to solve cases like 8 4 4 0 turning into 16 0 0 0
						 * This case goes like 8 4 4 0 -> 8 8 0 0, without with it'd be 8 4 4 0 -> 8 8 0 0 -> 16 0 0 0
						 * The array of bool's is because the UP and DOWN it would basically ignore just a single bool
						 * If the case were 8 2 -> 8 2 (rest combine) -> 16 2
						 * it'd go          4 0    8 0                   0  0
						 *                  4 0    0 0                   0  0
						 *                  0 0    0 0                   0  0
						 */
						combined[row] = false;
						continue;
					}
					if (this->map[row][col] == 0) continue;
					if (this->map[row][col] != this->map[row][col - 1]) continue;

					this->map[row][col - 1] *= 2;
					this->map[row][col] = 0;
					combined[row] = true;
					moved = true;
				}
			}
			// Move
			for (int i = 0; i < 2; i++) {
				for (int row = 0; row < 4; row++) {
					for (int col = 3; col > 0; col--) {
						if (this->map[row][col] == 0) continue;
						if (this->map[row][col - 1] != 0) continue;

						this->map[row][col - 1] = this->map[row][col];
						this->map[row][col] = 0;
						moved = true;
					}
				}
			}
		} else if (direction == RIGHT) {
			// Move
			for (int i = 0; i < 2; i++) { // This runs the check twice to grab edge cases such as 0 2 8 0
				for (int row = 0; row < 4; row++) {
					for (int col = 0; col < 3; col++) {
						if (this->map[row][col] == 0) continue;
						if (this->map[row][col + 1] != 0) continue;

						this->map[row][col + 1] = this->map[row][col];
						this->map[row][col] = 0;
						moved = true;
					}
				}
			}
			// Combine
			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 3; col++) {
					if (combined[row]) {
						combined[row] = false;
						continue;
					}
					if (this->map[row][col] == 0) continue;
					if (this->map[row][col] != this->map[row][col + 1]) continue;

					this->map[row][col + 1] *= 2;
					this->map[row][col] = 0;
					combined[row] = true;
					moved = true;
				}
			}
			// Move
			for (int i = 0; i < 2; i++) { // This runs the check twice to grab edge cases such as 0 2 8 0
				for (int row = 0; row < 4; row++) {
					for (int col = 0; col < 3; col++) {
						if (this->map[row][col] == 0) continue;
						if (this->map[row][col + 1] != 0) continue;

						this->map[row][col + 1] = this->map[row][col];
						this->map[row][col] = 0;
						moved = true;
					}
				}
			}
		} else if (direction == UP) {
			// Move
			for (int i = 0; i < 2; i++) {
				for (int row = 3; row > 0; row--) {
					for (int col = 0; col < 4; col++) {
						if (this->map[row][col] == 0) continue;
						if (this->map[row - 1][col] != 0) continue;

						this->map[row - 1][col] = this->map[row][col];
						this->map[row][col] = 0;
						moved = true;
					}
				}
			}
			// Combine
			for (int row = 3; row > 0; row--) {
				for (int col = 0; col < 4; col++) {
					// Combine
					if (combined[col]) {
						combined[col] = false;
						continue;
					}
					if (this->map[row][col] == 0) continue;
					if (this->map[row][col] != this->map[row - 1][col]) continue;

					this->map[row - 1][col] *= 2;
					this->map[row][col] = 0;
					combined[col] = true;
					moved = true;
				}
			}
			// Move
			for (int i = 0; i < 2; i++) {
				for (int row = 3; row > 0; row--) {
					for (int col = 0; col < 4; col++) {
						if (this->map[row][col] == 0) continue;
						if (this->map[row - 1][col] != 0) continue;

						this->map[row - 1][col] = this->map[row][col];
						this->map[row][col] = 0;
						moved = true;
					}
				}
			}
		} else if (direction == DOWN) {
			// Move
			for (int i = 0; i < 2; i++) {
				for (int row = 0; row < 3; row++) {
					for (int col = 0; col < 4; col++) {
						if (this->map[row][col] == 0) continue;
						if (this->map[row + 1][col] != 0) continue;

						this->map[row + 1][col] = this->map[row][col];
						this->map[row][col] = 0;
						moved = true;
					}
				}
			}
			// Combine
			for (int row = 0; row < 3; row++) {
				for (int col = 0; col < 4; col++) {
					// Combine
					if (combined[col]) {
						combined[col] = false;
						continue;
					}
					if (this->map[row][col] == 0) continue;
					if (this->map[row][col] != this->map[row + 1][col]) continue;

					this->map[row + 1][col] *= 2;
					this->map[row][col] = 0;
					combined[col] = true;
					moved = true;
				}
			}
			// Move
			for (int i = 0; i < 2; i++) {
				for (int row = 0; row < 3; row++) {
					for (int col = 0; col < 4; col++) {
						if (this->map[row][col] == 0) continue;
						if (this->map[row + 1][col] != 0) continue;

						this->map[row + 1][col] = this->map[row][col];
						this->map[row][col] = 0;
						moved = true;
					}
				}
			}
		}
		if (moved) this->newNum();
	}

	void input() {
		int input = getch();

		if (input == 3) { // Checks for ctrl-c
			exit(0);
		} else if (this->prevInput == 224) { // Checks for arrow presses
			if (input == 72) { // Up arrow
				this->move(UP);
			} else if (input == 80) { // Down arrow
				this->move(DOWN);
			} else if (input == 75) { // Left arrow
				this->move(LEFT);
			} else if (input == 77) { // Right arrow
				this->move(RIGHT);
			}
		} else if (input == 113) { // q
			this->running = false;
		}
		this->prevInput = input;
	}
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"
int main(int argc, char *argv[]) {
#pragma clang diagnostic pop
	Game game;
	if (argc > 1) {
		int debugMap[4][4] = {
				{16, 0, 8, 2},
				{0, 0, 4, 2},
				{0, 0, 0, 0},
				{0, 0, 0, 4}
		};
		game.debug(UP, debugMap);
		return 0;
	}
	game.run();
	return 0;
}
