/// Parse input into individual elements.
///
/// @file

#pragma once
#ifndef PARSER_HPP__DERLYKV2
#define PARSER_HPP__DERLYKV2

#include <cstdint>
#include <string>
#include <vector>

//==========================================================================
namespace omiphone {
//==========================================================================

/// Split raw input values in a string into individual elements. Input must
/// contain only numbers separated by a single space. Otherwise it will lead to
/// undefined behavior.
/// E.g. "20 4 9 3" -> {20, 4, 9, 3}
std::vector<uint32_t> parse(const std::string_view input);

//==========================================================================
} // namespace omilia

#endif /* include guard */
