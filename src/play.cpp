#include "agent.h"
#include "environment.h"
#include <torch/torch.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

// Function to let the human player choose an action
int humanPlayer(const std::vector<int>& validActions) {
    int action;
    std::cout << "Your move! Available positions: ";
    for (int validAction : validActions) {
        std::cout << validAction << " ";
    }
    std::cout << "\nEnter your move: ";
    std::cin >> action;

    // Validate the human's input
    while (std::find(validActions.begin(), validActions.end(), action) == validActions.end()) {
        std::cout << "Invalid move! Please choose a valid position: ";
        std::cin >> action;
    }
    return action;
}

int main() {
    // Initialize device
    torch::Device device(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);
    std::cout << "Using device: " << (torch::cuda::is_available() ? "GPU" : "CPU") << "\n";

    // Load the trained model
    std::string modelPath = "tictactoe_model.pt";
    Agent agent(device);

    std::ifstream modelFile(modelPath);
    if (modelFile.good()) {
        agent.loadModel(modelPath);
        std::cout << "Loaded trained model from " << modelPath << "\n";
    } else {
        std::cerr << "Trained model not found! Please train the agent first.\n";
        return 1;
    }

    // Initialize environment
    Environment env;
    bool done = false;
    auto state = env.reset();

    std::cout << "You are 'O'. The agent is 'X'. Let's play!\n";

    char currentPlayer = 'X'; // Start with the agent (X)

    while (!done) {
        env.printState();

        if (currentPlayer == 'X') {
            // Agent's turn
            try {
                int action = agent.chooseAction(state, env.getValidActions(), false);
                std::cout << "Agent chooses position: " << action << "\n";
                auto [nextState, reward, gameDone] = env.step(action);
                state = nextState;
                done = gameDone;

                if (done && reward > 0) {
                    std::cout << "Game Over: The agent (X) wins!\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "Error during agent's turn: " << e.what() << "\n";
                return 1;
            }
        } else {
            // Human player's turn
            int action = humanPlayer(env.getValidActions());
            try {
                auto [nextState, reward, gameDone] = env.step(action);
                state = nextState;
                done = gameDone;

                if (done && reward < 0) {
                    std::cout << "Game Over: You (O) win!\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "Error during your turn: " << e.what() << "\n";
                return 1;
            }
        }

        // Switch player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    // Show final board state
    env.printState();

    // If no winner declared and game is done, it's a draw
    if (done) {
        std::cout << "Game Over: It's a draw!\n";
    }

    return 0;
}