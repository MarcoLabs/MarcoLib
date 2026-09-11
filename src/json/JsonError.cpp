#include "marco/json/JsonError.h"
#include <format>

std::string Marco::JsonError::ToString() const
{
	std::string errorType;

	switch (this->errorType)
	{
		case JsonErrorType::InvalidFormat:
			errorType = "InvalidFormat";
			break;
		case Marco::JsonErrorType::WrongType:
			errorType = "WrongType";
			break;
		case Marco::JsonErrorType::NumberParseError:
			errorType = "NumberParseError";
			break;
		case Marco::JsonErrorType::NumberOutOfRange:
			errorType = "NumberOutOfRange";
			break;
		case Marco::JsonErrorType::KeyNotFound:
			errorType = "KeyNotFound";
			break;
		case Marco::JsonErrorType::NoError:
			errorType = "NoError";
			break;
	}

	return std::format(
		"Error occured: {} at index: {}", errorType, this->index
	);
}
