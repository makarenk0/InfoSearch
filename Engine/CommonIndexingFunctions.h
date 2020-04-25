#pragma once
#include <string>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;

class CommonIndexingFunctions
{
public:
	static unsigned long long enumerateFilesInDir(const std::string& directoryPath, std::map<short, fs::directory_entry>& number_filename);
};

