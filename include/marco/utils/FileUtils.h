#pragma once

#include <string>
#include <iosfwd>
#include <filesystem>


namespace Marco
{
	std::string ReadFile(const std::string&           filepath);
	std::string ReadFile(std::istream&                file);
	std::string ReadFile(const std::filesystem::path& filepath);
	
	void WriteFile(const std::string& filePath,           const std::string& content);
	void WriteFile(const std::filesystem::path& filePath, const std::string& content);
}
