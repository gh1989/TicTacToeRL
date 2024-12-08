#include "game.h"
#include <iostream>
#include <vector>

// Constructor
TicTacToe::TicTacToe() {
    reset();
}

// Resets the board and game state
void TicTacToe::reset() {
    board.fill(' ');
    currentPlayer = 'X';
    movesMade = 0;
}

// Prints the current board state
void TicTacToe::printBoard() const {
    std::cout << "-------------\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "| ";
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i * 3 + j] << " | ";
        }
        std::cout << "\n-------------\n";
    }
}

// Checks if a move is valid
bool TicTacToe::isValidMove(int position) const {
    return position >= 0 && position < 9 && board[position] == ' ';
}

// Makes a move for the current player
bool TicTacToe::makeMove(int position) {
    if (!isValidMove(position)) {
        return false;
    }
    board[position] = currentPlayer;
    ++movesMade;
    return true;
}

// Switches to the next player
void TicTacToe::switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

// Checks if there is a winner or a draw
std::optional<char> TicTacToe::checkWinner() const {
    // Define winning combinations
    const std::vector<std::array<int, 3>> winningCombos = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    for (const auto& combo : winningCombos) {
        if (board[combo[0]] != ' ' &&
            board[combo[0]] == board[combo[1]] &&
            board[combo[1]] == board[combo[2]]) {
            return board[combo[0]];
        }
    }

    // No winner, check for a draw
    if (movesMade == 9) {
        return 'D'; // Draw
    }

    return std::nullopt; // Game continues
}

// Returns the current player
char TicTacToe::getCurrentPlayer() const {
    return currentPlayer;
}

// Gets the current state of the board
const std::array<char, 9>& TicTacToe::getBoard() const {
    return board;
}
