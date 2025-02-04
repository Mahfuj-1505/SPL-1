#include <bits/stdc++.h>
#include <random>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

// TODO: WINDOKU(heuristic algorithm), KILLER SUDOKU
// FIXME: added a shuffle numbers before generating sudoku in main(fixed)

// Global Variables
#define LIVES 3
const int GRID_SIZE = 9;
vector<int> NUMBERS = {1, 3, 5, 7, 9, 11, 13, 15, 17};
vector<vector<int>> solvedGrid(GRID_SIZE, vector<int>(GRID_SIZE, 0));
int numTypeChoice;

// Function Prototypes
void traditionalSudoku();
void evenSudoku();
void chooseSudokuNumType();
void shuffleNumbers(vector<int> &NUMBERS);
bool isValid(vector<vector<int>> &grid, int row, int col, int num);
void printGridWithAlphabet(const vector<vector<int>> &grid);
bool fillGrid(vector<vector<int>> &grid);
void removeNumbers(vector<vector<int>> &grid, int holes);
void printGrid(const vector<vector<int>> &grid);
vector<vector<int>> generateSudoku();
bool isCorrect(int row, int col, int element);
bool isFilled(vector<vector<int>> grid);
bool startGame(vector<vector<int>> grid);
bool isDiagonalSudokuValid(vector<vector<int>> &grid, int row, int col, int num);
bool fillDiagonalSudokuGrid(vector<vector<int>> &grid);
void provideHint(const vector<vector<int>> &grid);
vector<int> getValidNumbers(int row, int col, const vector<vector<int>> &grid);
int randomInRange(int min, int max);
void shuffleNumbers(vector<int> &NUMBERS);
bool isValidWindoku(vector<vector<int>> &grid, int row, int col, int num);
bool fillWindokuGrid(vector<vector<int>> &grid);
void saveSudoku(vector<vector<int>> &grid, int lives);

// TODO: added x-wing and swordfish algorithm to solve a given sudoku.
// TODO: need to add it to the rest of the program

bool applyXWing(vector<vector<int>> &grid, int num)
{
    vector<vector<int>> positions(GRID_SIZE);

    for (int row = 0; row < GRID_SIZE; row++)
    {
        vector<int> cols;
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0 && isValid(grid, row, col, num))
            {
                cols.push_back(col);
            }
        }
        if (cols.size() == 2)
        {
            positions[row] = cols;
        }
    }

    for (int r1 = 0; r1 < GRID_SIZE; r1++)
    {
        for (int r2 = r1 + 1; r2 < GRID_SIZE; r2++)
        {
            if (!positions[r1].empty() && positions[r1] == positions[r2])
            {
                for (int r = 0; r < GRID_SIZE; r++)
                {
                    if (r != r1 && r != r2)
                    {
                        for (int col : positions[r1])
                        {
                            if (grid[r][col] == 0 && isValid(grid, r, col, num))
                            {
                                grid[r][col] = -1; // Mark for removal
                            }
                        }
                    }
                }
                return true;
            }
        }
    }
    return false;
}

bool applySwordfish(vector<vector<int>> &grid, int num)
{
    vector<vector<int>> positions(GRID_SIZE);

    for (int row = 0; row < GRID_SIZE; row++)
    {
        vector<int> cols;
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0 && isValid(grid, row, col, num))
            {
                cols.push_back(col);
            }
        }
        if (cols.size() == 3)
        {
            positions[row] = cols;
        }
    }

    for (int r1 = 0; r1 < GRID_SIZE; r1++)
    {
        for (int r2 = r1 + 1; r2 < GRID_SIZE; r2++)
        {
            for (int r3 = r2 + 1; r3 < GRID_SIZE; r3++)
            {
                if (!positions[r1].empty() && positions[r1] == positions[r2] && positions[r2] == positions[r3])
                {
                    for (int r = 0; r < GRID_SIZE; r++)
                    {
                        if (r != r1 && r != r2 && r != r3)
                        {
                            for (int col : positions[r1])
                            {
                                if (grid[r][col] == 0 && isValid(grid, r, col, num))
                                {
                                    grid[r][col] = -1; // Mark for removal
                                }
                            }
                        }
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool solveSudoku(vector<vector<int>> &grid)
{
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0)
            {
                for (int num = 1; num <= 9; num++)
                {
                    if (isValid(grid, row, col, num))
                    {
                        grid[row][col] = num;
                        if (solveSudoku(grid))
                            return true;
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// this function integrates xwing and swordfish before using  the brute force algorithm
void applyAdvancedTechniques(vector<vector<int>> &grid)
{
    for (int num = 1; num <= 9; num++)
    {
        applyXWing(grid, num);
        applySwordfish(grid, num);
    }
}

// TODO:

int main()
{
    srand(time(0));

    cout << "What Elements do you wanna use in the sudoku?" << endl;
    cout << "1. Traditional Sudoku .(1,2,3......8,9)" << endl;
    cout << "2. Odd number Sudoku [default].(1,3,5....,15,17)" << endl;
    cout << "3. Even number Sudoku. (2,4,6....,16,18)" << endl;
    cout << "4. Alphabet Sudoku. (A, B, C,..., H, I)" << endl;
    cout << "Enter your choice: ";
    cin >> numTypeChoice;
    chooseSudokuNumType();
    shuffleNumbers(NUMBERS);
    vector<vector<int>> grid = generateSudoku();
    cout << "\nGenerated Sudoku Puzzle:\n";
    printGrid(grid);
beforeChoice:
    int choice;
    cout << "Press 1 to start solving." << endl;
    cout << "Press 2 to see the solution." << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    if (choice == 1)
    {
        bool isWon = startGame(grid);
        if (!isWon)
        {
            cout << "\nHere is the solved grid:" << endl;
            printGrid(solvedGrid);
        }
    }
    else if (choice == 2)
    {
        cout << "\nHere is the solved grid:" << endl;
        printGrid(solvedGrid);
    }
    else
    {
        cout << "Invalid Choice. Enter again." << endl;
        goto beforeChoice;
    }

    return 0;
}

void saveSudoku(vector<vector<int>> &grid, int lives)
{
    ofstream outFile("saved.txt");
    if (!outFile)
    {
        cerr << "Error opening file!" << endl;
        return;
    }

    for (const auto &row : grid)
    {
        for (int num : row)
        {
            outFile << num << " ";
        }
        outFile << endl;
    }

    outFile << "Lives: " << lives << endl;

    outFile.close();
    cout << "Sudoku grid and lives saved successfully." << endl;
}

// TODO: add randomInrange and update shuffle number in all the rest of the program
int randomInRange(int min, int max)
{
    return min + rand() % (max - min + 1);
}

void shuffleNumbers(vector<int> &NUMBERS)
{
    // Use a random device to seed the random number generator
    int size = NUMBERS.size();
    for (int i = size - 1; i > 0; --i)
    {
        int j = randomInRange(0, i);
        swap(NUMBERS[i], NUMBERS[j]);
    }
}

bool isValidWindoku(vector<vector<int>> &grid, int row, int col, int num)
{
    // Check row and column
    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (grid[row][i] == num || grid[i][col] == num)
        {
            return false;
        }
    }

    // Check 3x3 standard subgrid
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++)
    {
        for (int c = startCol; c < startCol + 3; c++)
        {
            if (grid[r][c] == num)
            {
                return false;
            }
        }
    }

    // Check Windoku shaded region
    if ((row >= 1 && row <= 3 && col >= 1 && col <= 3) ||
        (row >= 1 && row <= 3 && col >= 5 && col <= 7) ||
        (row >= 5 && row <= 7 && col >= 1 && col <= 3) ||
        (row >= 5 && row <= 7 && col >= 5 && col <= 7))
    {

        int startWindokuRow = (row >= 5) ? 5 : 1;
        int startWindokuCol = (col >= 5) ? 5 : 1;

        for (int r = startWindokuRow; r < startWindokuRow + 3; r++)
        {
            for (int c = startWindokuCol; c < startWindokuCol + 3; c++)
            {
                if (grid[r][c] == num)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool fillWindokuGrid(vector<vector<int>> &grid)
{
    shuffleNumbers(NUMBERS); // Shuffle NUMBERS vector for random placement
    // shuffleNumbers(); // Shuffle NUMBERS vector for random placement
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0)
            {
                for (int num : NUMBERS)
                {
                    if (isValidWindoku(grid, row, col, num))
                    {
                        grid[row][col] = num;
                        if (fillWindokuGrid(grid))
                        {
                            return true;
                        }
                        grid[row][col] = 0; // Backtrack
                    }
                }
                return false; // No valid number found
            }
        }
    }
    return true; // Grid filled successfully
}

bool isDiagonalSudokuValid(vector<vector<int>> &grid, int row, int col, int num)
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (grid[row][i] == num || grid[i][col] == num)
        {
            return false;
        }

        if (grid[row / 3 * 3 + i / 3][col / 3 * 3 + i % 3] == num)
        {
            return false;
        }
    }

    if (row == col)
    {
        for (int i = 0; i < GRID_SIZE; i++)
        {
            if (grid[i][i] == num)
            {
                return false;
            }
        }
    }

    if (row + col == GRID_SIZE - 1)
    {
        for (int i = 0; i < GRID_SIZE; i++)
        {
            if (grid[i][GRID_SIZE - 1 - i] == num)
            {
                return false;
            }
        }
    }

    return true;
}

bool fillDiagonalSudokuGrid(vector<vector<int>> &grid)
{
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0)
            {
                for (int num : NUMBERS)
                {
                    if (isDiagonalSudokuValid(grid, row, col, num))
                    {
                        grid[row][col] = num;
                        if (fillDiagonalSudokuGrid(grid))
                        {
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

bool startGameWithAlp(vector<vector<int>> grid)
{
    int row, col;
    char ch;
    int lives = LIVES;
    cout << "Lets Start the game. You have 3 lives." << endl;
    cout << "If you want to quit enter -1 -1 -1" << endl;
    while (1)
    {
        cout << "Enter element(row, col, element.): " << endl;
        cin >> row >> col >> ch;
        int element = ch - 'A' + 1;
        if (row == -1 and col == -1 and element == -1)
        {
            return false;
        }
        else if (row > 9 or row < 1 or col > 9 or col < 1)
        {
            cout << "Invalid row or col. Enter again." << endl;
            continue;
        }
        if (isCorrect(row, col, element))
        {
            grid[row - 1][col - 1] = element;
            printGrid(grid);
            continue;
        }
        else
        {
            lives--;
            cout << "You have entered wrong element. Lives remaining: " << lives << endl;
        }
        if (lives == 0)
        {
            cout << "You are out of lives" << endl;
            cout << "********GAME OVER********" << endl;
            return false;
        }
        if (isFilled(grid))
        {
            cout << "*******Congratulation You Have WON the game*******" << endl;
            return true;
        }
    }
}

vector<int> getValidNumbers(int row, int col, const vector<vector<int>> &grid)
{
    vector<int> validNumbers;

    for (int num : NUMBERS)
    {
        bool isValid = true;

        for (int i = 0; i < GRID_SIZE; i++)
        {
            if (grid[row][i] == num || grid[i][col] == num)
            {
                isValid = false;
                break;
            }
        }

        if (isValid)
        {
            int startRow = (row / 3) * 3;
            int startCol = (col / 3) * 3;
            for (int r = startRow; r < startRow + 3; r++)
            {
                for (int c = startCol; c < startCol + 3; c++)
                {
                    if (grid[r][c] == num)
                    {
                        isValid = false;
                        break;
                    }
                }
                if (!isValid)
                    break;
            }
        }

        if (isValid)
        {
            validNumbers.push_back(num);
        }
    }

    return validNumbers;
}

void provideHint(const vector<vector<int>> &grid)
{
    int minChoices = GRID_SIZE + 1;
    int bestRow = -1, bestCol = -1;
    vector<int> bestChoices;

    for (int row = 0; row < GRID_SIZE; ++row)
    {
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            if (grid[row][col] == 0)
            {
                vector<int> validNumbers = getValidNumbers(row, col, grid);
                if (validNumbers.size() < minChoices)
                {
                    minChoices = validNumbers.size();
                    bestRow = row;
                    bestCol = col;
                    bestChoices = validNumbers;
                }
            }
        }
    }

    if (bestRow != -1 && bestCol != -1)
    {
        cout << "Greedy Hint: Cell (" << bestRow + 1 << ", " << bestCol + 1 << ") has "
             << bestChoices.size() << " valid choices left. Suggested number: "
             << bestChoices[0] << endl;
    }
}

bool startGame(vector<vector<int>> grid)
{
    if (numTypeChoice == 4)
    {
        return startGameWithAlp(grid);
    }
    int row, col, element;
    int lives = LIVES;
    cout << "Let's start the game. You have 3 lives." << endl;
    cout << "If you want to quit, enter q or Q or -1 -1 -1." << endl;
    cout << "If you want a greedy hint, enter h or H or 0 0 0." << endl;

    while (1)
    {
        cout << "Enter element (row, col, element): ";
        string input;
        cin >> input;

        // Quit option
        if (input == "q" || input == "Q")
        {
            cout << "You have quit the game." << endl;
            return false;
        }
        // Hint option
        if (input == "h" or input == "H")
        {
            provideHint(grid);
            continue;
        }

        if (input == "s" or input == "S")
        {
            saveSudoku(grid, LIVES);
            cout << "The current sudoku grid and live count has been saved in file saved.txt." << endl;
            return false;
        }

        // Convert the input to integer if it's not 'q' or 'Q'
        row = stoi(input);
        cin >> col >> element;

        // for alterative quit option
        if (row == -1 && col == -1 && element == -1)
        {
            return false;
        }

        // for alterative hint option
        if (row == 0 && col == 0 && element == 0)
        {
            provideHint(grid);
            continue;
        }

        if (row > 9 || row < 1 || col > 9 || col < 1)
        {
            cout << "Invalid row or col. Enter again." << endl;
            continue;
        }

        if (isCorrect(row, col, element))
        {
            grid[row - 1][col - 1] = element;
            printGrid(grid);
            if (isFilled(grid))
            {
                cout << "******* Congratulations! You have WON the game! *******" << endl;
                return true;
            }
            continue;
        }
        else
        {
            lives--;
            cout << "You have entered the wrong element. Lives remaining: " << lives << endl;
        }

        if (lives == 0)
        {
            cout << "You are out of lives" << endl;
            cout << "******** GAME OVER ********" << endl;
            return false;
        }
    }
}

bool isFilled(vector<vector<int>> grid)
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (grid[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool isCorrect(int row, int col, int element)
{
    if (solvedGrid[row - 1][col - 1] == element)
        return true;
    return false;
}

void traditionalSudoku()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        NUMBERS[i] = i + 1;
    }
}

void evenSudoku()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        NUMBERS[i] = 2 * i + 2;
    }
}

void chooseSudokuNumType()
{
    if (numTypeChoice == 1)
    {
        cout << "\nSudoku elements set to traditional (1 to 9)." << endl;
        traditionalSudoku();
    }
    else if (numTypeChoice == 2)
    {
        cout << "\nSudoku elements set to odd numbers (1, 3,....., 15, 17)." << endl;
    }
    else if (numTypeChoice == 3)
    {
        cout << "\nSudoku elements set to even numbers (2, 4,....., 16, 18)." << endl;
        evenSudoku();
    }
    else if (numTypeChoice == 4)
    {
        cout << "\nSudoku elements set to Alphabet (A, B, C,..., H, I)." << endl;
        traditionalSudoku();
    }
    else
    {
        cout << "\nInvalid Choice. Defaulting to odds (1, 3,....., 15, 17)." << endl;
    }
}

bool isValid(vector<vector<int>> &grid, int row, int col, int num)
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (grid[row][i] == num || grid[i][col] == num)
        {
            return false;
        }
        if (grid[row / 3 * 3 + i / 3][col / 3 * 3 + i % 3] == num)
        {
            return false;
        }
    }
    return true;
}

bool fillGrid(vector<vector<int>> &grid)
{
    shuffleNumbers(NUMBERS);
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0)
            {
                for (int num : NUMBERS)
                {
                    if (isValid(grid, row, col, num))
                    {
                        grid[row][col] = num;
                        if (fillGrid(grid))
                        {
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

void removeNumbers(vector<vector<int>> &grid, int holes)
{
    int count = holes;
    while (count > 0)
    {
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;
        if (grid[row][col] != 0)
        {
            grid[row][col] = 0;
            count--;
        }
    }
}

void printGridWithAlphabet(const vector<vector<int>> &grid)
{
    cout << "+---------+---------+---------+" << endl;

    for (int row = 0; row < GRID_SIZE; row++)
    {
        cout << "|";
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0)
            {
                cout << " . ";
            }
            else
            {
                char ch = grid[row][col] + 'A' - 1;
                cout << " " << ch << " ";
            }

            if ((col + 1) % 3 == 0)
            {
                cout << "|";
            }
        }
        cout << endl;

        if ((row + 1) % 3 == 0 && row != GRID_SIZE - 1)
        {
            cout << "+---------+---------+---------+" << endl;
        }
    }
    cout << "+---------+---------+---------+" << endl;
}

void printGrid(const vector<vector<int>> &grid)
{
    if (numTypeChoice == 4)
    {
        printGridWithAlphabet(grid);
        return;
    }
    if (numTypeChoice == 1)
    {
        cout << "+---------+---------+---------+" << endl;
    }
    else
    {
        cout << "+------------+------------+------------+" << endl;
    }
    for (int row = 0; row < GRID_SIZE; row++)
    {
        cout << "|";
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0)
            {
                if (numTypeChoice == 1)
                {
                    cout << " . ";
                }
                else
                {
                    cout << " .. ";
                }
            }
            else
            {
                if (numTypeChoice == 1)
                {
                    cout << " " << grid[row][col] << " ";
                }
                else
                {
                    cout << " " << setw(2) << setfill('0') << grid[row][col] << " ";
                }
            }

            if ((col + 1) % 3 == 0)
            {
                cout << "|";
            }
        }
        cout << endl;

        if ((row + 1) % 3 == 0 && row != GRID_SIZE - 1)
        {
            if (numTypeChoice == 1)
            {
                cout << "+---------+---------+---------+" << endl;
            }
            else
            {
                cout << "+------------+------------+------------+" << endl;
            }
        }
    }
    if (numTypeChoice == 1)
    {
        cout << "+---------+---------+---------+" << endl;
    }
    else
    {
        cout << "+------------+------------+------------+" << endl;
    }
}

vector<vector<int>> generateSudoku()
{
    vector<vector<int>> grid(GRID_SIZE, vector<int>(GRID_SIZE, 0));
    cout << "Which sudoku do you want to play??" << endl;
    cout << "1. General SUdoku" << endl;
    cout << "2. Diagonal SUdoku" << endl;
    cout << "3. Windoku" << endl;
    cout << "Enter your choice: ";
    int sudokuTypeChoice;
    cin >> sudokuTypeChoice;
    if (sudokuTypeChoice == 1)
    {
        fillGrid(grid);
    }
    else if (sudokuTypeChoice == 2)
    {
        fillDiagonalSudokuGrid(grid);
    }
    else if (sudokuTypeChoice == 3)
    {
        fillWindokuGrid(grid);
    }
    else
    {
        cout << "Invalid Choice. Defaulting to general sudoku." << endl;
        fillGrid(grid);
    }

    cout << "\nChoose difficulty level:\n";
    cout << "1. Easy (20 to 30 holes)" << endl;
    cout << "2. Medium (30 to 40 holes)" << endl;
    cout << "3. Hard (40 to 50 holes)" << endl;
    cout << "4. Expert (50 to 60 holes)" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    int holes;
    switch (choice)
    {
    case 1:
        holes = 20;
        break;
    case 2:
        holes = 30;
        break;
    case 3:
        holes = 40;
        break;
    case 4:
        holes = 50;
        break;
    default:
        cout << "Invalid choice, defaulting to Medium.\n";
        holes = 30;
    }

    solvedGrid = grid;
    removeNumbers(grid, holes);
    return grid;
}
