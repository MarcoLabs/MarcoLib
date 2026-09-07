#include "marco/toml/TomlDate.h"
#include "marco/toml/TomlError.h"
#include <gtest/gtest.h>

TEST(TomlDateConstruction, DefaultContructionIsZero)
{
	Marco::TomlDate date;

	EXPECT_EQ(date.year,  0);
	EXPECT_EQ(date.month, 0);
	EXPECT_EQ(date.day,   0);
}

TEST(TomlDateContruction, ParameterizedConstructorSetDateFields)
{
	Marco::TomlDate date(2024, 6, 15);

	EXPECT_EQ(date.year,  2024);
	EXPECT_EQ(date.month, 6);
	EXPECT_EQ(date.day,   15);
}

TEST(TomlDateFromString, TooShortStringReturnsError)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-06");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlDateFromString, DateOnlyParsesCorrectly)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-06-15");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::NoError);
	EXPECT_EQ(date.year, 2024);
	EXPECT_EQ(date.month, 6);
	EXPECT_EQ(date.day, 15);
}

TEST(TomlDateFromString, MonthOutOfBoundaryReturnsError)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-13-15");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlDateFromString, DayOutOfBoundaryReturnsError)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-12-32");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlDateFromString, MissingTimeDelimiterReturnsError)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-06-15X10:00:00Z");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlDateFromString, DateWithZuluTimeParsesCorrectly)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-06-15T10:30:00Z");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::NoError);
	EXPECT_EQ(date.year, 2024);
	EXPECT_EQ(date.month, 6);
	EXPECT_EQ(date.day, 15);
	EXPECT_EQ(date.offsetTimeStart.hour, 10);
	EXPECT_EQ(date.offsetTimeStart.minute, 30);
}

TEST(TomlDateFromString, DateWithZuluTimeSetsBothOffsetsEqual)
{
	Marco::TomlDate date{};

	date.FromString("2024-06-15T10:30:00Z");

	EXPECT_EQ(date.offsetTimeStart.hour,   date.offsetTimeEnd.hour);
	EXPECT_EQ(date.offsetTimeStart.minute, date.offsetTimeEnd.minute);
}

TEST(TomlDateFromString, DateWithSpaceDelimiterParsesCorrectly)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-06-15 10:30:00Z");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::NoError);
	EXPECT_EQ(date.offsetTimeStart.hour, 10);
}

TEST(TomlDateFromString, DateWithTimeRangeParsesBothOffsets)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-06-15T09:00-17:00");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::NoError);
	EXPECT_EQ(date.offsetTimeStart.hour, 9);
	EXPECT_EQ(date.offsetTimeEnd.hour,   17);
}

TEST(TomlDateFromString, InvalidStartTimeInRangeReturnsError)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-06-15T99:00-17:00");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlDateFromString, InvalidEndTimeInRangeReturnsError)
{
	Marco::TomlDate date{};

	Marco::TomlError error = date.FromString("2024-06-15T09:00-99:00");

	EXPECT_EQ(error.errorType, Marco::TomlErrorType::InvalidDateFormat);
}

TEST(TomlDateAsString, DateOnlyWithNoOffsetOmitsTime)
{
	Marco::TomlDate date(2024, 6, 15);

	EXPECT_EQ(date.AsString(), "2024-06-15");
}

TEST(TomlDateAsString, PadsSingleDigitMonthAndDay)
{
	Marco::TomlDate date(2024, 3, 5);

	EXPECT_EQ(date.AsString(), "2024-03-05");
}

TEST(TomlDateAsString, SingleOffsetAppendsZuluTime)
{
	Marco::TomlDate date{};
	date.FromString("2024-06-15T10:30Z");

	EXPECT_EQ(date.AsString(), "2024-06-15T10:30Z");
}

TEST(TomlDateAsString, DifferingOffsetsAppendTimeRange)
{
	Marco::TomlDate date{};

	date.FromString("2024-06-15T09:00-17:00");

	EXPECT_EQ(date.AsString(), "2024-06-15T09:00-17:00");
}

TEST(TomlDateBoundary, MonthBelowOneIsInvalid)
{
	Marco::TomlDate date(2024, 0, 15);

	EXPECT_FALSE(date.IsWithinBoundry());
}

TEST(TomlDateBoundary, MonthAtUpperBoundaryIsValid)
{
	Marco::TomlDate date(2024, 12, 15);

	EXPECT_TRUE(date.IsWithinBoundry());
}

TEST(TomlDateBoundary, MonthAboveBoundaryIsInvalid)
{
	Marco::TomlDate date(2024, 13, 15);

	EXPECT_FALSE(date.IsWithinBoundry());
}

TEST(TomlDateBoundary, DayBelowOneIsInvalid)
{
	Marco::TomlDate date(2024, 6, 0);

	EXPECT_FALSE(date.IsWithinBoundry());
}

TEST(TomlDateBoundary, DayAtUpperBoundaryIsValid)
{
	Marco::TomlDate date(2024, 6, 31);

	EXPECT_TRUE(date.IsWithinBoundry());
}

TEST(TomlDateBoundary, DayAboveBoundaryIsInvalid)
{
	Marco::TomlDate date(2024, 6, 32);

	EXPECT_FALSE(date.IsWithinBoundry());
}

TEST(TomlDateBoundary, InvalidOffsetTimeStartMakesDateInvalid)
{
	Marco::TomlDate date(2024, 6, 15);
	date.offsetTimeStart.hour = 25;

	EXPECT_FALSE(date.IsWithinBoundry());
}

TEST(TomlDateBoundary, InvalidOffsetTimeEndMakesDateInvalid)
{
	Marco::TomlDate date(2024, 6, 15);
	date.offsetTimeEnd.minute = 61;

	EXPECT_FALSE(date.IsWithinBoundry());
}
