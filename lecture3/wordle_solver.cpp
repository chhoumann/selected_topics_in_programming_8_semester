#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <regex>

std::vector<std::string> read_wordle_js(std::istream &&is)
{
    std::vector<std::string> words;
    auto word = std::string{};

    while (is)
    {
        if (is.peek() == '"')
        {
            is >> std::quoted(word);  // read word in quotes and remove quotes
            if (word.length() == 5 && // len = 5
                std::all_of(std::cbegin(word), std::cend(word), [](char c)
                            { return std::isalpha(c) && std::islower(c); })) // all lowercase
                words.push_back(word);
        }
        else
        {
            is.get(); // skip char
        }
    }

    std::sort(std::begin(words), std::end(words));
    words.erase(std::unique(std::begin(words), std::end(words)), words.end());

    return words;
}

// Pattern is based on word of length 5, but has + prepended if char is yellow, - prepended if char is gray, and just the char if green
// yellow = char is present, but in wrong place
// gray = char is absent, should ignore going forward
// green = char is present & in right place
// E.g. +S-L-I-DE means S is yellow, LID is gray, and E is green.
std::vector<std::string> filter(std::vector<std::string> &&words, std::vector<char> &ignoreChars, const std::string &pattern)
{
    std::regex patternMatcher("([+-]{0,1}[a-zA-Z]{1})");
    auto p_begin = std::sregex_iterator(pattern.begin(), pattern.end(), patternMatcher);
    auto p_end = std::sregex_iterator();

    std::vector<char> shouldContain{};

    // Filter here
    std::string targetStrRxp{};
    for (std::sregex_iterator i = p_begin; i != p_end; ++i)
    {
        std::smatch match = *i;
        std::string match_str = match.str();

        if (match_str.size() == 2)
        {
            if (match_str[0] == '-')
            {
                ignoreChars.push_back(match_str.at(1));
            }

            if (match_str[0] == '+')
            {
                shouldContain.push_back(match_str.at(1));
            }

            targetStrRxp.append("[^");
            for (char ignored : ignoreChars)
                targetStrRxp.append(std::string{ignored});
            targetStrRxp.append("]");
        }

        if (match_str.size() == 1)
        {
            targetStrRxp.append(match_str);
        }
    }

    words.erase(std::remove_if(words.begin(), words.end(), [&](const std::string &word)
                               { const bool remove = !std::regex_search(word, std::regex(targetStrRxp)); /* std::cout << "Should remove " << word << ": " << remove << std::endl; */ return remove; }),
                words.end());

    std::cout << "Candidates: ";
    for (auto word : words)
        std::cout << word << ", ";

    std::cout << std::endl;

    // Should sort by strings containing chars from shouldContain.
    // But this works pretty well already.

    return words;
}

int main()
{
    const auto database = read_wordle_js(std::ifstream("words.txt"));
    std::regex sanitizer("^([+-]{0,1}[a-zA-Z]{1}){5}$");
    auto words = database;      // make copy
    auto guess = std::string{}; // allocate memory to guess patterns
    std::vector<char> ignoredChars{};

    while (words.size() > 1)
    {
        std::cout << words.size() << " words remaining" << std::endl;
        bool good = true;

        do
        {
            std::cout << "Enter a guess pattern: ";
            std::cin >> guess;
            /* Sanitize: set good to false if input fails to follow the pattern */
            good = std::regex_search(guess, sanitizer);
        } while (!good);

        /* Apply filter on words */
        words = std::move(filter(std::move(words), ignoredChars, guess));
        /* Pick and announce one of the words for the user to try */
        if (words.size() >= 1)
            std::cout << "Try: " << words.front() << std::endl;
    }

    if (words.size() == 1)
    {
        std::cout << "Congratuations: " << words.front() << std::endl;
    }
    else
    {
        std::cerr << "Something got messed up :-(" << std::endl;
    }

    return 0;
}