#include "marco/toml/TomlError.h"
#include "marco/toml/TomlTime.h"
#include <gtest/gtest.h>

TEST(TomlTimeConstruction, DefaultConstructorIsZero)
{
	Marco::TomlTime time;

	EXPECT_EQ(time.hour,        0);
	EXPECT_EQ(time.minute,      0);
	EXPECT_EQ(time.second,      0);
	EXPECT_EQ(time.millisecond, 0);
}

TEST(TomlTimeConstruction, ParameterizedConstructorSetsValues)
{
	Marco::TomlTime time(12, 30, 45, 500);

	EXPECT_EQ(time.hour,        12);
	EXPECT_EQ(time.minute,      30);
	EXPECT_EQ(time.second,      45);
	EXPECT_EQ(time.millisecond, 500);
}

TEST(TomlTimeFromString, TooShortStringReturnsError)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("12:3");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlTimeFromString, MissingColonAtPositionTwoReturnsError)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("1230");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlTimeFromString, VaildHourMinuteParsesCorrectly)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("12:30");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::NoError);
	EXPECT_EQ(time.hour,   12);
	EXPECT_EQ(time.minute, 30);
	EXPECT_EQ(time.second, 0);
}

TEST(TomlTimeFromString, HourMinuteOutOfBoundryReturnsError)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("99:00");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlTimeFromString, MissingColonBeforeSecondsReturnsError)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("12:30x00");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlTimeFromString, ValidHourMinuteSecondParsesCorrectly)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("12:30:45");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::NoError);
	EXPECT_EQ(time.hour,   12);
	EXPECT_EQ(time.minute, 30);
	EXPECT_EQ(time.second, 45);
}

TEST(TomlTimeFromString, SecondOutOfBoundaryReturnsError)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("12:30:99");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlTimeFromString, MissingDotBeforeMillisecondReturnsError)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("12:30:45x123");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlTimeFromString, VaildFullTimeWithMillisecondParsesCorrectly)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("12:30:45.123");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::NoError);
	EXPECT_EQ(time.hour,        12);
	EXPECT_EQ(time.minute,      30);
	EXPECT_EQ(time.second,      45);
	EXPECT_EQ(time.millisecond, 123);
}

TEST(TomlTimeFromString, MillisecondLingerThanSixDigitsReturnsError)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("12:30:45.1234567");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlTimeFromString, NonNumericHourReturnsError)
{
	Marco::TomlTime time{};

	Marco::TomlError error = time.FromString("ab:cd");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlTimeAsString, HourAndMinutePadsWithLeadingZero)
{
	Marco::TomlTime time(5, 3);

	EXPECT_EQ(time.AsString(), "05:03");
}

TEST(TomlTimeAsString, IncludesSecondWhenNonZero)
{
	Marco::TomlTime time(5, 3, 9);

	EXPECT_EQ(time.AsString(), "05:03:09");
}

TEST(TomlTimeAsString, OmitsSecondsAndMillisecondsWhenZero)
{
	Marco::TomlTime time(5, 3, 0, 0);

	EXPECT_EQ(time.AsString(), "05:03");
}

TEST(TomlTimeAsString, ZeroSecondWithMillisecondIncludesZeroSeconds)
{
	Marco::TomlTime time(5, 3, 0, 500);

	EXPECT_EQ(time.AsString(), "05:03:00.500");
}

TEST(TomlTimeAsString, IncludesMillisecondWhenSecondNonZero)
{
	Marco::TomlTime time(5, 3, 9, 250);

	EXPECT_EQ(time.AsString(), "05:03:09.250");
}

TEST(TomlTimeBoundary, HourAtBoundryIsValid)
{
	Marco::TomlTime time(24);

	EXPECT_TRUE(time.IsWithinBoundry());
}

TEST(TomlTimeBoundary, HourAboveBoundaryIsInvalid)
{
	Marco::TomlTime time(25);

	EXPECT_FALSE(time.IsWithinBoundry());
}

TEST(TomlTimeBoundary, MinuteAtBoundaryIsValid)
{
	Marco::TomlTime time(0, 60);

	EXPECT_TRUE(time.IsWithinBoundry());
}

TEST(TomlTimeBoundary, MinuteAboveBoundaryIsInvalid)
{
	Marco::TomlTime time(0, 61);

	EXPECT_FALSE(time.IsWithinBoundry());
}

TEST(TomlTimeBoundary, SecondAtBoundaryIsValid)
{
	Marco::TomlTime time(0, 0, 60);

	EXPECT_TRUE(time.IsWithinBoundry());
}

TEST(TomlTimeBoundary, SecondAboveBoundaryIsInvalid)
{
	Marco::TomlTime time(0, 0, 61);

	EXPECT_FALSE(time.IsWithinBoundry());
}

TEST(TomlTimeBoundary, MillisecondAtBoundaryIsValid)
{
	Marco::TomlTime time(0, 0, 0, 999999);

	EXPECT_TRUE(time.IsWithinBoundry());
}

TEST(TomlTimeBoundary, MillisecondAboveBoundaryIsInvalid)
{
	Marco::TomlTime time(0, 0, 0, 1000000);

	EXPECT_FALSE(time.IsWithinBoundry());
}
