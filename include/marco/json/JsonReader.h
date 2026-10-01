#pragma once

#include <cstdint>
#include <string>
#include "marco/json/Json.h"
#include "marco/json/JsonError.h"
#include "marco/json/JsonValue.h"

namespace Marco
{
	class JsonReader
	{
	public:
		JsonReader();

		Json Parse(const std::string&  jsonString);
		Json Parse(std::istream& jsonFile);

		JsonError Error();
		bool      IsValid();

	private:
		static JsonError FormJsonFromString(JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static JsonError FormJsonValue     (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static JsonError HandleNumber      (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleString      (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleBool        (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleNull        (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleArray       (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleEscape      (std::string& value,   const std::string& jsonString, size_t& index); // index is left pointing at the last processed character

		static bool IsHex           (char c);
		static bool IsValueDelimiter(char c);

		static uint16_t ParseHex  (const std::string& jsonString, size_t& index);
		static void     AppendUtf8(std::string& value, uint32_t cp);

		static void MoveIndexUntilNotSpace(const std::string& tomlString, size_t& index);

		JsonError m_error;
		bool      m_valid;
	};
}
