#pragma once

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <stack>

void generateDictionary(const std::string filenames[],const int& fileNumber);

void addWordToDictionary(std::map<std::string, std::set<short>>& dictionary, std::string word, short& bookNumber, int& termsNumber);

void printInFile(std::map<std::string, std::set<short>>& dictionary, const int& allFilesNumber, const int& termsNumber,
    const int& allWordsNumber, const double& collectionSize);

void readDictionary(std::map<std::string, std::set<short>>& result, const std::string dictionaryName);

void readWord(std::string& line, std::string& value);

std::set<short> booleanSearch(std::string request, std::map<std::string, std::set<short>>& dictionary, const int& docNumber);

void andSearch(const std::set<short>& list1, const std::set<short>& list2, std::set<short>& result);

void orSearch(const std::set<short>& list1, const std::set<short>& list2, std::set<short>& result);

void notSearch(const std::set<short>& list1, const int& docNumber, std::set<short>& result);