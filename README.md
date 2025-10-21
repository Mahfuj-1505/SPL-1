# Sudoku Royale

A feature-rich Sudoku game with multiple variants, difficulty levels, and an interactive graphical interface built with C++ and SFML.

## 📝 Description

Sudoku Royale is an advanced Sudoku game that offers multiple Sudoku variants and number types. The game features automatic puzzle generation, solution validation, hint system, save/load functionality, and a scoring system. It provides an engaging gaming experience with lives tracking, timer, and various difficulty levels.

## ✨ Features

### Game Variants
- **Traditional Sudoku**: Classic 9x9 Sudoku puzzle
- **Diagonal Sudoku**: Additional constraint on main diagonals
- **Windoku**: Extra 3x3 box constraints in specific positions

### Number Types
- **Traditional**: Standard numbers 1-9
- **Odd Numbers**: Uses odd numbers (1, 3, 5, 7, 9, 11, 13, 15, 17)
- **Even Numbers**: Uses even numbers (2, 4, 6, 8, 10, 12, 14, 16, 18)
- **Alphabet**: Letters A-I instead of numbers

### Difficulty Levels
- **Easy**: Fewer cells removed
- **Medium**: Moderate challenge
- **Expert**: Maximum difficulty with more empty cells

### Gameplay Features
- ❤️ **Lives System**: 3 lives per game
- 💡 **Hint System**: Up to 5 hints available per game
- ⏱️ **Timer**: Track your solving time
- 💾 **Save/Load**: Save progress and continue later
- 🎯 **Score Calculation**: Based on lives remaining, hints used, time taken, and difficulty
- ⚠️ **Mistake Highlighting**: Visual feedback for incorrect entries
- 🏆 **Win Screen**: Congratulations message with final score and time

## 🛠️ Technologies Used

- **Language**: C++
- **Graphics Library**: SFML (Simple and Fast Multimedia Library)
- **Standard Libraries**: 
  - `<vector>` for dynamic arrays
  - `<random>` for puzzle generation
  - `<fstream>` for save/load functionality

## 📋 Requirements

- C++ compiler (C++11 or later)
- SFML library installed
- Linux/Unix system (uses DejaVu Sans font from `/usr/share/fonts/`)

## 🚀 Installation

1. Clone the repository:
```bash
git clone https://github.com/Mahfuj-1505/SPL-1.git
cd SPL-1
```

2. Install SFML (if not already installed):
```bash
# On Ubuntu/Debian
sudo apt-get install libsfml-dev

# On Fedora
sudo dnf install SFML-devel

# On Arch Linux
sudo pacman -S sfml
```

3. Compile the program:
```bash
g++ sudokuRoyale.cpp -o sudokuRoyale -lsfml-graphics -lsfml-window -lsfml-system
```

4. Run the game:
```bash
./sudokuRoyale
```

## 🎮 How to Play

1. **Start the Game**: Launch the application to see the main menu
2. **Choose Options**: 
   - Select number type (Traditional/Odd/Even/Alphabet)
   - Select Sudoku variant (Traditional/Diagonal)
   - Select difficulty level (Easy/Medium/Expert)
3. **Play**: 
   - Click on a cell to select it
   - Enter a number (1-9) or letter (A-I) depending on your choice
   - Use hints when stuck (limited to 5)
   - Complete the puzzle before running out of lives
4. **Save/Load**: Use the save button to save your progress and load it later
5. **Win**: Fill all cells correctly to win and see your score!

## 🎯 Game Controls

- **Mouse Click**: Select cells and interact with buttons
- **Keyboard Input**: Enter numbers (1-9) or use arrow keys for navigation
- **Hint Button**: Get a valid number for the selected cell
- **Save Button**: Save current game state
- **Load Button**: Load previously saved game

## 🧩 Advanced Features

### Puzzle Generation Algorithm
- Uses backtracking algorithm to generate valid Sudoku grids
- Ensures unique solutions
- Removes appropriate number of cells based on difficulty

### Solving Techniques
The game implements advanced Sudoku solving techniques:
- **Backtracking**: Basic solving algorithm
- **X-Wing**: Advanced elimination technique
- **Swordfish**: Complex pattern recognition (in development)

### Scoring System
Your score is calculated based on:
- Lives remaining
- Number of hints used
- Total time taken
- Difficulty level chosen

## 📁 Project Structure

```
SPL-1/
├── sudokuRoyale.cpp       # Main source code
├── sudokuRoyale           # Compiled executable
├── saved.txt              # Save game data
├── solution.txt           # Solution storage
├── README.md              # Project documentation
└── SPL-1 FinalReport_3.0.pdf  # Project report
```

## 🐛 Known Issues

- X-Wing and Swordfish algorithms are still being optimized
- Font path is hardcoded for Linux systems (may need adjustment for other OS)

## 🤝 Contributing

This is an academic project (SPL-1). Contributions, issues, and feature requests are welcome!

## 👥 Author

**Mahfuj-1505**
- GitHub: [@Mahfuj-1505](https://github.com/Mahfuj-1505)

## 📄 License

This project is part of an academic Software Project Lab (SPL-1) course.

## 🎓 Academic Information

This project was developed as part of the SPL-1 (Software Project Lab - 1) course. Documentation and presentations are included in the repository.

## 🙏 Acknowledgments

- SFML Community for the excellent graphics library
- Sudoku puzzle generation and solving algorithm references
- Course instructors and peers for guidance and feedback

---

**Note**: For Windows users, you may need to update the font path in the code:
```cpp
font.loadFromFile("C:/Windows/Fonts/arial.ttf")  // Windows
```
