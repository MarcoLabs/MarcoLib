#include "marco/toml/Toml.h"
#include "marco/toml/TomlError.h"
#include "marco/toml/TomlReader.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(TomlReaderConstruction, IsValidFalseBeforeParseIsCalled)
{
	Marco::TomlReader reader{};

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderParse, ParsesEmptyStringAsValid)
{
	Marco::TomlReader reader{};
	reader.Parse("");

	EXPECT_TRUE(reader.IsValid());
}

TEST(TomlReaderParse, ParsesWhitespaceOnlyStringAsValid)
{
	Marco::TomlReader reader{};
	reader.Parse("	\t\n\n   ");

	EXPECT_TRUE(reader.IsValid());
}

TEST(TomlReaderParse, IsValidUpdateAcrossMultipleParseCalls)
{
	Marco::TomlReader reader{};

	reader.Parse("key = @@@");
	EXPECT_FALSE(reader.IsValid());

	reader.Parse("key = 1");
	EXPECT_TRUE(reader.IsValid());

	reader.Parse("key = @@@");
	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderParse, ParseFromIstream)
{
	Marco::TomlReader reader{};

	std::istringstream iss("key = 1");
	Marco::Toml value = reader.Parse(iss);

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 1);
}

TEST(TomlReaderParse, IgnoreLeadingAndTrailingWHitespaceInDocument)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("\n\n	key = 1  \n\n");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 1);
}

TEST(TomlReaderParse, ToleratesWhitespaceAroundEqualSign)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key     =		1");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 1);
}

TEST(TomlReaderKeys, ParsesBareKey)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 1");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 1);
}

TEST(TomlReaderKeys, ParsesQuotedStringKey)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse(R"("key with spaces" = 1)");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key with spaces"].IsNumber());
	EXPECT_EQ(value["key with spaces"].AsNumber().value(), 1);
}

TEST(TomlReaderKeys, ParsesLiteralQuotedKey)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse(R"('key' = 1)");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 1);
}

TEST(TomlReaderKeys, FailsOnInvalidBareKeyCharacter)
{
	Marco::TomlReader reader{};
	reader.Parse("k$ey = 1");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderKeys, FailsOnDuplicateKey)
{
	Marco::TomlReader reader{};
	reader.Parse("key = 1\nkey = 2");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderNumbers, ParsesPositiveInteger)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 42");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 42);
}

TEST(TomlReaderNumbers, ParsesNegativeInteger)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = -42");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), -42);
}

TEST(TomlReaderNumbers, ParsesExplicitPositiveInteger)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = +42");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 42);
}

TEST(TomlReaderNumbers, ParsesFloatingPointValue)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 3.14");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 3.14);
}

TEST(TomlReaderNumbers, ParsesLowercaseExponent)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 2e3");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 2000);
}

TEST(TomlReaderNumbers, ParsesUppercaseExponent)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 2E3");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 2000);
}

TEST(TomlReaderNumbers, ParsesNegativeExponent)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 1.23e-4");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 1.23e-4);
}

TEST(TomlReaderNumbers, ParesHexLiteralContainingUppercaseE)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 0xAE");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 0xAE);
}

TEST(TomlReaderNumbers, ParsesHexLiteralLowercase)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 0xff");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 0xff);
}

TEST(TomlReaderNumbers, ParsesOctalLiteral)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 0o17");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 017);
}

TEST(TomlReaderNumbers, ParsesBinaryLiteral)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 0b101");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 5);
}

TEST(TomlReaderNumbers, ParsesIntegerWithUnderscoresSeperators)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 1_000_000");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsNumber());
	EXPECT_EQ(value["key"].AsNumber().value(), 1000000);
}

TEST(TomlReaderNumbers, FailsOnLoneMinuseSignNumber)
{
	Marco::TomlReader reader{};
	reader.Parse("key = -");

	EXPECT_FALSE(reader.IsValid());
	EXPECT_EQ(reader.Error().errorType, Marco::TomlErrorType::InvalidNumberFormat);
}

TEST(TomlReaderNumbers, FailsOnMalformedFloatWithTwoDots)
{
	Marco::TomlReader reader{};
	reader.Parse("key = 1.2.3");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderBooleans, ParsesTrueValue)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = true");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsBool());
	EXPECT_EQ(value["key"].AsBool().value(), true);
}

TEST(TomlReaderBooleans, ParsesFalseValue)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = false");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsBool());
	EXPECT_EQ(value["key"].AsBool().value(), false);
}

TEST(TomlReaderBooleans, FailsOnInvalidBooleanToken)
{
	Marco::TomlReader reader{};
	reader.Parse("key = tru");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderStrings, ParsesBasicString)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse(R"(key = "value")");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsString());
	EXPECT_EQ(value["key"].AsString().value().get(), "value");
}

TEST(TomlReaderStrings, ParsesLiteralString)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse(R"(key = 'C:\no\escapes')");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsString());
	EXPECT_EQ(value["key"].AsString().value().get(), R"(C:\no\escapes)");
}

TEST(TomlReaderStrings, ParsesStringWithEscapedQuote)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse(R"(key = "escaped \"quotes\"")");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsString());
	EXPECT_EQ(value["key"].AsString().value().get(), R"(escaped "quotes")");
}

TEST(TomlReaderStrings, ParsesStringWithEscapedNewlineAndTab)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse(R"(key = "line1\n\tline2")");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsString());
	EXPECT_EQ(value["key"].AsString().value().get(), "line1\n\tline2");
}

TEST(TomlReaderStrings, ParsesMultilineBasicString)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = \"\"\"hello\nworld\"\"\"");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsString());
	EXPECT_EQ(value["key"].AsString().value().get(), "hello\nworld");
}

TEST(TomlReaderStrings, ParsesMultilineLiteralString)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = '''hello\nworld'''");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsString());
	EXPECT_EQ(value["key"].AsString().value().get(), "hello\nworld");
}

TEST(TomlReaderStrings, MultilineBasicStringTrimsLinesContinuationWhitespace)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = \"\"\"hello \\\n\t world\"\"\"");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsString());
	EXPECT_EQ(value["key"].AsString().value().get(), "hello world");
}

TEST(TomlReaderStrings, FailsOnUnescapedNewlineInBasicString)
{
	Marco::TomlReader reader{};
	reader.Parse("key = \"line1\nline2\"");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderStrings, FailsOnUnclosedString)
{
	Marco::TomlReader reader{};
	reader.Parse(R"(key = "unclosed)");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderArrays, ParsesEmptyArray)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = []");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsArray());
	EXPECT_EQ(value["key"].AsArray().value().get().size(), 0);
}

TEST(TomlReaderArrays, ParsesArrayOfIntegers)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = [1, 2, 3]");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsArray());
	
	EXPECT_EQ(value["key"].AsArray().value().get().size(), 3);
	EXPECT_EQ(value["key"][0].AsNumber().value(), 1);
	EXPECT_EQ(value["key"][1].AsNumber().value(), 2);
	EXPECT_EQ(value["key"][2].AsNumber().value(), 3);
}

TEST(TomlReaderArrays, ParsesArrayWithMixedTypes)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse(R"(key = [1, "two", true])");

	ASSERT_TRUE(reader.IsValid());

	EXPECT_EQ(value["key"][0].AsNumber().value(), 1);
	EXPECT_EQ(value["key"][1].AsString().value().get(), "two");
	EXPECT_EQ(value["key"][2].AsBool().value(), true);
}

TEST(TomlReaderArrays, ParsesNestedArray)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = [[1, 2], [3, 4]]");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"][0][1].AsNumber().value(), 2);
	EXPECT_EQ(value["key"][1][1].AsNumber().value(), 4);
}

TEST(TomlReaderArrays, ParsesArrayWithTrailingComma)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = [1, 2, 3,]");

	ASSERT_TRUE(reader.IsValid());
	ASSERT_TRUE(value["key"].IsArray());
	EXPECT_EQ(value["key"].AsArray().value().get().size(), 3);
}

TEST(TomlReaderArrays, FailsOnArrayMissingCommaBetweenValues)
{
	Marco::TomlReader reader{};
	reader.Parse("key = [1 2]");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderArrays, FailsOnUnclosedArray)
{
	Marco::TomlReader reader{};
	reader.Parse("key = [1, 2");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderInlineTables, ParsesEmptyInlineTable)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse(R"(key = { a = 1, b = "two"})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"]["a"].AsNumber().value(), 1);
	EXPECT_EQ(value["key"]["b"].AsString().value().get(), "two");
}

TEST(TomlReaderInlineTables, ParsesNestedInlineTable)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse(R"(key = { a = { b = 1 } })");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"]["a"]["b"].AsNumber().value(), 1);
}

TEST(TomlReaderInlineTables, FailsOnTrailingCommaInInlineTables)
{
	Marco::TomlReader reader{};
	reader.Parse(R"(key = { a = 1, })");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderTables, ParsesKeyUnderTableHeader)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("[table]\nkey = 1");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["table"]["key"].AsNumber().value(), 1);
}

TEST(TomlReaderTables, ParsesMultipleTables)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("[a]\nx = 1\n\n[b]\ny = 2");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["a"]["x"].AsNumber().value(), 1);
	EXPECT_EQ(value["b"]["y"].AsNumber().value(), 2);
}

TEST(TomlReaderTables, ParsesDottedTableHeader)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("[a.b.c]\nkey = 1");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["a"]["b"]["c"]["key"].AsNumber().value(), 1);
}

TEST(TomlReaderTables, KeysBeforeFirstTableHeaderGoToRoot)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("root_key = 1\n\n[table]\nkey = 2");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["root_key"].AsNumber().value(), 1);
	EXPECT_EQ(value["table"]["key"].AsNumber().value(), 2);
}

TEST(TomlReaderTables, FailsOnDuplicateTableHeader)
{
	Marco::TomlReader reader{};
	reader.Parse("[table]\nkey = 1\n\n[table]\nother = 2");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderTables, FailsOnMalformedTableHeaderMissingClosingBracket)
{
	Marco::TomlReader reader{};
	reader.Parse("[table\nkey = 1");

	EXPECT_FALSE(reader.IsValid());
}

TEST(TomlReaderArrayOfTables, ParsesSingleArrayOfTablesEntry)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("[[items]]\nname = \"first\"");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["items"][0]["name"].AsString().value().get(), "first");
}

TEST(TomlReaderArrayOfTables, ParsesMultipleArrayOfTablesEntries)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("[[items]]\nname = \"first\"\n\n[[items]]\nname = \"second\"");

	ASSERT_TRUE(reader.IsValid());

	EXPECT_EQ(value["items"].AsArray().value().get().size(), 2);
	EXPECT_EQ(value["items"][0]["name"].AsString().value().get(), "first");
	EXPECT_EQ(value["items"][1]["name"].AsString().value().get(), "second");
}

TEST(TomlReaderComments, IgnoresFullLineComment)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("# comment\nkey = 1");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsNumber().value(), 1);
}

TEST(TomlReaderComments, IgnoresTrailingCommentAfterValue)
{
	Marco::TomlReader reader{};
	Marco::Toml value = reader.Parse("key = 1 # trailing comment");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsNumber().value(), 1);
}
