#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "types.h"
#include <graphviz/gvc.h> // ensure that graphviz is installed - sudo apt install libgraphviz-dev
#include <iomanip>
#include <cmath>
#include <iostream>
#include <string_view>
#include <vector>

// Function to format delay
std::string formatDelay(double delay);

// Function to generate graph
void generate_graph(const std::vector<Reaction> &reactions, const std::string &filename);

#endif // GRAPH_GENERATOR_H
