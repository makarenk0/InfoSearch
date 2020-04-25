#pragma once
#include "IndexCompression.h"
#include <unordered_map>
#include <windows.h>
#include <map>
#include <iostream>
#include "CommonIndexingFunctions.h"


class SPIMI
{
private:

	IndexCompression _compressor;
	std::string compressedDictionaryPath;
	std::string compressedPostingListsPath;
	const std::string& _savingPath;

	std::unordered_map<std::string, std::set<int>> _dataBuf;

	const double oneBlockMemory = 100000000; // 100 mb for clear data in RAM (the struct could take about 3 gb)

	double charsNumber = 0, wordsNumber = 0;

	void addWordToDictionary(const std::string& word, const int& value, double& memoryCounter);

	std::pair<std::string, std::set<int>> getLineFromFile(std::ifstream& stream);

	void printInFile(std::map<std::string, std::set<int>>& dictionary, const std::string& indexName);

	void printSortedIndex(std::string& outputPath, int& fileCounter);

	void printPostingList(std::ofstream& output, const std::set<int>& postingList);

	void printWordInDictionary(std::ofstream& output, const std::string& word);

	void mergeFiles(const std::string& directoryPath);

	void readWord(std::string& line, std::string& value);

	std::map<short, fs::directory_entry>& _number_filename;
public:
	SPIMI(const std::string& savingPath, std::map<short, fs::directory_entry>& number_filename);
	void generateInvertedIndexBySPIMI(unsigned long long& _bytesToIndexLeft);
	IndexCompression& getCompressor();
	
};

