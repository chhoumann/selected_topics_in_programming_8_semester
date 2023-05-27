#include "types.cpp"
#include "circadian_oscillator.cpp"

int main(int argc, char const *argv[])
{
    auto v = circadian_oscillator();
    auto reactions = v.getReactions();

    // auto sim = StochasticSimulator(v);

    return 0;
}
