#include "marco/toml/TomlDate.h"
#include "marco/toml/TomlError.h"
#include "marco/toml/TomlTime.h"
#include "marco/toml/TomlValue.h"
#include <gtest/gtest.h>

TEST(TomlValueConstruction, DefaultConstructorIsEmpty)
{
	Marco::TomlValue value;

	EXPECT_TRUE(value.IsEmpty());
}

TEST(TomlValueConstruction, NullptrConstructorIsEmpty)
{
	Marco::TomlValue value(nullptr);

	EXPECT_TRUE(value.IsEmpty());
}

TEST(TomlValueConstruction, BoolConstructorIsBool)
{
	Marco::TomlValue value(true);

	EXPECT_TRUE(value.IsBool());
}

TEST(TomlValueConstruction, IntConstructorIsNumber)
{
	Marco::TomlValue value(42);

	EXPECT_TRUE(value.IsNumber());
}

TEST(TomlValueConstruction, LongConstructorIsNumber)
{
	Marco::TomlValue value(42L);

	EXPECT_TRUE(value.IsNumber());
}

TEST(TomlValueConstruction, DoubleConstructorIsNumber)
{
	Marco::TomlValue value(3.14);

	EXPECT_TRUE(value.IsNumber());
}

TEST(TomlValueConstruction, StringConstructorIsString)
{
	Marco::TomlValue value(std::string("hello"));

	EXPECT_TRUE(value.IsString());
}

TEST(TomlValueConstruction, CStringConstructorIsString)
{
	Marco::TomlValue value("hello");

	EXPECT_TRUE(value.IsString());
}

TEST(TomlValueConstruction, ArrayConstructorIsArray)
{
	Marco::TomlValue value(Marco::TomlArray{});

	EXPECT_TRUE(value.IsArray());
}

TEST(TomlValueConstruction, ObjectContructorIsObject)
{
	Marco::TomlValue value(Marco::TomlObject{});

	EXPECT_TRUE(value.IsObject());
}

TEST(TomlValueConstruction, DateConstructorIsDate)
{
	Marco::TomlValue value(Marco::TomlDate{});

	EXPECT_TRUE(value.IsDate());
}

TEST(TomlValueConstruction, TimeConstructorIsTime)
{
	Marco::TomlValue value(Marco::TomlTime{});

	EXPECT_TRUE(value.IsTime());
}

TEST(TomlValueAccessors, AsBoolReturnsBalueForBool)
{
	Marco::TomlValue value(true);

	ASSERT_TRUE(value.AsBool().has_value());
	EXPECT_TRUE(value.AsBool().value());
}

TEST(TomlValueAccessors, AsBoolReturnsErrorForNonBool)
{
	Marco::TomlValue value(1.0);

	ASSERT_FALSE(value.AsBool().has_value());
	EXPECT_EQ(value.AsBool().error().errorType, Marco::TomlErrorType::WrongType);
}

TEST(TomlValueAccessors, AsNumberReturnsValueForNumber)
{
	Marco::TomlValue value(1.0);

	ASSERT_TRUE(value.AsNumber().has_value());
	EXPECT_EQ(value.AsNumber().value(), 1.0);
}

TEST(TomlValueAccessors, AsNumberReturnsErrorForNonNumber)
{
	Marco::TomlValue value("text");

	ASSERT_FALSE(value.AsNumber().has_value());
	EXPECT_EQ(value.AsNumber().error().errorType, Marco::TomlErrorType::WrongType);
}

TEST(TomlValueAccessors, AsStringReturnsValueForString)
{
	Marco::TomlValue value("text");

	ASSERT_TRUE(value.AsString().has_value());
	EXPECT_EQ(value.AsString().value().get(), "text");
}

TEST(TomlValueAccessors, AsStringReturnsErrorForNonString)
{
	Marco::TomlValue value(1);

	ASSERT_FALSE(value.AsString().has_value());
	EXPECT_EQ(value.AsString().error().errorType, Marco::TomlErrorType::WrongType);
}

TEST(TomlValueAccessors, AsObjectReturnsValueForObject)
{
	Marco::TomlObject obj;
	obj["key"] = 1;
	
	Marco::TomlValue value(obj);
	
	ASSERT_TRUE(value.AsObject().has_value());
	EXPECT_EQ(value.AsObject().value().get().size(), 1);
}

TEST(TomlValueAccessors, AsObjectReturnsErrorForNonObject)
{
	Marco::TomlValue value(1);

	ASSERT_FALSE(value.AsArray().has_value());
	EXPECT_EQ(value.AsArray().error().errorType, Marco::TomlErrorType::WrongType);
}

TEST(TomlValueAccessors, AsDateReturnsValueForData)
{
	Marco::TomlValue value(Marco::TomlDate{});

	EXPECT_TRUE(value.AsDate().has_value());
}

TEST(TomlValueAccessors, AsDateReturnsErrorForNonDate)
{
	Marco::TomlValue value(1);

	ASSERT_FALSE(value.AsDate().has_value());
	EXPECT_EQ(value.AsDate().error().errorType, Marco::TomlErrorType::WrongType);
}

TEST(TomlValueAccessors, AsTimeReturnsValueForTime)
{
	Marco::TomlValue value(Marco::TomlTime{});

	EXPECT_TRUE(value.AsTime().has_value());
}

TEST(TomlValueAccessors, AsTimeReturnsErrorForNonTime)
{
	Marco::TomlValue value(1);

	ASSERT_FALSE(value.AsTime().has_value());
	EXPECT_EQ(value.AsTime().error().errorType, Marco::TomlErrorType::WrongType);
}

TEST(TomlValuePushBack, PushBackOnEmptyValueCreatesArray)
{
	Marco::TomlValue value;
	value.PushBack(1);

	ASSERT_TRUE(value.IsArray());
	EXPECT_EQ(value.AsArray().value().get().size(), 1);
}

TEST(TomlValuePushBack, PushBackAppendsMultipleValues)
{
	Marco::TomlValue value;
	value.PushBack(1);
	value.PushBack(2);

	ASSERT_TRUE(value.IsArray());
	EXPECT_EQ(value.AsArray().value().get().size(), 2);
	EXPECT_EQ(value[0].AsNumber().value(), 1);
	EXPECT_EQ(value[1].AsNumber().value(), 2);
}

TEST(TomlValuePushBack, PushBackReturnsReferenceToAppendedValue)
{
	Marco::TomlValue value;
	Marco::TomlValue& ref = value.PushBack(5);

	EXPECT_EQ(ref.AsNumber().value(), 5);
}

TEST(TomlValueArrayIndexOperator, IndexOperatorOnEmptyValueCreatesArray)
{
	Marco::TomlValue value;
	value[0] = 1;

	ASSERT_TRUE(value.IsArray());
	EXPECT_EQ(value[0].AsNumber().value(), 1);
}

TEST(TomlValueArrayIndexOperator, IndexOperatorResizesArrayWhenIdexOutOfBounds)
{
	Marco::TomlValue value;
	value[2] = 1;
	
	ASSERT_TRUE(value.IsArray());

	EXPECT_EQ(value.AsArray().value().get().size(), 3);
	EXPECT_TRUE(value[0].IsEmpty());
	EXPECT_TRUE(value[1].IsEmpty());
	EXPECT_EQ(value[2].AsNumber().value(), 1);
}

TEST(TomlValueConstArrayIndexOperator, ReturnsValueWhenIndexInBounds)
{
	const Marco::TomlValue value = Marco::TomlArray{1, 2};

	auto result = value[0];
	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(result.value().get().AsNumber().value(), 1);
}

TEST(TomlValueConstArrayIndexOperator, ReturnsErrorWhenIndexOutOfBounds)
{
	const Marco::TomlValue value = Marco::TomlArray{1};

	auto result = value[5];
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::TomlErrorType::InvalidFormat);
}

TEST(TomlValueConstArrayIndexOperator, ReturnsErrorWhenValueIsNotArray)
{
	const Marco::TomlValue value(1);

	auto result = value[0];
	EXPECT_FALSE(result.has_value());
}

TEST(TomlValueObjectIndexOperator, IndexOperatorOnEmptyValueCreatesObject)
{
	Marco::TomlValue value;
	value["key"] = 1;

	ASSERT_TRUE(value.IsObject());
	EXPECT_EQ(value["key"].AsNumber().value(), 1);
}

TEST(TomlValueObjectIndexOperator, IndexOperatorCreatesEmptyValueForNewKey)
{
	Marco::TomlValue value;
	Marco::TomlValue& ref = value["key"];

	EXPECT_TRUE(ref.IsEmpty());
}

TEST(TomlValueConstObjectIndexOperator, ReturnsValueWhenKeyExists)
{
	Marco::TomlObject obj;
	obj["key"] = 1;
	const Marco::TomlValue value(obj);

	auto result = value["key"];
	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(result.value().get().AsNumber().value(), 1);
}

TEST(TomlValueConstObjectIndexOperator, ReturnsErrorWhenKeyMissing)
{
	const Marco::TomlValue value = Marco::TomlObject{};

	auto result = value["missing"];
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::TomlErrorType::InvalidFormat);
}

TEST(TomlValueConstObjectIndexOperator, ReturnsErrorWhenValueIsNotObject)
{
	const Marco::TomlValue value(1);

	auto result = value["key"];
	EXPECT_FALSE(result.has_value());
}
