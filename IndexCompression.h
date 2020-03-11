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
	void getEncodedByteArray(std::vector<unsigned char>& byteArr, const std::set<int>& postingsList);
};

