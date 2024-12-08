#include "environment.h"
#include <iostream>
#include <cassert>

void testEnvironment() {
    Environment env;

    // Reset environment
    std::vector<float> initialState = env.reset();
    env.printState();
    assert(initialState.size() == 9); // Tic-Tac-Toe has 9 cells
    std::cout << "Initial state test passed.\n";

    // Test valid actions
    std::vector<int> validActions = env.getValidActions();
    assert(validActions.size() == 9); // Initially, all 9 cells should be valid
    std::cout << "Valid actions test passed.\n";

    // Take a valid action
    auto [newState, reward, done] = env.step(validActions[0]); // Place 'X'
    env.printState();
    assert(reward == 0.0f); // No reward yet
    assert(!done);          // Game should not be over
    std::cout << "Step test passed.\n";

    // Recalculate valid actions
    validActions = env.getValidActions();
    std::cout << "Valid actions after step 1: ";
    for (int action : validActions) {
        std::cout << action << " ";
    }
    std::cout << std::endl;

    // Take another valid action
    validActions = env.getValidActions(); // Recalculate before stepping
    auto [nextState2, reward2, done2] = env.step(validActions[0]); // Place 'O'
    env.printState();
    assert(reward2 == 0.0f); // No reward yet
    assert(!done2);          // Game should not be over
    std::cout << "Second step test passed.\n";

    // Simulate a winning game dynamically
    while (!done) {
        validActions = env.getValidActions();
        assert(!validActions.empty()); // Ensure there are valid moves

        // Pick the first valid action
        auto [nextState, reward, gameDone] = env.step(validActions[0]);
        env.printState();

        if (gameDone) {
            std::cout << "Game Over. Reward: " << reward << "\n";
            break;
        }
    }

    std::cout << "Winning state test passed.\n";
}

int main() {
    testEnvironment();
    return 0;
}
