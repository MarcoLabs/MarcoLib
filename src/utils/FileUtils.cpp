#include "marco/utils/FileUtils.h"

#include <fstream>
#include <filesystem>
#include <sstream>
#include <stdexcept>


std::string Marco::ReadFile(const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("ReadFile: failed to open file: " + filepath);
	}

	std::error_code ec;
	auto size = std::filesystem::file_size(filepath, ec);

	if (ec)
	{
		throw std::runtime_error("ReadFile: failed to get size of file: " + filepath + " (" + ec.message() + ")");
	}

	std::string content(size, '\0');
	file.read(content.data(), static_cast<std::streamsize>(size));

	if (!file && !file.eof())
	{
		throw std::runtime_error("ReadFile: failed while reading file: " + filepath);
	}

	return content;
}

std::string Marco::ReadFile(std::istream& file)
{
	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

void Marco::WriteFile(const std::string& filePath, const std::string& content)
{
	std::ofstream file(filePath);

	file << content;

	file.close();
}

void Marco::WriteFile(const std::filesystem::path& filePath, const std::string& content)
{
	std::ofstream file(filePath);

	file << content;

	file.close();
}
