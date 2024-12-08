#ifndef GAME_H
#define GAME_H

#include <array>
#include <optional>

class TicTacToe {
public:
    TicTacToe();

    void reset();                               // Resets the game
    void printBoard() const;                    // Prints the board
    bool isValidMove(int position) const;       // Checks if a move is valid
    bool makeMove(int position);                // Makes a move
    void switchPlayer();                        // Switches to the next player
    std::optional<char> checkWinner() const;    // Checks for a winner or draw
    char getCurrentPlayer() const;              // Returns the current player
    const std::array<char, 9>& getBoard() const; // Returns the current board state

private:
    std::array<char, 9> board;                  // 1D array for the board
    char currentPlayer;                         // Current player ('X' or 'O')
    int movesMade;                              // Number of moves made
};

#endif
