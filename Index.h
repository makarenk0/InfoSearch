#pragma once

#include <iostream>
#include <fstream>
#include <set>
#include <list>
#include <map>
#include <string>
#include <stack>
#include <vector>
#include <windows.h>
#include "PositionalIndex.h"
#include "Btree.h"


class Index {
protected:

	void enumerateFilesInDir(const std::string& directoryPath);

	void readWord(std::string& line, std::string& value);

	void saveReadWord(const std::string& line, std::string& value);

	std::map<std::string, std::set<short>>::iterator insertMapKey(std::string& lineBuf, std::map<std::string, std::set<short>>& result);

	void saveNumberOfFiles(const std::string& directoryPath);

	std::map<short, std::string> number_filename;

	template<class T> void addWordToDictionary(std::map<std::string, std::set<T>>& index, const std::string& word, const T& value);

	void printInFile(std::map<std::string, std::set<short>>& index, const std::string& indexName);

private:
	std::map<std::string, std::set<short>> invertedIndex;
	std::map<std::string, std::set<short>> biwordIndex;
	std::map<std::string, std::set<std::string>> threegramIndex;
	PositionalIndex positionalIndex;
	Btree* permutermIndex = new Btree;

	void readFromFile(const std::string& fileName, std::map<std::string, std::set<short>>& index);

	std::vector<std::string> separateString(std::string line);

	std::string readInBrackets(std::string& line);

	template<class T> std::set<T> booleanSearchGeneral(std::string request, std::map<std::string, std::set<T>>& dictionary);

	template<class T> std::set<T> andSearch(const std::set<T>& list1, const std::set<T>& list2);

	template<class T> std::set<T> orSearch(const std::set<T>& list1, const std::set<T>& list2);

	std::set<short> notSearch(const std::set<short>& list1);
	std::set<std::string> notSearch(const std::set<std::string>& list1);

	std::map<short, std::set<int>> positionalIntersect(std::map<short, std::set<int>>& list1, std::map<short, std::set<int>>& list2, const int& k);
	std::map<short, std::set<int>> positionalIntersect(std::map<short, std::set<int>>& list1, std::map<short, std::set<int>>& list2);

public:
	enum class IndexName {inverted, biword, positional};

	Index(const std::string& directoryPath);
	Index();

	void generateInvertedIndex(const std::string& directoryPath);
	void generateBiwordIndex(const std::string& directoryPath);
	void generatePositionalIndex(const std::string& directoryPath);
	void generatePermutermIndex();
	void generateThreegramIndex();

	void loadIndex(IndexName name);
	

	std::set<short> booleanSearch(const std::string& request);
	std::set<short> phraseSearch(std::string request);
	std::set<short> positionalSearch(std::string request);

	
	std::map<std::string, std::set<short>> postWildcardQuery(std::string word);

	std::map<std::string, std::set<short>> freeWildcardQueryPermuterm(std::string word);

	std::map<std::string, std::set<short>> freeWildcardQuery3Gramm(std::string word);

};