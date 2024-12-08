#include "agent.h"
#include <torch/torch.h>
#include <random>
#include <iostream>

Agent::Agent(torch::Device device, float learningRate) : device(device) {
    // Initialize the neural network
    policyNetwork = std::make_shared<TicTacToeNet>(device);
    policyNetwork->to(device);

    // Set optimizer with configurable learning rate
    optimizer = std::make_unique<torch::optim::Adam>(policyNetwork->parameters(), torch::optim::AdamOptions(learningRate));

    // Initialize exploration parameters (can be set separately via setEpsilon)
    epsilon = 1.0;
    epsilonDecay = 0.99;
    minEpsilon = 0.1;
}

void Agent::setEpsilon(float initialEpsilon, float decay, float minValue) {
    epsilon = initialEpsilon;
    epsilonDecay = decay;
    minEpsilon = minValue;
}

int Agent::chooseAction(const std::vector<float>& state, const std::vector<int>& validActions, bool explore) {
    // Convert state to tensor
    torch::Tensor stateTensor = torch::from_blob((void*)state.data(), {1, static_cast<long>(state.size())}, torch::kFloat).to(device);

    // Pass through the policy network
    auto [policy, value] = policyNetwork->forward(stateTensor);

    // Mask invalid actions
    torch::Tensor validMask = torch::zeros_like(policy);
    for (int action : validActions) {
        validMask[0][action] = 1.0;
    }
    policy = policy * validMask;

    // Exploration (epsilon-greedy)
    if (explore && randomChance(epsilon)) {
        // Decay epsilon
        epsilon = std::max(epsilon * epsilonDecay, minEpsilon);

        // Choose a random valid action
        return validActions[std::rand() % validActions.size()];
    }

    // Exploitation: Choose the action with the highest probability
    return torch::argmax(policy).item<int>();
}

void Agent::updatePolicy(const std::vector<Transition>& transitions) {
    torch::Tensor totalLoss = torch::zeros({1}).to(device);

    for (const auto& transition : transitions) {
        torch::Tensor stateTensor = torch::from_blob((void*)transition.state.data(), {1, static_cast<long>(transition.state.size())}, torch::kFloat).to(device);
        torch::Tensor actionTensor = torch::tensor({transition.action}, torch::kLong).to(device);
        torch::Tensor rewardTensor = torch::tensor({transition.reward}, torch::kFloat).to(device);

        // Forward pass
        auto [policy, value] = policyNetwork->forward(stateTensor);

        // Compute loss
        int actionIndex = actionTensor.item<int>();
        torch::Tensor policyLoss = -torch::log(policy[0][actionIndex]) * rewardTensor;
        torch::Tensor valueLoss = torch::mse_loss(value, rewardTensor);
        totalLoss += policyLoss + valueLoss;
    }

    optimizer->zero_grad();
    totalLoss.backward();
    optimizer->step();
}

bool Agent::randomChance(float probability) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng) < probability;
}

void Agent::loadModel(const std::string& path) {
    torch::load(policyNetwork, path);
    policyNetwork->to(device);
    std::cout << "Model loaded from " << path << "\n";
}

void Agent::saveModel(const std::string& path) {
    torch::save(policyNetwork, path);
    policyNetwork->to(device);
    std::cout << "Model saved to " << path << "\n";
}