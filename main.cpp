#include <iostream>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <ctime>
#include <cmath>

using namespace std;

const int n = 9;

#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

void display_number_with_color(int number, bool is_user_input) {
    if (is_user_input) {
        cout << ANSI_COLOR_YELLOW << number << ANSI_COLOR_RESET;
    }
    else {
        cout << ANSI_COLOR_BLUE << number << ANSI_COLOR_RESET;
    }
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear")
#endif
}

void press_return() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nPress 'enter' to return.";
    cin.get();
    clear_screen();
}

void sudoku_board(int grid[n][n], bool user_input[n][n]) {
    cout << "   ";
    for (int col = 9; col < n; col++) {
        cout << col + 1 << " ";
        if (col == 2 || col == 5)
        cout << "| ";
    }
    cout << endl;
	cout << "  ----------------------\n";

	for (int row = 0; row < n; row++) {
		cout << row + 1 << "| ";
		for (int col = 0; col < n; col++) {
			if (grid[row][col] != 0) {
				display_number_with_color(grid[row][col], user_input[row][col]);
				cout << " ";
			}
			else {
				cout << grid[row][col] << " ";
			}
			if (col == 2 || col == 5)
				cout << "| ";
		}
		cout << endl;

		if (row == 2 || row == 5) {
			cout << "  ----------------------" << endl;
		}
	}
	cout << ANSI_COLOR_RESET;
}

bool is_number_valid(int grid[n][n], int row, int col, int num) {
	for (int x = 0; x < n; x++)
		if (grid[row][x] == num)
			return false;

	for (int x = 0; x < n; x++)
		if (grid[x][col] == num)
			return false;

	int start_row = row - row % 3, start_col = col - col % 3;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (grid[i + start_row][j + start_col] == num)
				return false;

	return true;
}

bool board_complete(int board[n][n]) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] == 0) {
				return false;
			}
		}
	}
	return true;
}

bool solution_checker(int board[n][n]) {
	for (int row = 0; row < n; row++) {
		for (int col = 0; col < n; col++) {
			if (board[row][col] != 0) {
				int num = board[row][col];
				board[row][col] = 0;
				if (!is_number_valid(board, row, col, num)) {
					board[row][col] = num;
					return false;
				}
				board[row][col] = num;
			}
		}
	}
	return true;
}

void start_game(int board[n][n], bool fixed[n][n], bool user_input[n][n]) {
	int choice;
	int row, col, num;
	int tries = 0;

	memset(user_input, false, sizeof(bool) * n * n);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (board[i][j] != 0) {
				fixed[i][j] = true;
			}
		}
	}

	while (true) {
		sudoku_board(board, user_input);
		cout << "\nEnter row (1-9) or -1 to exit: ";
		cin >> row;
		if (row == -1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nPress 'enter' to return.";
			cin.get();
			clear_screen();
			break;
		}
		cout << "Enter column (1-9): ";
		cin >> col;
		cout << "Enter number (1-9): ";
		cin >> num;
		cout << "\n";

		row--;
		col--;

		if (row >= 0 && row < n && col >= 0 && col < n && num >= 1 && num <= 9) {
			if (!fixed[row][col]) {
				board[row][col] = num;
				user_input[row][col] = true;
				clear_screen();
			}
			else {
				clear_screen();
				cout << "You cannot change the initially given number.\n" << endl;
			}
		}
		else {
			clear_screen();
			cout << "\nInvalid input.\n" << endl;
		}

		if (board_complete(board)) {
			sudoku_board(board, user_input);
			cout << "\n[1] Submit Board [2] Continue Editting: ";
			cin >> choice;

			if (choice == 1) {
				if (solution_checker(board)) {
					cout << "\nYou have solved the puzzle!" << endl;
					break;
				}
				else {
					cout << "\nPuzzle not solved." << endl;
					tries++;
					if (tries <= 0) {
						cout << "You have reached the maximum amount of tries." << endl;
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "\nPress 'enter' to retun to menu.";
						cin.get();
						clear_screen();
						break;
					}
					else {
						if (tries == 1) {
							cout << "You only have" << tries << " try left to solve the puzzle." << endl;
						}
                        else if (tries == 2) {
                            cout << "You only have" << tries << ". tries left to solve the puzzle" << endl;
                        }
						else {
							cout << "You only have" << tries << " tries left to solve the puzzle." << endl;
						}
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "\nPress 'enter' to continue playing.";
						cin.get();
						clear_screen();
					}
				}
			}
			else if (choice == 2) {
				clear_screen();
			}
			else {
				cout << "Invalid choice." << endl;
			}
		}
	}
}

void puzzles(int board[n][n], bool fixed[n][n], bool user_input[n][n]) {
	int random_puzzle = 1 + rand() % 3;

	int board1[n][n] = {
		{0, 0, 0, 2, 6, 0, 7, 0, 1},
		{6, 8, 0, 0, 7, 0, 0, 9, 0},
		{1, 9, 0, 0, 0, 4, 5, 0, 0},
		{8, 2, 0, 1, 0, 0, 0, 4, 0},
		{0, 0, 4, 6, 0, 2, 9, 0, 0},
		{0, 5, 0, 0, 0, 3, 0, 2, 8},
		{0, 0, 9, 3, 0, 0, 0, 7, 4},
		{0, 4, 0, 0, 5, 0, 0, 3, 6},
		{7, 0, 3, 0, 1, 8, 0, 0, 0}
	};

	int board2[n][n] = {
		{1, 0, 0, 4, 8, 9, 0, 0, 6},
		{7, 3, 0, 0, 0, 0, 0, 4, 0},
		{0, 0, 0, 0, 0, 1, 2, 9, 5},
		{0, 0, 7, 1, 2, 0, 6, 0, 0},
		{5, 0, 0, 7, 0, 3, 0, 0, 8},
		{0, 0, 6, 0, 9, 5, 7, 0, 0},
		{9, 1, 4, 6, 0, 0, 0, 0, 0},
		{0, 2, 0, 0, 0, 0, 0, 3, 7},
		{8, 0, 0, 5, 1, 2, 0, 0, 4}
	};

	int board3[n][n] = {
		{6, 5, 9, 0, 1, 0, 2, 8, 0},
		{1, 0, 0, 0, 5, 0, 0, 3, 0},
		{2, 0, 0, 8, 0, 0, 0, 1, 0},
		{0, 0, 0, 1, 3, 5, 0, 7, 0},
		{8, 0, 0, 9, 0, 0, 0, 0, 2},
		{0, 0, 3, 0, 7, 8, 6, 4, 0},
		{3, 0, 2, 0, 0, 9, 0, 0, 4},
		{0, 0, 0, 0, 0, 1, 8, 0, 0},
		{0, 0, 8, 7, 6, 0, 0, 0, 0}
	};

	switch (random_puzzle) {
	case 1:
		memcpy(board, board1, sizeof(board1));
		break;
	case 2:
		memcpy(board, board2, sizeof(board2));
		break;
	case 3:
		memcpy(board, board3, sizeof(board3));
		break;
	}
	start_game(board, fixed, user_input);
}

void instructions() {
	cout << "============================" << endl;
	cout << "  INSTRUCTIONS OF THE GAME\n" << endl;
	cout << "Objectives:" << endl;
	cout << "-----------" << endl;
	cout << "To fill a 9x9 grid with numbers so that each column, each row, and each of the nine 3x3 subgrids contain all the numbers from 1 to 9.\n" << endl;
	cout << "Basic Rules:" << endl;
	cout << "------------" << endl;
	cout << "1. Grid Layout" << endl;
	cout << "   - The grid is divided into nine 3x3 subgrids." << endl;
	cout << "   - Each row has 9 cells." << endl;
	cout << "   - Each column has 9 cells." << endl;
	cout << "2. Starting Grid" << endl;
	cout << "   - The puzzle starts with some cells filled with numbers." << endl;
	cout << "   - The difficulty of the puzzle depends on the number and placement of these givens." << endl;
	cout << "3. Filling the Grid" << endl;
	cout << "   - Each row must contain the numbers 1-9 without repetition." << endl;
	cout << "   - Each column must contain the numbers 1-9 without repetition." << endl;
	cout << "   - Each 3x3 subgrid must contain the numbers 1-9 without repetition." << endl;
	cout << "4. No Guessing" << endl;
	cout << "   - A valid sudoku puzzle should have only one solution and can be solved using logic alone.\n" << endl;
	cout << "How to play?" << endl;
	cout << "------------" << endl;
	cout << "1. Choose a row" << endl;
	cout << "2. Choose a column" << endl;
	cout << "3. Input a number from 1-9\n" << endl;
	cout << "Winning:" << endl;
	cout << "--------" << endl;
	cout << "The game is won when all rows and columns are filled correctly.\n" << endl;

	press_return();
}

void main_menu() {
	srand(static_cast<unsigned int>(time(0)));
	int choice;

	int board[n][n];
	bool fixed[n][n] = { false };
	bool userInput[n][n] = { false };

	while (true) {
		cout << "+----------------------------------+" << endl;
		cout << "|       Welcome to Sudoku!         |" << endl;
		cout << "+----------------------------------+" << endl;
		cout << "| Select an option:                |" << endl;
		cout << "| [1] Start the Game               |" << endl;
		cout << "| [2] Instructions                 |" << endl;
        cout << "+----------------------------------+" << endl;
		cout << "| [3] Exit                         |" << endl;
		cout << "+----------------------------------+" << endl;
		cout << " Enter you choice: ";

		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			clear_screen();
			cout << "Please enter a valid choice from the menu.\n" << endl;
			continue;
		}

		switch (choice) {
		case 1:
			clear_screen();
			puzzles(board, fixed, userInput);
			break;
		case 2:
			clear_screen();
			instructions();
			break;
		case 3:
			cout << "Exiting..." << endl;
			exit;
		default:
			clear_screen();
			cout << "Please enter a valid choice from the menu.\n" << endl;
		}
	}
}

int main() {
	main_menu();

	return 0;
}