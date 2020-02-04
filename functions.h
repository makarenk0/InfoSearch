#pragma once

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <stack>
#include <vector>
#include <windows.h>

typedef std::map<std::string, std::pair<int, std::map<int , std::set<int>>>> PositionalIndex;

enum binOperations { AND, OR };

std::vector<std::string> filesInDir(const std::string& directoryPath);

void invertedIndex(const std::string& directoryPath);

void biwordIndex(const std::string& directoryPath);

void positionalIndex(const std::string& directoryPath);

void addWordToDictionary(std::map<std::string, std::set<short>>& dictionary, std::string word, short& bookNumber, int& termsNumber);

void addWordToDictionary(std::map<std::string, std::set<short>>& dictionary, std::string word, short& bookNumber);

void printInFile(std::map<std::string, std::set<short>>& dictionary, const int& allFilesNumber, const std::string& dicName);

void readDictionary(std::map<std::string, std::set<short>>& result, const std::string dictionaryName, int& filesNumber);

std::map<std::string, std::set<short>>::iterator insertMapKey(std::string& lineBuf, std::map<std::string, std::set<short>>& result);

void readWord(std::string& line, std::string& value);

std::set<short> booleanSearch(std::string request, std::map<std::string, std::set<short>>& dictionary, const int& docNumber, const bool& biword = false);

std::set<short> phraseSearch(std::string request, std::map<std::string, std::set<short>>& dictionary, const int& docNumber);

void sortBinOperations(std::stack<std::set<short>>& lists, std::stack<binOperations>& operations, int currentSize);

void andSearch(const std::set<short>& list1, const std::set<short>& list2, std::set<short>& result);

void orSearch(const std::set<short>& list1, const std::set<short>& list2, std::set<short>& result);

void notSearch(const std::set<short>& list1, const int& docNumber, std::set<short>& result);