#include <iostream>

#include "omiphone/numbers.hpp"
#include "omiphone/parser.hpp"

int main(int, char *[])
{
    std::string input{};
    std::getline(std::cin, input);
    if (input.size() == 0)
    {
        std::cout << "no input" << '\n';
    }
    else
    {
        // "pipeline"
        const auto elements = omiphone::parse(input);
        const auto expanded_elements = omiphone::expand_elements(elements);
        const auto interpretations = omiphone::interpretations(expanded_elements);

        size_t counter{1};
        for (const auto& interpretation: interpretations)
        {
            std::cout
                << "Interpretation " << counter << ": "
                << interpretation
                << " [phone number: "
                << (omiphone::is_valid_phone_number(interpretation)
                        ? "VALID"
                        : "INVALID")
                << "]\n";
            ++counter;
        }
    }
    return 0;
}
