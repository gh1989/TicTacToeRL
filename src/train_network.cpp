#include "trainer.h"
#include "environment.h"
#include "agent.h"
#include <torch/torch.h>
#include <iostream>
#include <fstream>

int _main() {
    // Device configuration
    torch::Device device(torch::kCUDA);

    // Training configuration
    int numEpisodes = 100000;       // Number of training episodes
    float initialEpsilon = 1e-4;  // Initial exploration rate
    float epsilonDecay = 0.99;   // Decay rate for exploration
    float minEpsilon = 0.0001;      // Minimum exploration rate
    float learningRate = 1e-5;   // Learning rate for optimizer
    std::string modelPath = "tictactoe_model.pt";

    // Create environment and agent
    Environment env;
    Agent agent(device, learningRate);

    // Configure epsilon decay
    agent.setEpsilon(initialEpsilon, epsilonDecay, minEpsilon);

    // Load existing model if available
    std::ifstream modelFile(modelPath);
    if (modelFile.good()) {
        agent.loadModel(modelPath);
    }

    // Create trainer
    Trainer trainer(env, agent);

    // Train the agent
    std::cout << "Starting training for " << numEpisodes << " episodes...\n";
    trainer.train(numEpisodes);

    // Save the trained model
    agent.saveModel(modelPath);

    std::cout << "Training complete. Model saved to " << modelPath << "\n";
    return 0;
}