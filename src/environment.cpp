#include "environment.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

Environment::Environment() : game() {
    reset();
}

std::vector<float> Environment::reset() {
    game.reset();
    done = false;
    return getState();
}

std::tuple<std::vector<float>, float, bool> Environment::step(int action) {
    if (done) {
        throw std::runtime_error("Cannot take actions in a finished game. Call reset() to start a new game.");
    }

    if (!isValidAction(action)) {
        throw std::invalid_argument("Invalid action.");
    }

    // Apply the action
    bool moveSuccessful = game.makeMove(action);
    if (!moveSuccessful) {
        throw std::runtime_error("Failed to apply the action.");
    }

    // Check for winner or draw
    auto winner = game.checkWinner();
    float reward = 0.0;

    if (winner) {
        done = true;
        if (*winner == 'X') {
            reward = game.getCurrentPlayer() == 'X' ? 1.0f : -1.0f;
        } else if (*winner == 'O') {
            reward = game.getCurrentPlayer() == 'O' ? 1.0f : -1.0f;
        } else if (*winner == 'D') {
            reward = 0.0f; // Draw
        }
    } else {
        // Switch player for next turn
        game.switchPlayer();
    }

    return {getState(), reward, done};
}

std::vector<float> Environment::getState() const {
    std::vector<float> state(9);
    const auto& board = game.getBoard();

    for (size_t i = 0; i < board.size(); ++i) {
        if (board[i] == 'X') {
            state[i] = 1.0f; // 'X' is represented as 1
        } else if (board[i] == 'O') {
            state[i] = -1.0f; // 'O' is represented as -1
        } else {
            state[i] = 0.0f; // Empty spaces are 0
        }
    }

    return state;
}

std::vector<int> Environment::getValidActions() const {
    std::vector<int> validActions;
    const auto& board = game.getBoard();

    for (size_t i = 0; i < board.size(); ++i) {
        if (board[i] == ' ') {
            validActions.push_back(static_cast<int>(i));
        }
    }

    return validActions;
}

bool Environment::isValidAction(int action) const {
    const auto& validActions = getValidActions();
    return std::find(validActions.begin(), validActions.end(), action) != validActions.end();
}

void Environment::printState() const {
    game.printBoard();
}
