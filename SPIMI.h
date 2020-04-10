#pragma once
#include "IndexCompression.h"
#include "Index.h"
#include <unordered_map>
#include <windows.h>


class SPIMI : Index
{
private:
	IndexCompression _compressor;
	const std::string compressedDictionaryPath = "Index\\SPIMI\\Compressed\\MergedDictionary.txt";
	const std::string compressedPostingListsPath = "Index\\SPIMI\\Compressed\\MergedPostings.bin";

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

public:
	SPIMI(const std::string& directoryPath);
	void generateInvertedIndexBySPIMI(const std::string& directoryPath);
	IndexCompression& getCompressor();
	
};

