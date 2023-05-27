#include "types.cpp"
#include "circadian_oscillator.cpp"
#include <iostream>
#include <graphviz/gvc.h> // ensure that graphviz is installed - sudo apt install libgraphviz-dev

// Use operator overloading to support the reaction rule typesetting directly in C++ code.
std::ostream& operator<<(std::ostream& os, const Reaction& reaction) {
    os << reaction.reactants[0].getName();
    for (int i = 1; i < reaction.reactants.size(); i++) {
        os << " + " << reaction.reactants[i].getName();
    }

    os << " → " << reaction.products[0].getName();
    for (int i = 1; i < reaction.products.size(); i++) {
        os << " + " << reaction.products[i].getName();
    }

    os << " (rate: " << reaction.delay << ")";

    return os;
}

void generate_graph(std::vector<Reaction>& reactions) {
    GVC_t *gvc;
    Agraph_t *g;

    gvc = gvContext();
    g = agopen("Reactions", Agdirected, NULL);

    for (auto& reaction : reactions) {
        std::string reaction_str = "";

        for (auto& reactant : reaction.reactants) {
            reaction_str += reactant.getName();
            reaction_str += " + ";
        }
        reaction_str = reaction_str.substr(0, reaction_str.size() - 3);  // remove last " + "
        
        reaction_str += " -> ";

        for (auto& product : reaction.products) {
            reaction_str += product.getName();
            reaction_str += " + ";
        }
        reaction_str = reaction_str.substr(0, reaction_str.size() - 3);  // remove last " + "

        char *mutable_str = strdup(reaction_str.c_str());
        agnode(g, mutable_str, TRUE);
        free(mutable_str);  // don't forget to free the memory
    }

    gvLayout(gvc, g, "dot");
    
    FILE *fp = fopen("graph.png", "w");
    if (fp != NULL) {
        gvRender(gvc, g, "png", fp);
        fclose(fp);
    }
    else {
        std::cerr << "Failed to open file for writing\n";
    }
    
    gvFreeLayout(gvc, g);

    agclose(g);
    gvFreeContext(gvc);
}


int main(int argc, char const *argv[])
{
    auto v = circadian_oscillator();
    auto reactions = v.getReactions();

    // Print all reactions in v
    for (const auto& reaction : reactions) {
        std::cout << reaction << std::endl;
    }

    // Generate a graph of the reactions
    generate_graph(reactions);

    // auto sim = StochasticSimulator(v);

    return 0;
}
