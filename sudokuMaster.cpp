#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <random>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

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
void saveSudoku(vector<vector<int>> &grid, int lives, sf::Time elapsedTime);
void loadSudoku(vector<vector<int>> &grid, int &lives, sf::Time &elapsedTime);
void drawGrid(sf::RenderWindow &window, const vector<vector<int>> &grid);
void drawButtons(sf::RenderWindow &window, sf::Font &font);
void drawMainMenu(sf::RenderWindow &window, sf::Font &font);
void drawSudokuOptions(sf::RenderWindow &window, sf::Font &font);
int calculateScore(int difficulty, int lives, int hintsUsed, sf::Time elapsedTime);
void drawScore(sf::RenderWindow &window, sf::Font &font, int score);

void drawButton(sf::RenderWindow &window, sf::Font &font, const string &textString, sf::Vector2f position, sf::Color fillColor)
{
    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setPosition(position);
    button.setFillColor(fillColor);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::Black);
    window.draw(button);

    sf::Text text;
    text.setFont(font);
    text.setString(textString);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(position.x + 50, position.y + 10);
    window.draw(text);
}

void drawHint(sf::RenderWindow &window, sf::Font &font, const string &hint)
{
    sf::Text hintText;
    hintText.setFont(font);
    hintText.setString(hint);
    hintText.setCharacterSize(24);
    hintText.setFillColor(sf::Color::Black);
    hintText.setPosition(50, 700);
    window.draw(hintText);
}

void drawLives(sf::RenderWindow &window, sf::Font &font, int lives)
{
    // Create a string with lives and a heart emoji
    std::wstring livesString = L"Lives: " + std::wstring(lives, L'\u2764'); // L'\u2764' represents the heart emoji

    // Create text using the font and string
    sf::Text livesText(livesString, font, 50); // Use an appropriate font size

    // Set text color (you can adjust the color as needed)
    livesText.setFillColor(sf::Color::Red);

    // Draw the text on the window
    window.draw(livesText);
}

void drawGameOver(sf::RenderWindow &window, sf::Font &font)
{
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("   \tYou are out of lives\n******** GAME OVER ********");
    gameOverText.setCharacterSize(36);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(200, 300);
    window.draw(gameOverText);
}

void drawCongratulation(sf::RenderWindow &window, sf::Font &font)
{
    sf::Text congratsText;
    congratsText.setFont(font);
    congratsText.setString("******* Congratulations! *******\n You have WON the game! ");
    congratsText.setCharacterSize(36);
    congratsText.setFillColor(sf::Color::Green);
    congratsText.setPosition(100, 300);
    window.draw(congratsText);

    drawButton(window, font, "Restart", sf::Vector2f(300, 400), sf::Color::Cyan);
    drawButton(window, font, "Exit", sf::Vector2f(300, 500), sf::Color::Red);
}

void drawTimer(sf::RenderWindow &window, sf::Font &font, sf::Time elapsedTime)
{
    int minutes = static_cast<int>(elapsedTime.asSeconds()) / 60;
    int seconds = static_cast<int>(elapsedTime.asSeconds()) % 60;

    sf::Text timerText;
    timerText.setFont(font);
    timerText.setString("Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds));
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(700, 50);
    window.draw(timerText);
}

int calculateScore(int difficulty, int lives, int hintsUsed, sf::Time elapsedTime)
{
    int baseScore;
    switch (difficulty)
    {
    case 1:
        baseScore = 1000;
        break; // Easy
    case 2:
        baseScore = 2000;
        break; // Medium
    case 3:
        baseScore = 3000;
        break; // Hard
    case 4:
        baseScore = 4000;
        break; // Expert
    default:
        baseScore = 1000;
        break; // Default to Easy
    }

    int timePenalty = static_cast<int>(elapsedTime.asSeconds()) / 10;
    int hintPenalty = hintsUsed * 50;
    int livesBonus = lives * 100;

    return baseScore - timePenalty - hintPenalty + livesBonus;
}

void drawScore(sf::RenderWindow &window, sf::Font &font, int score)
{
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(700, 100);
    window.draw(scoreText);
}

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(900, 900), "Sudoku Main Menu");
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    bool inMainMenu = true;
    bool inGame = false;
    bool inOptions = false;
    bool gameOver = false;
    bool gameWon = false;
    vector<vector<int>> grid(GRID_SIZE, vector<int>(GRID_SIZE, 0));
    string hintMessage = "";
    int lives = LIVES;
    int hintsUsed = 0;
    sf::Clock gameClock;
    sf::Time savedTime;
    int score = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    if (inMainMenu)
                    {
                        if (x > 300 && x < 500)
                        {
                            if (y > 300 && y < 350)
                            {
                                inMainMenu = false;
                                inOptions = true;
                            }
                            else if (y > 400 && y < 450)
                            {
                                loadSudoku(grid, lives, savedTime);
                                inMainMenu = false;
                                inGame = true;
                                gameClock.restart();
                                savedTime += gameClock.getElapsedTime(); // Add the saved time
                            }
                            else if (y > 500 && y < 550)
                            {
                                window.close();
                            }
                        }
                    }
                    else if (inOptions)
                    {
                        if (x > 50 && x < 250)
                        {
                            if (y > 100 && y < 150)
                            {
                                numTypeChoice = 1;
                            }
                            else if (y > 200 && y < 250)
                            {
                                numTypeChoice = 3;
                            }
                            else if (y > 350 && y < 400)
                            {
                                sudokuTypeChoice = 1;
                            }
                            else if (y > 450 && y < 500)
                            {
                                sudokuTypeChoice = 3;
                            }
                            else if (y > 600 && y < 650)
                            {
                                difficultyChoice = 1;
                            }
                            else if (y > 700 && y < 750)
                            {
                                difficultyChoice = 3;
                            }
                        }
                        else if (x > 300 && x < 500)
                        {
                            if (y > 100 && y < 150)
                            {
                                numTypeChoice = 2;
                            }
                            else if (y > 200 && y < 250)
                            {
                                numTypeChoice = 4;
                            }
                            else if (y > 350 && y < 400)
                            {
                                sudokuTypeChoice = 2;
                            }
                            else if (y > 600 && y < 650)
                            {
                                difficultyChoice = 2;
                            }
                            else if (y > 700 && y < 750)
                            {
                                difficultyChoice = 4;
                            }
                        }
                        if (numTypeChoice != 0 && sudokuTypeChoice != 0 && difficultyChoice != 0)
                        {
                            chooseSudokuNumType();
                            shuffleNumbers(NUMBERS);
                            grid = generateSudoku();
                            inOptions = false;
                            inGame = true;
                            gameClock.restart(); // Restart the timer when the game starts
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
                            {
                                saveSudoku(grid, lives, gameClock.getElapsedTime());
                            }
                            else if (x > 200 && x < 300)
                            {
                                loadSudoku(grid, lives, savedTime);
                                gameClock.restart();
                                savedTime += gameClock.getElapsedTime(); // Add the saved time
                            }
                            else if (x > 350 && x < 450)
                            {
                                if (selectedRow != -1 && selectedCol != -1)
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
                                    hintMessage = "Hint: Select a cell first.";
                                }
                            }
                            else if (x > 500 && x < 600)
                            {
                                grid = solvedGrid;
                            }
                        }
                        else if (y > 750 && y < 800)
                        {
                            if (x > 50 && x < 150)
                            {
                                grid = generateSudoku();
                                gameClock.restart(); // Restart the timer when the game restarts
                            }
                            else if (x > 200 && x < 300)
                            {
                                inGame = false;
                                inMainMenu = true;
                            }
                            else if (x > 350 && x < 450)
                            {
                                window.close();
                            }
                        }
                    }
                    else if (gameOver)
                    {
                        if (x > 300 && x < 500)
                        {
                            if (y > 400 && y < 450)
                            {
                                inMainMenu = true;
                                gameOver = false;
                            }
                            else if (y > 500 && y < 550)
                            {
                                window.close();
                            }
                        }
                    }
                    else if (gameWon)
                    {
                        if (x > 300 && x < 500)
                        {
                            if (y > 400 && y < 450)
                            {
                                grid = generateSudoku();
                                gameWon = false;
                                inGame = true;
                                gameClock.restart(); // Restart the timer when the game restarts
                            }
                            else if (y > 500 && y < 550)
                            {
                                window.close();
                            }
                        }
                    }
                    // Check if the Exit button is clicked
                    if (x > 700 && x < 900 && y > 750 && y < 800)
                    {
                        window.close();
                    }
                }
            }
            else if (inGame && event.type == sf::Event::KeyPressed)
            {
                if (selectedRow != -1 && selectedCol != -1)
                {
                    if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9)
                    {
                        int num = event.key.code - sf::Keyboard::Num1 + 1;
                        if (isCorrect(selectedRow + 1, selectedCol + 1, num))
                        {
                            grid[selectedRow][selectedCol] = num;
                            if (isFilled(grid))
                            {
                                std::cout << "******* Congratulations! You have WON the game! *******" << std::endl;
                                inGame = false;
                                gameWon = true;
                                score = calculateScore(difficultyChoice, lives, hintsUsed, gameClock.getElapsedTime() + savedTime);
                            }
                        }
                        else
                        {
                            lives--;
                            if (lives == 0)
                            {
                                std::cout << "    You are out of lives    " << std::endl;
                                std::cout << "******** GAME OVER ********" << std::endl;
                                inGame = false;
                                gameOver = true;
                                score = calculateScore(difficultyChoice, lives, hintsUsed, gameClock.getElapsedTime() + savedTime);
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);
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
            drawGrid(window, grid);
            drawButtons(window, font);
            drawHint(window, font, hintMessage);
            drawLives(window, font, lives);
            drawTimer(window, font, gameClock.getElapsedTime() + savedTime); // Draw the timer
            drawScore(window, font, score);                                  // Draw the score
        }
        else if (gameOver)
        {
            drawGameOver(window, font);
            drawScore(window, font, score); // Draw the score
        }
        else if (gameWon)
        {
            drawCongratulation(window, font);
            drawScore(window, font, score); // Draw the score
        }
        drawButton(window, font, "Exit", sf::Vector2f(700, 750), sf::Color::Red); // Always show exit button
        window.display();
    }

    return 0;
}

void drawMainMenu(sf::RenderWindow &window, sf::Font &font)
{
    sf::Text title;
    title.setFont(font);
    title.setString("Sudoku Game");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::Black);
    title.setPosition(200, 100);
    window.draw(title);

    drawButton(window, font, "New Game", sf::Vector2f(300, 300), sf::Color::Green);
    drawButton(window, font, "Load Game", sf::Vector2f(300, 400), sf::Color::Blue);
}

void drawSudokuOptions(sf::RenderWindow &window, sf::Font &font)
{
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    // Draw the title for Sudoku Elements selection
    text.setString("Choose Sudoku Elements:");
    text.setPosition(50, 50);
    window.draw(text);

    // Button for selecting number type
    drawButton(window, font, "Traditional", sf::Vector2f(50, 100), numTypeChoice == 1 ? sf::Color::Green : sf::Color::White);
    drawButton(window, font, "Odd Numbers", sf::Vector2f(300, 100), numTypeChoice == 2 ? sf::Color::Green : sf::Color::White);
    drawButton(window, font, "Even Numbers", sf::Vector2f(50, 200), numTypeChoice == 3 ? sf::Color::Green : sf::Color::White);
    drawButton(window, font, "Alphabet", sf::Vector2f(300, 200), numTypeChoice == 4 ? sf::Color::Green : sf::Color::White);

    // Draw the title for Sudoku version selection
    text.setString("Choose Sudoku Version:");
    text.setPosition(50, 300);
    window.draw(text);

    // Button for selecting sudoku version
    drawButton(window, font, "General", sf::Vector2f(50, 350), sudokuTypeChoice == 1 ? sf::Color::Green : sf::Color::White);
    drawButton(window, font, "Diagonal", sf::Vector2f(300, 350), sudokuTypeChoice == 2 ? sf::Color::Green : sf::Color::White);
    drawButton(window, font, "Windoku", sf::Vector2f(50, 450), sudokuTypeChoice == 3 ? sf::Color::Green : sf::Color::White);

    // Draw the title for Difficulty Level selection
    text.setString("Choose Difficulty Level:");
    text.setPosition(50, 550);
    window.draw(text);

    // Button for selecting difficulty level
    drawButton(window, font, "Easy", sf::Vector2f(50, 600), difficultyChoice == 1 ? sf::Color::Green : sf::Color::White);
    drawButton(window, font, "Medium", sf::Vector2f(300, 600), difficultyChoice == 2 ? sf::Color::Green : sf::Color::White);
    drawButton(window, font, "Hard", sf::Vector2f(50, 700), difficultyChoice == 3 ? sf::Color::Green : sf::Color::White);
    drawButton(window, font, "Expert", sf::Vector2f(300, 700), difficultyChoice == 4 ? sf::Color::Green : sf::Color::White);
}

void drawGrid(sf::RenderWindow &window, const vector<vector<int>> &grid)
{
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Error loading font" << std::endl;
        return;
    }

    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 0; col < GRID_SIZE; col++)
        {
            sf::RectangleShape cell(sf::Vector2f(60, 60));
            cell.setPosition(col * 60 + 100, row * 60 + 50);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);

            // Highlight Windoku subgrids
            if ((row >= 1 && row <= 3 && col >= 1 && col <= 3) ||
                (row >= 1 && row <= 3 && col >= 5 && col <= 7) ||
                (row >= 5 && row <= 7 && col >= 1 && col <= 3) ||
                (row >= 5 && row <= 7 && col >= 5 && col <= 7))
            {
                cell.setFillColor(sf::Color(200, 200, 200));
            }

            if (row == selectedRow && col == selectedCol)
            {
                cell.setFillColor(sf::Color(150, 150, 150));
            }
            window.draw(cell);

            if (grid[row][col] != 0)
            {
                sf::Text text;
                text.setFont(font);
                if (numTypeChoice == 4)
                {
                    char ch = grid[row][col] + 'A' - 1;
                    text.setString(string(1, ch));
                }
                else
                {
                    text.setString(to_string(grid[row][col]));
                }
                text.setCharacterSize(24);
                if (highlightMistakes && !isCorrect(row + 1, col + 1, grid[row][col]))
                {
                    text.setFillColor(sf::Color::Red);
                }
                else
                {
                    text.setFillColor(sf::Color::Black);
                }
                text.setPosition(col * 60 + 120, row * 60 + 60);
                window.draw(text);
            }
        }
    }

    // Draw thicker lines for 3x3 subgrid boundaries
    sf::RectangleShape thickLine(sf::Vector2f(540, 3)); // Horizontal line
    thickLine.setFillColor(sf::Color::Black);
    for (int i = 1; i < 3; i++)
    {
        thickLine.setPosition(100, i * 180 + 50);
        window.draw(thickLine);
    }

    thickLine.setSize(sf::Vector2f(3, 540)); // Vertical line
    for (int i = 1; i < 3; i++)
    {
        thickLine.setPosition(i * 180 + 100, 50);
        window.draw(thickLine);
    }
}

void drawButtons(sf::RenderWindow &window, sf::Font &font)
{
    drawButton(window, font, "Save", sf::Vector2f(50, 650), sf::Color::Cyan);
    drawButton(window, font, "Load", sf::Vector2f(200, 650), sf::Color::Cyan);
    drawButton(window, font, "Hint", sf::Vector2f(350, 650), sf::Color::Cyan);
    drawButton(window, font, "Solve", sf::Vector2f(500, 650), sf::Color::Cyan);
    drawButton(window, font, "Restart", sf::Vector2f(50, 750), sf::Color::Green);
    drawButton(window, font, "Main Menu", sf::Vector2f(200, 750), sf::Color::Magenta);
    drawButton(window, font, "Exit", sf::Vector2f(700, 750), sf::Color::Green);
}

void saveSudoku(vector<vector<int>> &grid, int lives, sf::Time elapsedTime)
{
    ofstream outFile("saved.txt");
    if (!outFile)
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    for (const auto &row : grid)
    {
        for (int num : row)
        {
            outFile << num << " ";
        }
        outFile << std::endl;
    }

    outFile << "Lives: " << lives << std::endl;
    outFile << "Time: " << elapsedTime.asSeconds() << std::endl;

    outFile.close();
    std::cout << "Sudoku grid, lives, and time saved successfully." << std::endl;
}

void loadSudoku(vector<vector<int>> &grid, int &lives, sf::Time &elapsedTime)
{
    ifstream inFile("saved.txt");
    if (!inFile)
    {
        std::cerr << "Error opening file!" << std::endl;
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
    std::cout << "Sudoku grid, lives, and time loaded successfully." << std::endl;
}

// Add the rest of your functions here, keeping the logic the same as in your original code.

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

bool fillWindokuGrid(vector<vector<int>> &grid)
{
    // Don't need to use shuffle numbers here
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
    std::cout << "Lets Start the game. You have 3 lives." << std::endl;
    std::cout << "If you want to quit enter -1 -1 -1" << std::endl;
    while (1)
    {
        std::cout << "Enter element(row, col, element.): " << std::endl;
        std::cin >> row >> col >> ch;
        int element = ch - 'A' + 1;
        if (row == -1 and col == -1 and element == -1)
        {
            return false;
        }
        else if (row > 9 or row < 1 or col > 9 or col < 1)
        {
            std::cout << "Invalid row or col. Enter again." << std::endl;
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
            std::cout << "You have entered wrong element. Lives remaining: " << lives << std::endl;
        }
        if (lives == 0)
        {
            std::cout << "   You are out of lives  " << std::endl;
            std::cout << "********GAME OVER********" << std::endl;
            return false;
        }
        if (isFilled(grid))
        {
            std::cout << "*******Congratulation You Have WON the game*******" << std::endl;
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
        std::cout << "Greedy Hint: Cell (" << bestRow + 1 << ", " << bestCol + 1 << ") has "
                  << bestChoices.size() << " valid choices left. Suggested number: "
                  << bestChoices[0] << std::endl;
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
    std::cout << "Let's start the game. You have 3 lives." << std::endl;
    std::cout << "If you want to quit, enter q or Q or -1 -1 -1." << std::endl;
    std::cout << "Enter H or h for a greedy hint." << std::endl;
    std::cout << "Enter S or s to save the current game." << std::endl;

    while (1)
    {
        std::cout << "Enter element (row, col, element): ";
        string input;
        std::cin >> input;

        // Quit option
        if (input == "q" || input == "Q")
        {
            std::cout << "You have quit the game." << std::endl;
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
            saveSudoku(grid, lives, sf::seconds(0));
            std::cout << "The current sudoku grid, live count, and time have been saved in file saved.txt." << std::endl;
            return false;
        }

        // Convert the input to integer if it's not 'q' or 'Q'
        row = stoi(input);
        std::cin >> col >> element;

        // for alternative quit option
        if (row == -1 && col == -1 && element == -1)
        {
            return false;
        }

        // for alternative hint option
        if (row == 0 && col == 0 && element == 0)
        {
            provideHint(grid);
            continue;
        }

        if (row > 9 || row < 1 || col > 9 || col < 1)
        {
            std::cout << "Invalid row or col. Enter again." << std::endl;
            continue;
        }

        if (isCorrect(row, col, element))
        {
            grid[row - 1][col - 1] = element;
            printGrid(grid);
            if (isFilled(grid))
            {
                std::cout << "******* Congratulations! You have WON the game! *******" << std::endl;
                return true;
            }
            continue;
        }
        else
        {
            lives--;
            std::cout << "You have entered the wrong element. Lives remaining: " << lives << std::endl;
        }

        if (lives == 0)
        {
            std::cout << "   You are out of lives    " << std::endl;
            std::cout << "******** GAME OVER ********" << std::endl;
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
        std::cout << "\nSudoku elements set to traditional (1 to 9)." << std::endl;
        traditionalSudoku();
    }
    else if (numTypeChoice == 2)
    {
        std::cout << "\nSudoku elements set to odd numbers (1, 3,....., 15, 17)." << std::endl;
    }
    else if (numTypeChoice == 3)
    {
        std::cout << "\nSudoku elements set to even numbers (2, 4,....., 16, 18)." << std::endl;
        evenSudoku();
    }
    else if (numTypeChoice == 4)
    {
        std::cout << "\nSudoku elements set to Alphabet (A, B, C,..., H, I)." << std::endl;
        traditionalSudoku();
    }
    else
    {
        std::cout << "\nInvalid Choice. Defaulting to odds (1, 3,....., 15, 17)." << std::endl;
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
    std::cout << "+---------+---------+---------+" << std::endl;

    for (int row = 0; row < GRID_SIZE; row++)
    {
        std::cout << "|";
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0)
            {
                std::cout << " . ";
            }
            else
            {
                char ch = grid[row][col] + 'A' - 1;
                std::cout << " " << ch << " ";
            }

            if ((col + 1) % 3 == 0)
            {
                std::cout << "|";
            }
        }
        std::cout << std::endl;

        if ((row + 1) % 3 == 0 && row != GRID_SIZE - 1)
        {
            std::cout << "+---------+---------+---------+" << std::endl;
        }
    }
    std::cout << "+---------+---------+---------+" << std::endl;
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
        std::cout << "+---------+---------+---------+" << std::endl;
    }
    else
    {
        std::cout << "+------------+------------+------------+" << std::endl;
    }
    for (int row = 0; row < GRID_SIZE; row++)
    {
        std::cout << "|";
        for (int col = 0; col < GRID_SIZE; col++)
        {
            if (grid[row][col] == 0)
            {
                if (numTypeChoice == 1)
                {
                    std::cout << " . ";
                }
                else
                {
                    std::cout << " .. ";
                }
            }
            else
            {
                if (numTypeChoice == 1)
                {
                    std::cout << " " << grid[row][col] << " ";
                }
                else
                {
                    std::cout << " " << std::setw(2) << std::setfill('0') << grid[row][col] << " ";
                }
            }

            if ((col + 1) % 3 == 0)
            {
                std::cout << "|";
            }
        }
        std::cout << std::endl;

        if ((row + 1) % 3 == 0 && row != GRID_SIZE - 1)
        {
            if (numTypeChoice == 1)
            {
                std::cout << "+---------+---------+---------+" << std::endl;
            }
            else
            {
                std::cout << "+------------+------------+------------+" << std::endl;
            }
        }
    }
    if (numTypeChoice == 1)
    {
        std::cout << "+---------+---------+---------+" << std::endl;
    }
    else
    {
        std::cout << "+------------+------------+------------+" << std::endl;
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
        std::cout << "Invalid Choice. Defaulting to general sudoku." << std::endl;
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
        std::cout << "Invalid choice, defaulting to Medium.\n";
        holes = 30;
    }

    solvedGrid = grid;
    removeNumbers(grid, holes);
    return grid;
}
