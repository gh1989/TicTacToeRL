#include "trainer.h"
#include "environment.h"
#include "agent.h"
#include <cassert>

void testTrainer() {
    torch::Device device(torch::kCPU);
    Environment env;
    Agent agent(device);

    Trainer trainer(env, agent);

    // Train for 1 episode
    trainer.train(1);
    std::cout << "Trainer test passed.\n";
}

int main() {
    testTrainer();
    return 0;
}
