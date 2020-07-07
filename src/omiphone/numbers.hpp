/// Process phone numbers.
///
/// @file

#pragma once
#ifndef NUMBERS_HPP__Y0V1WMOX
#define NUMBERS_HPP__Y0V1WMOX

#include <string>
#include <string_view>
#include <vector>

#include "omiphone/disablewarnings.hpp"
#include <strong_type/strong_type.hpp>
#include "omiphone/enablewarnings.hpp"

//==========================================================================
namespace omiphone {
//==========================================================================

/// Input phone number element. E.g. 1, 10, 98, 23453, 402, ...
using Element = uint32_t;
/// Expanded phone number element, not divisible, mostly contains a single
/// non-zero digit. E.g. 1, 40, 100, 5000
using ExpandedElement = strong::type<uint32_t, struct ExpElm_, strong::regular>;

//==========================================================================

/// @return true if the input is a valid Greek phone number.
bool is_valid_phone_number(const std::string_view phone_number);

/// Expand "complex" numbers to individual components, e.g. {8, 321} -> {8, 300, 20, 1}
std::vector<ExpandedElement> expand_elements(const std::vector<Element>& elements);

/// Count how many digits from the left are zeros and suitable for combination with
/// "smaller" numbers.  E.g. 5->0, 13->0, 20->1, 23->0, 400->2, 5000->3 ...
uint32_t free_digit_places(uint32_t element);

/// @return how many digits the number contains
uint32_t digits_count(uint32_t number);

/// @param elements expanded elements
/// @return list of phone number interpretations
std::vector<std::string> interpretations(const std::vector<ExpandedElement>& elements);

//==========================================================================
} // namespace omiphone

#endif /* include guard */
