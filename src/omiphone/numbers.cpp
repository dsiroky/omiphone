#include <cassert>

#include "omiphone/numbers.hpp"

//==========================================================================
namespace omiphone {
//==========================================================================

/// When numbers start to be combinable.
static constexpr uint32_t ATOMIC_THRESHOLD{20};

//==========================================================================

static bool starts_with(const std::string_view txt, const std::string_view fragment)
{
    assert(txt.size() >= fragment.size());
    return txt.substr(0, fragment.size()) == fragment;
}

//==========================================================================

bool is_valid_phone_number(const std::string_view phone_number)
{
    const bool without_area_code{
        (phone_number.size() == 10)
        and (starts_with(phone_number, "2") or starts_with(phone_number, "69"))};
    const bool with_area_code{(
        (phone_number.size() == 14)
        and (starts_with(phone_number, "00302") or starts_with(phone_number, "003069")))};
    return without_area_code or with_area_code;
}

//--------------------------------------------------------------------------

std::vector<ExpandedElement> expand_elements(const std::vector<uint32_t>& elements)
{
    std::vector<ExpandedElement> new_elements{};

    for (auto element: elements)
    {
        if (element == 0)
        {
            new_elements.emplace_back(0);
            continue;
        }

        // iterate through digits from the left to the right
        std::vector<ExpandedElement> inversed_expansion{};
        uint32_t mask{};
        // the first 2 left digits are "special"
        if ((element % 100) < ATOMIC_THRESHOLD)
        {
            mask = 100;
        }
        else
        {
            mask = 10;
        }

        while (element != 0)
        {
            const uint32_t part{element % mask};
            if (part != 0)
            {
                inversed_expansion.emplace_back(part);
            }
            element -= part;
            mask *= 10;
        }

        std::copy(inversed_expansion.rbegin(), inversed_expansion.rend(),
                  std::back_inserter(new_elements));
    }

    return new_elements;
}

//--------------------------------------------------------------------------

uint32_t free_digit_places(uint32_t element)
{
    if ((element < ATOMIC_THRESHOLD)
        or ((((element % 100) < ATOMIC_THRESHOLD)) and ((element % 100) != 0)))
    {
        return 0;
    }

    uint32_t counter{0};

    while ((element != 0) and ((element % 10) == 0))
    {
        element /= 10;
        ++counter;
    }

    return counter;
}

//--------------------------------------------------------------------------

uint32_t digits_count(uint32_t number)
{
    uint32_t counter{1};
    number /= 10;
    while (number != 0)
    {
        number /= 10;
        ++counter;
    }
    return counter;
}

//--------------------------------------------------------------------------

[[maybe_unused]]
static uint32_t decimal_power(const uint32_t decimal_power)
{
    uint32_t res{1};
    for (uint32_t i{1}; i <= decimal_power; ++i)
    {
        res *= 10;
    }
    return res;
}

//--------------------------------------------------------------------------

/// Walk the tree of number combinations.
static void combine(std::vector<std::string>& interps,
                    std::vector<Element> buffer,
                    bool do_combine,
                    const std::vector<ExpandedElement>::const_iterator expanded_begin,
                    const std::vector<ExpandedElement>::const_iterator expanded_end)
{
    for (auto expanded_it = expanded_begin; (expanded_it != expanded_end) ; ++expanded_it)
    {
        const uint32_t current_elm = strong::value_of(*expanded_it);
        if (do_combine)
        {
            buffer.back() += current_elm;
            // just to make sure it is really combinable
            assert((buffer.back() % decimal_power(digits_count(current_elm)))
                   == current_elm);
            do_combine = false;
        }
        else
        {
            buffer.emplace_back(current_elm);
        }

        const auto next_it = std::next(expanded_it);
        // Don't do combinations if:
            // element is the last
        if ((next_it == expanded_end)
            // cannot be combined with the next element
            or (free_digit_places(current_elm)
                < digits_count(strong::value_of(*next_it)))
            // zero is not combinable
            or (strong::value_of(*next_it) == 0))
        {
            continue;
        }

        // we are not in the leaf yet
        combine(interps, buffer, false, std::next(expanded_it), expanded_end);
        combine(interps, buffer, true, std::next(expanded_it), expanded_end);
        return;
    }

    // if we got here then we are in the tree leaf
    std::string interpretation{};
    for (const auto buf_elm: buffer)
    {
        interpretation += std::to_string(strong::value_of(buf_elm));
    }
    interps.emplace_back(std::move(interpretation));
}

//--------------------------------------------------------------------------

std::vector<std::string> interpretations(const std::vector<ExpandedElement>& elements)
{
    if (elements.size() == 0)
    {
        return {};
    }

    std::vector<std::string> interps{};
    combine(interps, {}, false, elements.begin(), elements.end());
    return interps;
}

//==========================================================================
} // namespace omiphone
