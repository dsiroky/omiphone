#include <iostream>

#include <cassert>
#include <charconv>

#include "omiphone/parser.hpp"

//==========================================================================
namespace omiphone {
//==========================================================================

static uint32_t to_int(const std::string_view s)
{
    uint32_t result{};
    [[maybe_unused]] const auto fc_result
        = std::from_chars(s.data(), s.data() + s.size(), result);
    // input must be valid
    assert(fc_result.ec != std::errc::invalid_argument);
    return result;
}

//==========================================================================

std::vector<uint32_t> parse(const std::string_view input)
{
    if (input.size() == 0)
    {
        return {};
    }

    std::vector<uint32_t> elements{};
    size_t old_pos{0};
    size_t pos{input.find(' ')};
    while (pos != std::string::npos)
    {
        elements.push_back(
            to_int(std::string_view{input.data() + old_pos, pos - old_pos}));
        old_pos = pos + 1;
        pos = input.find(' ', old_pos);
    }
    elements.push_back(to_int(std::string_view{input.data() + old_pos}));

    return elements;
}

//==========================================================================
} // namespace omiphone
