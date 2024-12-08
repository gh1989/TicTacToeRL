#include "trainer.h"
#include <iostream>

// Constructor
Trainer::Trainer(Environment& env, Agent& agent) : env(env), agent(agent) {}

void Trainer::train(int episodes) {
    int wins = 0, losses = 0, draws = 0;

    for (int episode = 1; episode <= episodes; ++episode) {
        // Reset environment and play the game
        std::vector<float> state = env.reset();
        bool done = false;
        float totalReward = 0.0f;

        while (!done) {
            int action = agent.chooseAction(state, env.getValidActions(), true);
            auto [nextState, reward, gameDone] = env.step(action);
            state = nextState;
            done = gameDone;
            totalReward += reward;
        }

        // Update statistics based on total reward
        if (totalReward > 0) wins++;
        else if (totalReward < 0) losses++;
        else draws++;

        // Log every 100 episodes
        if (episode % 100 == 0) {
            std::cout << "Episodes: " << episode
                      << ", Wins: " << wins
                      << ", Losses: " << losses
                      << ", Draws: " << draws << "\n";
        }
    }

    // Final results
    std::cout << "Final Results -> Wins: " << wins
              << ", Losses: " << losses
              << ", Draws: " << draws << "\n";
}