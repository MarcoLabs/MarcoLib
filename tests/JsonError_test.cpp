#include "marco/json/JsonError.h"
#include <gtest/gtest.h>

TEST(JsonErrorToString, IsValidErrorMessageOnInvalidFormatError)
{
	Marco::JsonError error{Marco::JsonErrorType::InvalidFormat, 21};
	std::string result = error.ToString();

	EXPECT_EQ(result, "Error occured: InvalidFormat at index: 21");
}

TEST(JsonErrorToString, IsValidErrorMessageOnWrongTypeError)
{
	Marco::JsonError error{Marco::JsonErrorType::WrongType, 21};
	std::string result = error.ToString();

	EXPECT_EQ(result, "Error occured: WrongType at index: 21");
}

TEST(JsonErrorToString, IsValidErrorMessageOnNumberParseError)
{
	Marco::JsonError error{Marco::JsonErrorType::NumberParseError, 21};
	std::string result = error.ToString();

	EXPECT_EQ(result, "Error occured: NumberParseError at index: 21");
}

TEST(JsonErrorToString, IsValidErrorMessageOnNumberOutOfRange)
{
	Marco::JsonError error{Marco::JsonErrorType::NumberOutOfRange, 21};
	std::string result = error.ToString();

	EXPECT_EQ(result, "Error occured: NumberOutOfRange at index: 21");
}

TEST(JsonErrorToString, IsValidErrorMessageOnKeyNotFound)
{
	Marco::JsonError error{Marco::JsonErrorType::KeyNotFound, 21};
	std::string result = error.ToString();

	EXPECT_EQ(result, "Error occured: KeyNotFound at index: 21");
}

TEST(JsonErrorToString, IsValidErrorMessageOnNoError)
{
	Marco::JsonError error{Marco::JsonErrorType::NoError, 21};
	std::string result = error.ToString();

	EXPECT_EQ(result, "Error occured: NoError at index: 21");
}

TEST(JsonErrorToString, IsValidErrorMessageOnDefaultConstructor)
{
	Marco::JsonError error{};
	std::string result = error.ToString();

	EXPECT_EQ(result, "Error occured: InvalidFormat at index: 0");
}
