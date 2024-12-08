#ifndef AGENT_H
#define AGENT_H

#include "network.h"
#include <torch/torch.h>
#include <vector>
#include <memory>

struct Transition {
    std::vector<float> state;
    int action;
    float reward;
};

class Agent {
public:
    Agent(torch::Device device, float learningRate = 1e-3);

    // Select an action based on the state and valid actions
    int chooseAction(const std::vector<float>& state, const std::vector<int>& validActions, bool explore = true);

    // Update policy with collected transitions
    void updatePolicy(const std::vector<Transition>& transitions);

    // Set exploration parameters
    void setEpsilon(float initialEpsilon, float decay, float minValue);

    // Save and load model
    void saveModel(const std::string& path);
    void loadModel(const std::string& path);

private:
    std::shared_ptr<TicTacToeNet> policyNetwork; // Neural network
    std::unique_ptr<torch::optim::Adam> optimizer; // Optimizer
    torch::Device device;

    // Exploration parameters
    float epsilon = 1.0;         // Current exploration rate
    float epsilonDecay = 0.99;   // Decay factor per episode
    float minEpsilon = 0.1;      // Minimum exploration rate

    // Helper function for random chance
    bool randomChance(float probability);
};

#endif
