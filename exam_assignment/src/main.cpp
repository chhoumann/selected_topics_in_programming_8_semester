#include <iostream>
#include <random>

const double reactionRate = 0.1;

std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0.0, 1.0);

double calculateReactionTime() {
    // Draw a random number and calculate the time until the next reaction.
    double random = distribution(generator);
    return -log(random) / reactionRate;
}

void performReaction(int& moleculeCount) {
    // This system only has one reaction, which converts two molecules into one.
    // This function therefore decreases the molecule count by one.
    if (moleculeCount >= 2) {
        moleculeCount--;
    }
}

int main(int argc, char const *argv[])
{
    int moleculeCount = 10;

    double time = 0.0;

    while (time < 1000.0) {
        double reactionTime = calculateReactionTime();
        time += reactionTime;

        performReaction(moleculeCount);

        std::cout << "Time: " << time << " Molecule count: " << moleculeCount << std::endl;
    }

    return 0;
}
