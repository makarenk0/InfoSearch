#pragma once
#include <vector>
#include <set>
#include <string>

class IndexCompression
{
private:
	std::string dictionary;
	
public:
	IndexCompression();
	std::vector<char> getEncodedByteArray(const std::set<int>& postingsList);
};

