/// Test numbers conversion.
///
/// @file

#include "omiphone/disablewarnings.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "omiphone/enablewarnings.hpp"

#include "omiphone/numbers.hpp"
#include "omiphone/parser.hpp"

//==========================================================================
namespace omiphone::tests {
//==========================================================================

namespace t = testing;

//==========================================================================

TEST(Parser, Parse)
{
    EXPECT_THAT(omiphone::parse(""), t::ElementsAre());
    EXPECT_THAT(omiphone::parse("30"), t::ElementsAre(30));
    EXPECT_THAT(omiphone::parse("9 3"), t::ElementsAre(9, 3));
    EXPECT_THAT(omiphone::parse("800 3 5 60"),
                t::ElementsAre(800, 3, 5, 60));
}

//--------------------------------------------------------------------------

TEST(Numbers, IsValidPhoneNumber)
{
    EXPECT_FALSE(omiphone::is_valid_phone_number(""));
    EXPECT_FALSE(omiphone::is_valid_phone_number("02"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("023"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("2"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("8"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("23"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("300"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("003"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("69"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("693"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("0069"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("00693"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("00302"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("003069"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("003022"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("0030693"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("1111111111"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("44444444444444"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("6074092252"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("6874092252"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("00306074092252"));
    EXPECT_FALSE(omiphone::is_valid_phone_number("00306874092252"));

    EXPECT_TRUE(omiphone::is_valid_phone_number("2106930664"));
    EXPECT_TRUE(omiphone::is_valid_phone_number("00302106930664"));
    EXPECT_TRUE(omiphone::is_valid_phone_number("6974092252"));
    EXPECT_TRUE(omiphone::is_valid_phone_number("00306974092252"));
}
//--------------------------------------------------------------------------

TEST(Numbers, ExpandElements)
{
    using EE = ExpandedElement;

    EXPECT_THAT(omiphone::expand_elements({}),
                t::ElementsAre());
    EXPECT_THAT(omiphone::expand_elements({0}),
                t::ElementsAre(EE{0}));
    EXPECT_THAT(omiphone::expand_elements({3}),
                t::ElementsAre(EE{3}));
    EXPECT_THAT(omiphone::expand_elements({3, 7}),
                t::ElementsAre(EE{3}, EE{7}));
    EXPECT_THAT(omiphone::expand_elements({3, 70}),
                t::ElementsAre(EE{3}, EE{70}));
    EXPECT_THAT(omiphone::expand_elements({30, 70}),
                t::ElementsAre(EE{30}, EE{70}));

    EXPECT_THAT(omiphone::expand_elements({10}),
                t::ElementsAre(EE{10}));
    EXPECT_THAT(omiphone::expand_elements({11}),
                t::ElementsAre(EE{11}));
    EXPECT_THAT(omiphone::expand_elements({19}),
                t::ElementsAre(EE{19}));

    EXPECT_THAT(omiphone::expand_elements({20}),
                t::ElementsAre(EE{20}));
    EXPECT_THAT(omiphone::expand_elements({21}),
                t::ElementsAre(EE{20}, EE{1}));
    EXPECT_THAT(omiphone::expand_elements({33}),
                t::ElementsAre(EE{30}, EE{3}));
    EXPECT_THAT(omiphone::expand_elements({333}),
                t::ElementsAre(EE{300}, EE{30}, EE{3}));

    EXPECT_THAT(omiphone::expand_elements({301}),
                t::ElementsAre(EE{300}, EE{1}));
    EXPECT_THAT(omiphone::expand_elements({309}),
                t::ElementsAre(EE{300}, EE{9}));

    EXPECT_THAT(omiphone::expand_elements({310}),
                t::ElementsAre(EE{300}, EE{10}));
    EXPECT_THAT(omiphone::expand_elements({311}),
                t::ElementsAre(EE{300}, EE{11}));

    EXPECT_THAT(omiphone::expand_elements({320}),
                t::ElementsAre(EE{300}, EE{20}));
    EXPECT_THAT(omiphone::expand_elements({321}),
                t::ElementsAre(EE{300}, EE{20}, EE{1}));

    EXPECT_THAT(omiphone::expand_elements({3020}),
                t::ElementsAre(EE{3000}, EE{20}));
}

//--------------------------------------------------------------------------

TEST(Numbers, FreeDigitPlaces)
{
    EXPECT_EQ(omiphone::free_digit_places(0), 0);
    EXPECT_EQ(omiphone::free_digit_places(1), 0);
    EXPECT_EQ(omiphone::free_digit_places(9), 0);
    EXPECT_EQ(omiphone::free_digit_places(10), 0);
    EXPECT_EQ(omiphone::free_digit_places(11), 0);
    EXPECT_EQ(omiphone::free_digit_places(19), 0);
    EXPECT_EQ(omiphone::free_digit_places(20), 1);
    EXPECT_EQ(omiphone::free_digit_places(21), 0);
    EXPECT_EQ(omiphone::free_digit_places(90), 1);
    EXPECT_EQ(omiphone::free_digit_places(99), 0);
    EXPECT_EQ(omiphone::free_digit_places(100), 2);
    EXPECT_EQ(omiphone::free_digit_places(101), 0);
    EXPECT_EQ(omiphone::free_digit_places(110), 0);
    EXPECT_EQ(omiphone::free_digit_places(119), 0);
    EXPECT_EQ(omiphone::free_digit_places(120), 1);
    EXPECT_EQ(omiphone::free_digit_places(1000), 3);
    EXPECT_EQ(omiphone::free_digit_places(1001), 0);
    EXPECT_EQ(omiphone::free_digit_places(1010), 0);
    EXPECT_EQ(omiphone::free_digit_places(1019), 0);
    EXPECT_EQ(omiphone::free_digit_places(1020), 1);
    EXPECT_EQ(omiphone::free_digit_places(1100), 2);
}

//--------------------------------------------------------------------------

TEST(Numbers, DigitsCount)
{
    EXPECT_EQ(omiphone::digits_count(0), 1);
    EXPECT_EQ(omiphone::digits_count(1), 1);
    EXPECT_EQ(omiphone::digits_count(9), 1);
    EXPECT_EQ(omiphone::digits_count(10), 2);
    EXPECT_EQ(omiphone::digits_count(11), 2);
    EXPECT_EQ(omiphone::digits_count(19), 2);
    EXPECT_EQ(omiphone::digits_count(20), 2);
    EXPECT_EQ(omiphone::digits_count(90), 2);
    EXPECT_EQ(omiphone::digits_count(99), 2);
    EXPECT_EQ(omiphone::digits_count(100), 3);
    EXPECT_EQ(omiphone::digits_count(1000), 4);
}

//--------------------------------------------------------------------------

TEST(Numbers, Interpretations)
{
    using EE = ExpandedElement;

    EXPECT_THAT(omiphone::interpretations({}),
                t::UnorderedElementsAre());
    EXPECT_THAT(omiphone::interpretations({EE{0}}),
                t::UnorderedElementsAre("0"));
    EXPECT_THAT(omiphone::interpretations({EE{3}}),
                t::UnorderedElementsAre("3"));
    EXPECT_THAT(omiphone::interpretations({EE{30}}),
                t::UnorderedElementsAre("30"));
    EXPECT_THAT(omiphone::interpretations({EE{300}}),
                t::UnorderedElementsAre("300"));

    EXPECT_THAT(omiphone::interpretations({EE{2}, EE{10}, EE{6}, EE{9}, EE{30}, EE{6}}),
                t::UnorderedElementsAre("21069306", "2106936"));
    EXPECT_THAT(omiphone::interpretations({EE{700}, EE{3}, EE{50}, EE{9}}),
                t::UnorderedElementsAre("70359", "703509", "700359", "7003509"));

    // "ten"
    EXPECT_THAT(omiphone::interpretations({EE{10}, EE{7}}),
                t::UnorderedElementsAre("107"));
    EXPECT_THAT(omiphone::interpretations({EE{200}, EE{10}, EE{7}}),
                t::UnorderedElementsAre("200107", "2107"));

    EXPECT_THAT(omiphone::interpretations({EE{70}, EE{0}}),
                t::UnorderedElementsAre("700"));
    EXPECT_THAT(omiphone::interpretations({EE{700}, EE{0}}),
                t::UnorderedElementsAre("7000"));
    EXPECT_THAT(omiphone::interpretations({EE{70}, EE{1}}),
                t::UnorderedElementsAre("71", "701"));
    EXPECT_THAT(omiphone::interpretations({EE{70}, EE{700}}),
                t::UnorderedElementsAre("70700"));
    EXPECT_THAT(omiphone::interpretations({EE{700}, EE{70}}),
                t::UnorderedElementsAre("70070", "770"));
    EXPECT_THAT(omiphone::interpretations({EE{7000}, EE{70}, EE{7}}),
                t::UnorderedElementsAre("7000707", "70707", "7077", "700077"));
    EXPECT_THAT(omiphone::interpretations({EE{70}, EE{30}}),
                t::UnorderedElementsAre("7030"));
    EXPECT_THAT(omiphone::interpretations({EE{70}, EE{30}, EE{8}}),
                t::UnorderedElementsAre("70308", "7038"));
    EXPECT_THAT(omiphone::interpretations({EE{700}, EE{300}}),
                t::UnorderedElementsAre("700300"));
}

//==========================================================================
} // namespace omiphone::tests
