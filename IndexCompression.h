#pragma once
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <tuple>
#include "PointersTable.h"

class IndexCompression
{
private:
	std::string _dictionary;
	PointersTable _table;
	int dictionarySize = 0;
	std::vector<char> getPostingListBytes(std::ifstream& stream);
	void loadCompressedDictionary(const std::string& dictionaryPath);
	int unpackFrequency(std::vector<char>& postingList);
public:
	IndexCompression();
	void getEncodedByteArray(std::vector<unsigned char>& byteArr, const std::vector<int>& postingsList);
	void generatePtrTable(const std::string& dictionaryPath, const std::string& postingsPath);
};

