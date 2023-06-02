#include "graph_generator.h"

std::string formatDelay(double delay)
{
    if (delay == static_cast<int>(delay))
    {
        return std::to_string(static_cast<int>(delay));
    }
    else
    {
        double decimalPart = delay - static_cast<int>(delay);
        int precision = 0;
        while (std::abs(decimalPart - static_cast<int>(decimalPart)) > 1e-10 && precision < 10)
        {
            decimalPart *= 10;
            precision++;
        }
        
        std::stringstream stream;
        stream << std::setprecision(precision) << std::fixed << delay;
        return stream.str();
    }
}

void generate_graph(const std::vector<Reaction> &reactions, const std::string &filename)
{
    auto gvc = std::unique_ptr<GVC_t, decltype(&gvFreeContext)>(gvContext(), gvFreeContext);
    auto g = std::unique_ptr<Agraph_t, decltype(&agclose)>(agopen(const_cast<char *>("Reactions"), Agdirected, nullptr), agclose);

    int delayCount = 0;
    for (const auto &reaction : reactions)
    {
        std::stringstream ss;
        ss << "delay_" << delayCount++;
        auto delay_name = ss.str();
        auto n_delay = agnode(g.get(), const_cast<char *>(delay_name.c_str()), TRUE);

        for (const auto &reactant : reaction.reactants)
        {
            std::string_view reactant_name = reactant.getName();

            auto n = agnode(g.get(), const_cast<char *>(reactant_name.data()), TRUE);
            agsafeset(n, const_cast<char *>("shape"), const_cast<char *>("box"), const_cast<char *>(""));
            agsafeset(n, const_cast<char *>("style"), const_cast<char *>("filled"), const_cast<char *>(""));
            agsafeset(n, const_cast<char *>("fillcolor"), const_cast<char *>("cyan"), const_cast<char *>(""));

            std::string delay_string = formatDelay(reaction.rate());
            agsafeset(n_delay, const_cast<char *>("label"), const_cast<char *>(delay_string.c_str()), const_cast<char *>(""));

            agsafeset(n_delay, const_cast<char *>("shape"), const_cast<char *>("oval"), const_cast<char *>(""));
            agsafeset(n_delay, const_cast<char *>("style"), const_cast<char *>("filled"), const_cast<char *>(""));
            agsafeset(n_delay, const_cast<char *>("fillcolor"), const_cast<char *>("yellow"), const_cast<char *>(""));

            agedge(g.get(), n, n_delay, nullptr, TRUE);
        }

        for (const auto &product : reaction.products)
        {
            if (product.getName() == "environment")
            {
                continue;
            }

            std::string_view product_name = product.getName();
            auto n_product = agnode(g.get(), const_cast<char *>(product_name.data()), TRUE);

            agsafeset(n_product, const_cast<char *>("shape"), const_cast<char *>("box"), const_cast<char *>(""));
            agsafeset(n_product, const_cast<char *>("style"), const_cast<char *>("filled"), const_cast<char *>(""));
            agsafeset(n_product, const_cast<char *>("fillcolor"), const_cast<char *>("cyan"), const_cast<char *>(""));

            agedge(g.get(), n_delay, n_product, nullptr, TRUE);
        }
    }

    gvLayout(gvc.get(), g.get(), "dot");

    if (auto file = fopen(filename.c_str(), "w"); file != nullptr)
    {
        gvRender(gvc.get(), g.get(), "png", file);
        fclose(file);
    }
    else
    {
        std::cerr << "Failed to open file for writing\n";
    }

    gvFreeLayout(gvc.get(), g.get());
}
