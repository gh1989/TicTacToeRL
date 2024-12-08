#ifndef NETWORK_H
#define NETWORK_H

#include <torch/torch.h>

struct TicTacToeNet : torch::nn::Module {
    explicit TicTacToeNet(torch::Device device) : device(device) {
        // Define layers
        fc1 = register_module("fc1", torch::nn::Linear(9, 64));
        fc2 = register_module("fc2", torch::nn::Linear(64, 64));
        policyHead = register_module("policyHead", torch::nn::Linear(64, 9)); // 9 actions
        valueHead = register_module("valueHead", torch::nn::Linear(64, 1));  // 1 scalar value

        // Move the model to the specified device
        this->to(device);
    }

    std::tuple<torch::Tensor, torch::Tensor> forward(torch::Tensor x) {
        x = x.to(device); // Ensure input tensor is on the correct device
        x = torch::relu(fc1->forward(x));
        x = torch::relu(fc2->forward(x));
        auto policy = torch::softmax(policyHead->forward(x), 1);
        auto value = torch::tanh(valueHead->forward(x));
        return std::make_tuple(policy, value);
    }

private:
    torch::Device device; // Store the device
    torch::nn::Linear fc1{nullptr}, fc2{nullptr}, policyHead{nullptr}, valueHead{nullptr};
};

#endif