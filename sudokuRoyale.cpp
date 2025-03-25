#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace sf;

// TODO:  xwing and swordfish

// Global Variables
#define LIVES 3
const int GRID_SIZE = 9;
vector<int> NUMBERS = {1, 3, 5, 7, 9, 11, 13, 15, 17};
vector<vector<int>> solvedGrid(GRID_SIZE, vector<int>(GRID_SIZE, 0));
int numTypeChoice;
int sudokuTypeChoice;
int difficultyChoice;
int selectedRow = -1, selectedCol = -1;
bool highlightMistakes = false; // Global variable to control mistake highlighting
int hintLimit = 5;              // Limit for the number of hints
int hintsUsed = 0;              // Counter for hints used

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
bool isValidWindoku(vector<vector<int>> &grid, int row, int col, int num);
bool fillWindokuGrid(vector<vector<int>> &grid);
void saveSudoku(vector<vector<int>> &grid, int lives, Time elapsedTime);
void loadSudoku(vector<vector<int>> &grid, int &lives, Time &elapsedTime);
void drawGrid(RenderWindow &window, const vector<vector<int>> &grid, const vector<vector<int>> &wrongEntries);
void drawButtons(RenderWindow &window, Font &font);
void drawMainMenu(RenderWindow &window, Font &font);
void drawSudokuOptions(RenderWindow &window, Font &font);
int calculateScore(int lives, int hintsUsed, Time totalTime, int difficultyChoice);
void drawButton(RenderWindow &window, Font &font, const string &textString, Vector2f position, Color fillColor);
void drawHint(RenderWindow &window, Font &font, const string &hint);
void drawLives(RenderWindow &window, Font &font, int lives);
void drawGameOver(RenderWindow &window, Font &font);
void drawCongratulation(RenderWindow &window, Font &font, int score, Time totalTime);
void drawTimer(RenderWindow &window, Font &font, Clock &clock, int hintsRemaining);
bool applyXWing(vector<vector<int>> &grid, int num);
bool applySwordfish(vector<vector<int>> &grid, int num);
bool solveSudoku(vector<vector<int>> &grid);
void applyAdvancedTechniques(vector<vector<int>> &grid);
bool solveWithAdvancedTechniques(vector<vector<int>> &grid);
bool startGameWithAlp(vector<vector<int>> grid);

//
void drawButton(RenderWindow &window, Font &font, const string &textString, Vector2f position, Color fillColor)
{
    RectangleShape button(Vector2f(200, 50));
    button.setPosition(position);
    button.setFillColor(fillColor);
    button.setOutlineThickness(2);
    button.setOutlineColor(Color::Black);
    window.draw(button);

    Text text;
    text.setFont(font);
    text.setString(textString);
    text.setCharacterSize(24);
    text.setFillColor(Color::Black);
    text.setPosition(position.x + 50, position.y + 10);
    window.draw(text);
}

void drawHint(RenderWindow &window, Font &font, const string &hint)
{
    Text hintText;
    hintText.setFont(font);
    hintText.setString(hint);
    hintText.setCharacterSize(24);
    hintText.setFillColor(Color::Black);
    hintText.setPosition(50, 700);
    window.draw(hintText);
}

void drawLives(RenderWindow &window, Font &font, int lives)
{
    wstring livesString = L"Lives: " + wstring(lives, L'\u2764'); // L'\u2764' represents the heart emoji

    Text livesText(livesString, font, 30);

    livesText.setFillColor(Color::Red);

    window.draw(livesText);
}

void drawGameOver(RenderWindow &window, Font &font)
{
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("   \tYou are out of lives\n******** GAME OVER ********");
    gameOverText.setCharacterSize(36);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(200, 300);
    window.draw(gameOverText);

    drawButton(window, font, "Restart", Vector2f(300, 500), Color::Cyan);
    drawButton(window, font, "Main Menu", Vector2f(300, 600), Color::Magenta);
}

void drawCongratulation(RenderWindow &window, Font &font, int score, Time totalTime)
{
    Text congratsText;
    congratsText.setFont(font);
    congratsText.setString("******* Congratulations! *******\n You have WON the game! ");
    congratsText.setCharacterSize(36);
    congratsText.setFillColor(Color::Green);
    congratsText.setPosition(100, 300);
    window.draw(congratsText);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(100, 400);
    window.draw(scoreText);

    Text timeText;
    timeText.setFont(font);
    int minutes = static_cast<int>(totalTime.asSeconds()) / 60;
    int seconds = static_cast<int>(totalTime.asSeconds()) % 60;
    timeText.setString("Time Taken: " + to_string(minutes) + " minutes " + to_string(seconds) + " seconds");
    timeText.setCharacterSize(24);
    timeText.setFillColor(Color::Black);
    timeText.setPosition(100, 450);
    window.draw(timeText);

    drawButton(window, font, "Restart", Vector2f(300, 500), Color::Cyan);
    drawButton(window, font, "Main Menu", Vector2f(300, 600), Color::Magenta);
}

void drawTimer(RenderWindow &window, Font &font, Clock &clock, int hintsRemaining)
{
    Time elapsed = clock.getElapsedTime();
    int minutes = static_cast<int>(elapsed.asSeconds()) / 60;
    int seconds = static_cast<int>(elapsed.asSeconds()) % 60;

    Text timerText;
    timerText.setFont(font);
    timerText.setString("Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds));
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::Black);
    timerText.setPosition(700, 50);
    window.draw(timerText);

    Text hintsText;
    hintsText.setFont(font);
    hintsText.setString("Hints left: " + to_string(hintLimit - hintsUsed));
    hintsText.setCharacterSize(24);
    hintsText.setFillColor(Color::Black);
    hintsText.setPosition(700, 80);
    window.draw(hintsText);
}

bool solveWithAdvancedTechniques(vector<vector<int>> &grid)
{
    vector<vector<int>> tempGrid = grid;
    applyAdvancedTechniques(tempGrid);
    return solveSudoku(tempGrid);
}

bool isCorrect(int row, int col, int element)
{
    vector<vector<int>> tempGrid = solvedGrid;
    if (solveWithAdvancedTechniques(tempGrid) && tempGrid[row - 1][col - 1] == element)
    {
        return true;
    }
    return solvedGrid[row - 1][col - 1] == element;
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(900, 900), "Sudoku Main Menu");
    Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        cerr << "Error loading font" << endl;
        return -1;
    }

    bool inMainMenu = true;
    bool inGame = false;
    bool inOptions = false;
    bool gameOver = false;
    bool gameWon = false;
    vector<vector<int>> grid(GRID_SIZE, vector<int>(GRID_SIZE, 0));
    vector<vector<int>> wrongEntries(GRID_SIZE, vector<int>(GRID_SIZE, 0));
    string hintMessage = "";
    int lives = LIVES;
    Clock gameClock;
    Time savedTime;
    Time totalTime;
    int score = 0;
    string inputBuffer = ""; // Buffer to store multi-digit input

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    if (inMainMenu)
                    {
                        if (x > 300 && x < 500)
                        {
                            if (y > 300 && y < 350)
                            { // New Game button
                                inMainMenu = false;
                                inOptions = true;
                            }
                            else if (y > 400 && y < 450)
                            { // Load Game button
                                loadSudoku(grid, lives, savedTime);
                                inMainMenu = false;
                                inGame = true;
                                gameClock.restart();
                                savedTime += gameClock.getElapsedTime();
                            }
                            else if (y > 500 && y < 550)
                            { // Exit button
                                window.close();
                            }
                        }
                    }
                    else if (inOptions)
                    {
                        if (x > 50 && x < 250)
                        {
                            if (y > 100 && y < 150)
                            { // Traditional button
                                numTypeChoice = 1;
                            }
                            else if (y > 200 && y < 250)
                            { // Even Num button
                                numTypeChoice = 3;
                            }
                            else if (y > 350 && y < 400)
                            { // General button
                                sudokuTypeChoice = 1;
                            }
                            else if (y > 450 && y < 500)
                            { // Windoku button
                                sudokuTypeChoice = 3;
                            }
                            else if (y > 600 && y < 650)
                            { // Easy button
                                difficultyChoice = 1;
                            }
                            else if (y > 700 && y < 750)
                            { // Hard button
                                difficultyChoice = 3;
                            }
                        }
                        else if (x > 300 && x < 500)
                        {
                            if (y > 100 && y < 150)
                            { // Odd Num button
                                numTypeChoice = 2;
                            }
                            else if (y > 200 && y < 250)
                            { // Alphabet button
                                numTypeChoice = 4;
                            }
                            else if (y > 350 && y < 400)
                            { // Diagonal button
                                sudokuTypeChoice = 2;
                            }
                            else if (y > 600 && y < 650)
                            { // Medium button
                                difficultyChoice = 2;
                            }
                            else if (y > 700 && y < 750)
                            { // Expert button
                                difficultyChoice = 4;
                            }
                        }
                        if (numTypeChoice != 0 && sudokuTypeChoice != 0 && difficultyChoice != 0)
                        {
                            chooseSudokuNumType();
                            shuffleNumbers(NUMBERS);
                            grid = generateSudoku();
                            wrongEntries = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
                            inOptions = false;
                            inGame = true;
                            gameClock.restart();
                            lives = LIVES;
                        }
                    }
                    else if (inGame)
                    {
                        if (y < 600)
                        {
                            selectedRow = (y - 50) / 60;
                            selectedCol = (x - 100) / 60;
                        }
                        else if (y > 650 && y < 700)
                        {
                            if (x > 50 && x < 150)
                            { // Save button
                                saveSudoku(grid, lives, gameClock.getElapsedTime());
                            }
                            else if (x > 200 && x < 300)
                            { // Load button
                                loadSudoku(grid, lives, savedTime);
                                gameClock.restart();
                                savedTime += gameClock.getElapsedTime();
                            }
                            else if (x > 350 && x < 450)
                            { // Hint button
                                if (selectedRow != -1 && selectedCol != -1)
                                {
                                    if (hintsUsed < hintLimit)
                                    {
                                        vector<int> validNumbers = getValidNumbers(selectedRow, selectedCol, grid);
                                        if (!validNumbers.empty())
                                        {
                                            hintMessage = "Hint: Cell (" + to_string(selectedRow + 1) + ", " + to_string(selectedCol + 1) + ") has " + to_string(validNumbers.size()) + " valid choices: " + to_string(validNumbers[0]);
                                            hintsUsed++;
                                        }
                                        else
                                        {
                                            hintMessage = "Hint: No valid choices available.";
                                        }
                                    }
                                    else
                                    {
                                        hintMessage = "Hint limit reached.";
                                    }
                                }
                                else
                                {
                                    hintMessage = "Hint: Select a cell first.";
                                }
                            }
                            else if (x > 500 && x < 600)
                            { // Solve button
                                grid = solvedGrid;
                                gameClock.restart();
                            }
                        }
                        else if (y > 750 && y < 800)
                        {
                            if (x > 50 && x < 150)
                            { // Restart button
                                grid = generateSudoku();
                                wrongEntries = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
                                gameClock.restart();
                                lives = LIVES;
                            }
                            else if (x > 200 && x < 300)
                            { // Main Menu button
                                inGame = false;
                                inMainMenu = true;
                            }
                            else if (x > 350 && x < 450)
                            { // Exit button
                                window.close();
                            }
                        }
                    }
                    else if (gameOver)
                    {
                        if (x > 300 && x < 500)
                        {
                            if (y > 500 && y < 550)
                            { // Restart button
                                grid = generateSudoku();
                                wrongEntries = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
                                gameOver = false;
                                inGame = true;
                                gameClock.restart();
                                lives = LIVES;
                            }
                            else if (y > 600 && y < 650)
                            { // Main Menu button
                                inMainMenu = true;
                                gameOver = false;
                            }
                        }
                    }
                    else if (gameWon)
                    {
                        if (x > 300 && x < 500)
                        {
                            if (y > 500 && y < 550)
                            { // Restart button
                                grid = generateSudoku();
                                wrongEntries = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
                                gameWon = false;
                                inGame = true;
                                gameClock.restart();
                                lives = LIVES;
                            }
                            else if (y > 600 && y < 650)
                            { // Main Menu button
                                inMainMenu = true;
                                gameWon = false;
                            }
                        }
                    }
                    if (x > 700 && x < 900 && y > 750 && y < 800)
                    { // Exit button
                        window.close();
                    }
                }
            }
            else if (inGame && event.type == Event::KeyPressed)
            {
                if (selectedRow != -1 && selectedCol != -1)
                {
                    if (numTypeChoice == 1 || numTypeChoice == 4)
                    { // Traditional or Alphabet
                        if (event.key.code >= Keyboard::Num1 && event.key.code <= Keyboard::Num9)
                        {
                            int num = event.key.code - Keyboard::Num1 + 1;
                            if (isCorrect(selectedRow + 1, selectedCol + 1, num))
                            {
                                grid[selectedRow][selectedCol] = num;
                                wrongEntries[selectedRow][selectedCol] = 0;
                                if (isFilled(grid))
                                {
                                    totalTime = gameClock.getElapsedTime() + savedTime;
                                    score = calculateScore(lives, hintsUsed, totalTime, difficultyChoice);
                                    cout << "******* Congratulations! You have WON the game! *******" << endl;
                                    inGame = false;
                                    gameWon = true;
                                }
                            }
                            else
                            {
                                lives--;
                                hintMessage = "Wrong number! Lives remaining: " + to_string(lives);
                                wrongEntries[selectedRow][selectedCol] = num;
                                if (lives == 0)
                                {
                                    cout << "    You are out of lives    " << endl;
                                    cout << "******** GAME OVER ********" << endl;
                                    inGame = false;
                                    gameOver = true;
                                }
                            }
                        }
                    }
                    else
                    { // Even or Odd
                        // Buffer multi-digit input and handle it on Enter key press
                        if (event.key.code >= Keyboard::Num0 && event.key.code <= Keyboard::Num9)
                        {
                            inputBuffer += static_cast<char>(event.key.code - Keyboard::Num0 + '0');
                        }
                        else if (event.key.code == Keyboard::Enter)
                        {
                            if (!inputBuffer.empty())
                            {
                                int num = stoi(inputBuffer);
                                if (isCorrect(selectedRow + 1, selectedCol + 1, num))
                                {
                                    grid[selectedRow][selectedCol] = num;
                                    wrongEntries[selectedRow][selectedCol] = 0;
                                    if (isFilled(grid))
                                    {
                                        totalTime = gameClock.getElapsedTime() + savedTime;
                                        score = calculateScore(lives, hintsUsed, totalTime, difficultyChoice);
                                        cout << "******* Congratulations! You have WON the game! *******" << endl;
                                        inGame = false;
                                        gameWon = true;
                                    }
                                }
                                else
                                {
                                    lives--;
                                    hintMessage = "Wrong number! Lives remaining: " + to_string(lives);
                                    wrongEntries[selectedRow][selectedCol] = num;
                                    if (lives == 0)
                                    {
                                        cout << "    You are out of lives    " << endl;
                                        cout << "******** GAME OVER ********" << endl;
                                        inGame = false;
                                        gameOver = true;
                                    }
                                }
                                inputBuffer.clear();
                            }
                        }
                        else if (event.key.code == Keyboard::BackSpace && !inputBuffer.empty())
                        {
                            inputBuffer.pop_back();
                        }
                    }
                }
            }
        }

        window.clear(Color::White);
        if (inMainMenu)
        {
            drawMainMenu(window, font);
        }
        else if (inOptions)
        {
            drawSudokuOptions(window, font);
        }
        else if (inGame)
        {
            drawGrid(window, grid, wrongEntries);
            drawButtons(window, font);
            drawHint(window, font, hintMessage);
            drawLives(window, font, lives);
            drawTimer(window, font, gameClock, hintLimit - hintsUsed);
        }
        else if (gameOver)
        {
            drawGameOver(window, font);
        }
        else if (gameWon)
        {
            drawCongratulation(window, font, score, totalTime);
        }
        drawButton(window, font, "Exit", Vector2f(700, 750), Color::Red);
        window.display();
    }

    return 0;
}

void drawMainMenu(RenderWindow &window, Font &font)
{
    Text title;
    title.setFont(font);
    title.setString("Sudoku Game");
    title.setCharacterSize(48);
    title.setFillColor(Color::Black);
    title.setPosition(200, 100);
    window.draw(title);

    drawButton(window, font, "New Game", Vector2f(300, 300), Color::Cyan);
    drawButton(window, font, "Load Game", Vector2f(300, 400), Color::Cyan);
    // Removed the Exit button
    // drawButton(window, font, "Exit", Vector2f(300, 500), Color::Red);
}

void drawSudokuOptions(RenderWindow &window, Font &font)
{
    Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(Color::Black);

    text.setString("Choose Sudoku Elements:");
    text.setPosition(50, 50);
    window.draw(text);

    drawButton(window, font, "Traditional", Vector2f(50, 100), numTypeChoice == 1 ? Color::Green : Color::White);
    drawButton(window, font, "Odd Num", Vector2f(300, 100), numTypeChoice == 2 ? Color::Green : Color::White);
    drawButton(window, font, "Even Num", Vector2f(50, 200), numTypeChoice == 3 ? Color::Green : Color::White);
    drawButton(window, font, "Alphabet", Vector2f(300, 200), numTypeChoice == 4 ? Color::Green : Color::White);

    text.setString("Choose Sudoku Version:");
    text.setPosition(50, 300);
    window.draw(text);

    drawButton(window, font, "General", Vector2f(50, 350), sudokuTypeChoice == 1 ? Color::Green : Color::White);
    drawButton(window, font, "Diagonal", Vector2f(300, 350), sudokuTypeChoice == 2 ? Color::Green : Color::White);
    drawButton(window, font, "Windoku", Vector2f(50, 450), sudokuTypeChoice == 3 ? Color::Green : Color::White);

    text.setString("Choose Difficulty Level:");
    text.setPosition(50, 550);
    window.draw(text);

    drawButton(window, font, "Easy", Vector2f(50, 600), difficultyChoice == 1 ? Color::Green : Color::White);
    drawButton(window, font, "Medium", Vector2f(300, 600), difficultyChoice == 2 ? Color::Green : Color::White);
    drawButton(window, font, "Hard", Vector2f(50, 700), difficultyChoice == 3 ? Color::Green : Color::White);
    drawButton(window, font, "Expert", Vector2f(300, 700), difficultyChoice == 4 ? Color::Green : Color::White);
}

void drawGrid(RenderWindow &window, const vector<vector<int>> &grid, const vector<vector<int>> &wrongEntries)
{
    Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        cerr << "Error loading font" << endl;
        return;
    }

    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            RectangleShape cell(Vector2f(60, 60));
            cell.setPosition(col * 60 + 100, row * 60 + 50);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(Color::Black);

            // Highlight Windoku subgrids
            if ((row >= 1 && row <= 3 && col >= 1 && col <= 3) ||
                (row >= 1 && row <= 3 && col >= 5 && col <= 7) ||
                (row >= 5 && row <= 7 && col >= 1 && col <= 3) ||
                (row >= 5 && row <= 7 && col >= 5 && col <= 7))
            {
                cell.setFillColor(Color(200, 200, 200));
            }

            if (row == selectedRow && col == selectedCol)
            {
                cell.setFillColor(Color(150, 150, 150));
            }
            window.draw(cell);

            if (grid[row][col] != 0 || wrongEntries[row][col] != 0)
            {
                Text text;
                text.setFont(font);
                if (numTypeChoice == 4)
                {
                    char ch = (grid[row][col] != 0) ? grid[row][col] + 'A' - 1 : wrongEntries[row][col] + 'A' - 1;
                    text.setString(string(1, ch));
                }
                else
                {
                    text.setString(to_string((grid[row][col] != 0) ? grid[row][col] : wrongEntries[row][col]));
                }
                text.setCharacterSize(24);
                if (highlightMistakes && !isCorrect(row + 1, col + 1, grid[row][col]))
                {
                    text.setFillColor(Color::Red);
                }
                else if (wrongEntries[row][col] != 0)
                {
                    text.setFillColor(Color::Red);
                }
                else
                {
                    text.setFillColor(Color::Black);
                }
                text.setPosition(col * 60 + 120, row * 60 + 60);
                window.draw(text);
            }
        }
    }

    RectangleShape thickLine(Vector2f(540, 3));
    thickLine.setFillColor(Color::Black);
    for (int i = 1; i < 3; i++)
    {
        thickLine.setPosition(100, i * 180 + 50);
        window.draw(thickLine);
    }

    thickLine.setSize(Vector2f(3, 540));
    for (int i = 1; i < 3; i++)
    {
        thickLine.setPosition(i * 180 + 100, 50);
        window.draw(thickLine);
    }
}

void drawButtons(RenderWindow &window, Font &font)
{
    drawButton(window, font, "Save", Vector2f(50, 650), Color::Cyan);
    drawButton(window, font, "Load", Vector2f(200, 650), Color::Cyan);
    drawButton(window, font, "Hint", Vector2f(350, 650), Color::Cyan);
    drawButton(window, font, "Solve", Vector2f(500, 650), Color::Cyan);
    drawButton(window, font, "Restart", Vector2f(50, 750), Color::Magenta);
    drawButton(window, font, "Main Menu", Vector2f(200, 750), Color::Magenta);
    drawButton(window, font, "Exit", Vector2f(700, 750), Color::Green);
}

void saveSudoku(vector<vector<int>> &grid, int lives, Time elapsedTime)
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
    outFile << "Time: " << elapsedTime.asSeconds() << endl;

    outFile.close();
    cout << "Sudoku grid, lives, and time saved successfully." << endl;

    // Save the solution grid
    ofstream solutionFile("solution.txt");
    if (!solutionFile)
    {
        cerr << "Error opening solution file!" << endl;
        return;
    }

    for (const auto &row : solvedGrid)
    {
        for (int num : row)
        {
            solutionFile << num << " ";
        }
        solutionFile << endl;
    }

    solutionFile.close();
    cout << "Solution grid saved successfully." << endl;
}

void loadSudoku(vector<vector<int>> &grid, int &lives, Time &elapsedTime)
{
    ifstream inFile("saved.txt");
    if (!inFile)
    {
        cerr << "Error opening file!" << endl;
        return;
    }

    for (auto &row : grid)
    {
        for (int &num : row)
        {
            inFile >> num;
        }
    }

    string livesLabel;
    inFile >> livesLabel >> lives;

    string timeLabel;
    float seconds;
    inFile >> timeLabel >> seconds;
    elapsedTime = sf::seconds(seconds);

    inFile.close();
    cout << "Sudoku grid, lives, and time loaded successfully." << endl;

    // Load the solution grid
    ifstream solutionFile("solution.txt");
    if (!solutionFile)
    {
        cerr << "Error opening solution file!" << endl;
        return;
    }

    for (auto &row : solvedGrid)
    {
        for (int &num : row)
        {
            solutionFile >> num;
        }
    }

    solutionFile.close();
    cout << "Solution grid loaded successfully." << endl;
}

int randomInRange(int min, int max)
{
    return min + rand() % (max - min + 1);
}

void shuffleNumbers(vector<int> &NUMBERS)
{
    int size = NUMBERS.size();
    for (int i = size - 1; i > 0; --i)
    {
        int j = randomInRange(0, i);
        swap(NUMBERS[i], NUMBERS[j]);
    }
}

bool isValidWindoku(vector<vector<int>> &grid, int row, int col, int num)
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (grid[row][i] == num || grid[i][col] == num)
        {
            return false;
        }
    }

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
                                grid[r][col] = -1;
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
                                    grid[r][col] = -1;
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

void applyAdvancedTechniques(vector<vector<int>> &grid)
{
    for (int num = 1; num <= 9; num++)
    {
        applyXWing(grid, num);
        applySwordfish(grid, num);
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

bool fillWindokuGrid(vector<vector<int>> &grid)
{
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
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
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
            cout << "   You are out of lives  " << endl;
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
    int choosedRow = -1, choosedCol = -1;
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
                    choosedRow = row;
                    choosedCol = col;
                    bestChoices = validNumbers;
                }
            }
        }
    }

    if (choosedRow != -1 && choosedCol != -1)
    {
        cout << "Greedy Hint: Cell (" << choosedRow + 1 << ", " << choosedCol + 1 << ") has "
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
    cout << "Enter H or h for a greedy hint." << endl;
    cout << "Enter S or s to save the current game." << endl;

    while (1)
    {
        cout << "Enter element (row, col, element): ";
        string input;
        cin >> input;

        if (input == "q" || input == "Q")
        {
            cout << "You have quit the game." << endl;
            return false;
        }
        if (input == "h" or input == "H")
        {
            provideHint(grid);
            continue;
        }

        if (input == "s" or input == "S")
        {
            saveSudoku(grid, lives, seconds(0));
            cout << "The current sudoku grid, live count, and time have been saved in file saved.txt." << endl;
            return false;
        }

        row = stoi(input);
        cin >> col >> element;

        if (row == -1 && col == -1 && element == -1)
        {
            return false;
        }

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
            cout << "   You are out of lives    " << endl;
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

vector<vector<int>> generateSudoku()
{
    vector<vector<int>> grid(GRID_SIZE, vector<int>(GRID_SIZE, 0));
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

    int holes;
    switch (difficultyChoice)
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

int calculateScore(int lives, int hintsUsed, Time totalTime, int difficultyChoice)
{
    int baseScore;
    switch (difficultyChoice)
    {
    case 1:
        baseScore = 1500; // Easy
        break;
    case 2:
        baseScore = 2000; // Medium
        break;
    case 3:
        baseScore = 2500; // Hard
        break;
    case 4:
        baseScore = 3000; // Expert
        break;
    default:
        baseScore = 1500; // Default to Easy
    }
    int timePenalty = static_cast<int>(totalTime.asSeconds()) / 60;
    int hintPenalty = hintsUsed * 25;
    int livesBonus = lives * 100;
    return baseScore - timePenalty - hintPenalty + livesBonus;
}
