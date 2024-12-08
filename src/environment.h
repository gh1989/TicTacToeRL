#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "game.h"
#include <vector>
#include <tuple>
#include <torch/torch.h>

class Environment {
public:
    Environment();
    std::vector<float> reset();
    std::tuple<std::vector<float>, float, bool> step(int action);
    std::vector<float> getState() const;
    std::vector<int> getValidActions() const;
    void printState() const;

private:
    TicTacToe game;  
    bool done = false; 
    bool isValidAction(int action) const;
};

#endif
