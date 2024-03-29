#include "graph_generator.h"

// Format delay as a string to have as few significant digits as possible while still being accurate - up to 10 significant digits
std::string formatDelay(double delay)
{
    // If it's already a whole number, just return it as such
    if (delay == static_cast<int>(delay))
    {
        return std::to_string(static_cast<int>(delay));
    }
    else // Otherwise, format it as a decimal by finding number of significant digits in the decimal part of `delay` (up to 10 digits) and format `delay` as a string with that many digits of precision.
    {
        double decimalPart = delay - static_cast<int>(delay);
        int precision = 0;
        // Calculate number of significant digits by repeatedly multiplying the decimal part by 10 and checking if the new decimal part is different from its integer part. This continues until the decimal part is essentially an integer (i.e. difference between decimal part and integer part is less than 1e-10) or until 10 significant digits have been found.
        while (std::abs(decimalPart - static_cast<int>(decimalPart)) > 1e-10 && precision < 10)
        {
            decimalPart *= 10;
            precision++;
        }

        // Format delay as a string with the calculated precision
        std::stringstream stream;
        stream << std::setprecision(precision) << std::fixed << delay;
        return stream.str();
    }
}

void generate_graph(const std::vector<Reaction> &reactions, const std::string &filename)
{
    auto gvc = std::unique_ptr<GVC_t, decltype(&gvFreeContext)>(gvContext(), &gvFreeContext);
    auto g = std::unique_ptr<Agraph_t, decltype(&agclose)>(agopen(const_cast<char *>("Reactions"), Agdirected, nullptr), agclose);

    int delayCount = 0; // for unique labels
    for (const auto &reaction : reactions)
    {
        std::stringstream ss;
        ss << "delay_" << delayCount++;
        auto delay_name = ss.str();
        auto n_delay = agnode(g.get(), const_cast<char *>(delay_name.c_str()), TRUE);

        std::string delay_string = formatDelay(reaction.rate());
        agsafeset(n_delay, const_cast<char *>("label"), const_cast<char *>(delay_string.c_str()), const_cast<char *>(""));

        agsafeset(n_delay, const_cast<char *>("shape"), const_cast<char *>("oval"), const_cast<char *>(""));
        agsafeset(n_delay, const_cast<char *>("style"), const_cast<char *>("filled"), const_cast<char *>(""));
        agsafeset(n_delay, const_cast<char *>("fillcolor"), const_cast<char *>("yellow"), const_cast<char *>(""));

        for (const auto &reactant : reaction.reactants)
        {
            std::string_view reactant_name = reactant.getName();

            auto n = agnode(g.get(), const_cast<char *>(reactant_name.data()), TRUE);
            agsafeset(n, const_cast<char *>("shape"), const_cast<char *>("box"), const_cast<char *>(""));
            agsafeset(n, const_cast<char *>("style"), const_cast<char *>("filled"), const_cast<char *>(""));
            agsafeset(n, const_cast<char *>("fillcolor"), const_cast<char *>("cyan"), const_cast<char *>(""));

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
