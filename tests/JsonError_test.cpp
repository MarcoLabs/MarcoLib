#include "marco/json/JsonError.h"
#include <gtest/gtest.h>

TEST(ToStringInvalidFormatTest, ToString)
{
    Marco::JsonError error{Marco::JsonErrorType::InvalidFormat, 21};
    std::string result = error.ToString();

    EXPECT_EQ(result, "Error occured: InvalidFormat at index: 21");
}

TEST(ToStringWrongTypeTest, ToString)
{
    Marco::JsonError error{Marco::JsonErrorType::WrongType, 21};
    std::string result = error.ToString();

    EXPECT_EQ(result, "Error occured: WrongType at index: 21");
}

TEST(ToStringNumberParseErrorTest, ToString)
{
    Marco::JsonError error{Marco::JsonErrorType::NumberParseError, 21};
    std::string result = error.ToString();

    EXPECT_EQ(result, "Error occured: NumberParseError at index: 21");
}

TEST(ToStringNumberOutOfRangeTest, ToString)
{
    Marco::JsonError error{Marco::JsonErrorType::NumberOutOfRange, 21};
    std::string result = error.ToString();

    EXPECT_EQ(result, "Error occured: NumberOutOfRange at index: 21");
}

TEST(ToStringKeyNotFoundTest, ToString)
{
    Marco::JsonError error{Marco::JsonErrorType::KeyNotFound, 21};
    std::string result = error.ToString();

    EXPECT_EQ(result, "Error occured: KeyNotFound at index: 21");
}

TEST(ToStringNoErrorTest, ToString)
{
    Marco::JsonError error{Marco::JsonErrorType::NoError, 21};
    std::string result = error.ToString();

    EXPECT_EQ(result, "Error occured: NoError at index: 21");
}
