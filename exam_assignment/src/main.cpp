#include "types.cpp"
#include "circadian_oscillator.cpp"
#include <iostream>
#include <graphviz/gvc.h> // ensure that graphviz is installed - sudo apt install libgraphviz-dev
#include <cstring>

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
    Agnode_t *n, *n_delay, *n_product;
    Agedge_t *e;

    gvc = gvContext();
    g = agopen("Reactions", Agdirected, NULL);

    int delayCount = 0;
    for (auto& reaction : reactions) {
        for (auto& reactant : reaction.reactants) {
            std::string reactant_name = reactant.getName();
            char *mutable_str = strdup(reactant_name.c_str());
            n = agnode(g, mutable_str, TRUE);

            agsafeset(n, "shape", "box", "");
            agsafeset(n, "style", "filled", "");
            agsafeset(n, "fillcolor", "cyan", "");

            // delay node
            std::string delay_name = "delay_" + std::to_string(delayCount++);
            char *delay_str = strdup(delay_name.c_str());
            n_delay = agnode(g, delay_str, TRUE);
            
            std::string delay_string = std::to_string(reaction.delay);
            char* delay_label = new char[delay_string.length() + 1];
            std::strcpy(delay_label, delay_string.c_str());

            agsafeset(n_delay, "label", delay_label, "");

            // Don't forget to free the memory later
            delete[] delay_label;

            agsafeset(n_delay, "shape", "oval", "");
            agsafeset(n_delay, "style", "filled", "");
            agsafeset(n_delay, "fillcolor", "yellow", "");

            e = agedge(g, n, n_delay, NULL, TRUE);

            free(mutable_str);  // don't forget to free the memory
            free(delay_str);
        }

        for (auto& product : reaction.products) {
            std::string product_name = product.getName();
            char *mutable_str = strdup(product_name.c_str());
            n_product = agnode(g, mutable_str, TRUE);

            agsafeset(n_product, "shape", "box", "");
            agsafeset(n_product, "style", "filled", "");
            agsafeset(n_product, "fillcolor", "cyan", "");

            e = agedge(g, n_delay, n_product, NULL, TRUE);

            free(mutable_str);  // don't forget to free the memory
        }
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
