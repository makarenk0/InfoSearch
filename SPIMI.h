#pragma once
#include "Index.h"
#include <unordered_map>


class SPIMI : Index
{
private:
	std::unordered_map<std::string, std::set<int>> dataBuf;

	void addWordToDictionary(std::unordered_map<std::string, std::set<int>>& index, const std::string& word, const int& value, double& memoryCounter);

	std::pair<std::string, std::set<int>> getLineFromFile(std::ifstream& stream);

	void printInFile(std::map<std::string, std::set<int>>& dictionary, const std::string& indexName);

	void printSortedIndex(std::string& outputPath, int& fileCounter);

	void printInFileLine(std::ofstream& output, std::pair<std::string, std::set<int>>& line);

	double roughRamMapCount();
public:
	SPIMI(const std::string& directoryPath);
	void generateInvertedIndexBySPIMI(const std::string& directoryPath);
	void mergeFiles(const std::string& directoryPath);
};

