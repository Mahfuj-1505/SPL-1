#include <bits/stdc++.h>
#include <random>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// Global Variables
const int GRID_SIZE = 9; 
vector<int> NUMBERS = {1, 3, 5, 7, 9, 11, 13, 15, 17};
vector<vector<int>> solvedGrid(GRID_SIZE, vector<int>(GRID_SIZE, 0));
int numTypeChoice;

// Function Prototypes
void traditionalSudoku();
void evenSudoku();
void chooseSudokuNumType();
void shuffleNumbers();
bool isValid(vector<vector<int>>& grid, int row, int col, int num);
void printGridWithAlphabet(const vector<vector<int>>& grid);
bool fillGrid(vector<vector<int>>& grid);
void removeNumbers(vector<vector<int>>& grid, int holes);
void printGrid(const vector<vector<int>>& grid);
vector<vector<int>> generateSudoku();
bool isCorrect(int row, int col, int element);
bool  isFilled(vector<vector<int>> grid);
bool startGame(vector<vector<int>> grid);

bool isCorrect(int row, int col, int element) {
    if(solvedGrid[row-1][col-1] == element) return true;
    return false;
}

bool  isFilled(vector<vector<int>> grid) {
    for(int i = 0; i<GRID_SIZE; i++) {
        for(int j = 0; j<GRID_SIZE; j++) {
            if(grid[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool startGame(vector<vector<int>> grid) {
    int row, col, element;
    int lives = 3;
    cout << "Lets Start the game. You have 3 lives." << endl;
    cout << "If you want to quit enter -1 -1 -1" << endl;
    while(1) {
        cout << "Enter element(row, col, element.): " << endl;
        cin >> row >> col >> element;
        if( row == -1 and col == -1 and element == -1) {
            return false;
        }
        else if((row or col or element) > 9 or (row or col or element) < 1 ) {
            cout << "Invalid row, col or element. Enter again." << endl;
            continue;
        }
        if(isCorrect(row, col, element)) {
            grid[row-1][col-1] = element;
            printGrid(grid);
            continue;
        }
        else {
            lives--;
            cout << "You have entered wrong element. Lives remaining: " << lives << endl;
        }
        if(lives == 0) {
            cout << "You are out of lives" << endl; 
            cout << "********GAME OVER********" << endl;
            return false;
        }
        if(isFilled(grid)) {
            cout << "Congratulation You Have WON the game" << endl;
            return true;
        }

    }
    
}

int main() {
    srand(time(0));

    cout << "What Elements do you wanna use in the sudoku?" << endl;
    cout << "1. Traditional Sudoku .(1,2,3......8,9)" << endl;
    cout << "2. Odd number Sudoku [default].(1,3,5....,15,17)" << endl;
    cout << "3. Even number Sudoku. (2,4,6....,16,18)" << endl;
    cout << "4. Alphabet Sudoku. (A, B, C,..., H, I)" << endl;
    cout << "Enter your choice: ";
    cin >> numTypeChoice;
    chooseSudokuNumType();

    vector<vector<int>> grid = generateSudoku();
    cout << "\nGenerated Sudoku Puzzle:\n";
    printGrid(grid);
    beforeChoice:
    int choice;
    cout << "Press 1 to start solving." << endl; 
    cout << "Press 2 to see the solution." << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    if(choice == 1) {
        bool isWon = startGame(grid);
        if(!isWon) {
            cout << "\nHere is the solved grid:" << endl;
            printGrid(solvedGrid);
        }
    }
    else if(choice == 2) {
        cout << "\nHere is the solved grid:" << endl;
        printGrid(solvedGrid);
    }
    else {
        cout << "Invalid Choice. Enter again." << endl;
        goto beforeChoice;
    }
    
    

    return 0;
}

void traditionalSudoku(){
    for(int i = 0; i<GRID_SIZE; i++) {
        NUMBERS[i] = i+1;
    }
}

void evenSudoku() {
    for(int i = 0; i<GRID_SIZE; i++) {
        NUMBERS[i] = 2*i + 2; 
    }
}

void chooseSudokuNumType() {
    if(numTypeChoice == 1) {
        cout << "\nSudoku elements set to traditional (1 to 9)." << endl;
        traditionalSudoku();
    }
    else if(numTypeChoice == 2) {
        cout << "\nSudoku elements set to odd numbers (1, 3,....., 15, 17)." << endl;
    }
    else if(numTypeChoice == 3) {
        cout << "\nSudoku elements set to even numbers (2, 4,....., 16, 18)." << endl;
        evenSudoku();
    }
    else if(numTypeChoice == 4) {
        cout << "\nSudoku elements set to Alphabet (A, B, C,..., H, I)." << endl;
        traditionalSudoku();
    }
    else {
        cout << "\nInvalid Choice. Defaulting to odds (1, 3,....., 15, 17)." << endl;
    }
}

void shuffleNumbers() {
    random_device rd; 
    mt19937 rng(rd()); 

    shuffle(NUMBERS.begin(), NUMBERS.end(), rng);
}


bool isValid(vector<vector<int>>& grid, int row, int col, int num) {
    for (int i = 0; i < GRID_SIZE; i++) {
        if (grid[row][i] == num || grid[i][col] == num) {
            return false;
        }
        if (grid[row / 3 * 3 + i / 3][col / 3 * 3 + i % 3] == num) {
            return false;
        }
    }
    return true;
}

bool fillGrid(vector<vector<int>>& grid) {
    shuffleNumbers();
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == 0) { 
                for (int num : NUMBERS) {
                    if (isValid(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (fillGrid(grid)) {
                            return true;
                        }
                        grid[row][col] = 0; 
                    }
                }
                return false;
            }
        }
    }
    return true;
}


void removeNumbers(vector<vector<int>>& grid, int holes) {
    int count = holes;
    while (count > 0) {
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;
        if (grid[row][col] != 0) {
            grid[row][col] = 0;
            count--;
        }
    }
}

void printGridWithAlphabet(const vector<vector<int>>& grid) {
    cout << "+---------+---------+---------+" << endl; 
    
    for (int row = 0; row < GRID_SIZE; row++) {
        cout << "|"; 
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == 0) {
                cout << " . ";
            }
            else {
                char ch = grid[row][col] + 'A' - 1;
                cout << " " << ch << " ";
            }

            if ((col + 1) % 3 == 0) {
                cout << "|"; 
            }
        }
        cout << endl;

        if ((row + 1) % 3 == 0 && row != GRID_SIZE - 1) {
                cout << "+---------+---------+---------+" << endl; 
        }
    }
    cout << "+---------+---------+---------+" << endl; 
    

}

void printGrid(const vector<vector<int>>& grid) {
    if(numTypeChoice == 4) {
        printGridWithAlphabet(grid);
        return;
    }
    if(numTypeChoice == 1) {
        cout << "+---------+---------+---------+" << endl; 
    }
    else {
        cout << "+------------+------------+------------+" << endl; 
    }
    for (int row = 0; row < GRID_SIZE; row++) {
        cout << "|"; 
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == 0) {
                if(numTypeChoice == 1) {
                    cout << " . ";
                }
                else {
                    cout << " .. "; 
                }
            }
            else {
                if(numTypeChoice == 1) {
                    cout << " " << grid[row][col] << " ";
                }
                else {
                    cout << " " << setw(2) << setfill('0') << grid[row][col] << " ";
                }
            }

            if ((col + 1) % 3 == 0) {
                cout << "|"; 
            }
        }
        cout << endl;

        if ((row + 1) % 3 == 0 && row != GRID_SIZE - 1) {
            if(numTypeChoice == 1) {
                cout << "+---------+---------+---------+" << endl; 
            }
            else {
                cout << "+------------+------------+------------+" << endl; 
            }
        }
    }
    if(numTypeChoice == 1) {
        cout << "+---------+---------+---------+" << endl; 
    }
    else {
        cout << "+------------+------------+------------+" << endl; 
    }
}

vector<vector<int>> generateSudoku() {
    vector<vector<int>> grid(GRID_SIZE, vector<int>(GRID_SIZE, 0));
    
    fillGrid(grid);

    cout << "\nChoose difficulty level:\n";
    cout << "1. Easy (20 to 30 holes)" << endl;
    cout << "2. Medium (30 to 40 holes)" << endl;
    cout << "3. Hard (40 to 50 holes)" << endl;
    cout << "4. Expert (50 to 60 holes)" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    int holes;
    switch (choice) {
        case 1:
            holes = 25; // Easy: 20-30 holes
            break;
        case 2:
            holes = 35; // Medium: 30-40 holes
            break;
        case 3:
            holes = 45; // Hard: 40-50 holes
            break;
        case 4:
            holes = 55; // Expert: 50-60 holes
            break;
        default:
            cout << "Invalid choice, defaulting to Medium.\n";
            holes = 35;
    }

    solvedGrid = grid;
    removeNumbers(grid, holes);
    return grid;

}
