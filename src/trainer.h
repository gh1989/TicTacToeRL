#ifndef TRAINER_H
#define TRAINER_H

#include "environment.h"
#include "agent.h"

class Trainer {
public:
    Trainer(Environment& env, Agent& agent);
    void train(int episodes); 

private:
    Environment& env;
    Agent& agent;

    void generateEpisode(std::vector<std::tuple<std::vector<float>, int, float>>& episodeData); // Plays one game
};

#endif