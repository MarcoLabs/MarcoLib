#pragma once

#include <string>
#include <cstddef>

namespace Marco
{
	enum class JsonErrorType
	{
		InvalidFormat,
		WrongType,
		NumberParseError,
		NumberOutOfRange,
		KeyNotFound,
		NoError
	};

	struct JsonError
	{
	    std::string ToString() const;

		JsonErrorType errorType;
		std::size_t   index;
	};
}
