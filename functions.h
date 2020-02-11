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

enum binOperations { AND, OR };

std::vector<std::string> filesInDir(const std::string& directoryPath);

void invertedIndex(const std::string& directoryPath);

void biwordIndex(const std::string& directoryPath);

void positionalIndex(const std::string& directoryPath);

void permutermIndex(Btree* tree, std::map<std::string, const std::set<short>>& invertedIndex);

void threeGramIndexGen(std::map<std::string, std::set<std::string>>& index, const std::map<std::string, std::set<short>>& invertedIndex);

template<class T> void addWordToDictionary(std::map<std::string, std::set<T>>& dictionary, const std::string& word, T& value);

void printInFile(std::map<std::string, std::set<short>>& dictionary, const int& allFilesNumber, const std::string& dicName);

void readDictionary(std::map<std::string, std::set<short>>& result, const std::string dictionaryName, int& filesNumber);

std::map<std::string, std::set<short>>::iterator insertMapKey(std::string& lineBuf, std::map<std::string, std::set<short>>& result);

void readWord(std::string& line, std::string& value);

void saveReadWord(const std::string& line, std::string& value);

template<class T> std::set<T> booleanSearch(std::string request, std::map<std::string, std::set<T>>& dictionary, const int& docNumber, const bool& biword = false);

std::set<short> phraseSearch(std::string request, std::map<std::string, std::set<short>>& dictionary, const int& docNumber);

std::set<short> positionalSearch(std::string request, PositionalIndex& dictionary);

void sortBinOperations(std::stack<std::set<short>>& lists, std::stack<binOperations>& operations, int currentSize);

template<class T> void andSearch(const std::set<T>& list1, const std::set<T>& list2, std::set<T>& result);

template<class T> void orSearch(const std::set<T>& list1, const std::set<T>& list2, std::set<T>& result);

void notSearch(const std::set<short>& list1, const int& docNumber, std::set<short>& result);

void notSearch(const std::set<std::string>& list1, const int& docNumber, std::set<std::string>& result);

std::map<short, std::set<int>> positionalIntersect(std::map<short, std::set<int>>& list1, std::map<short, std::set<int>>& list2, const int& k);

std::map<short, std::set<int>> positionalIntersect(std::map<short, std::set<int>>& list1, std::map<short, std::set<int>>& list2);

std::map<std::string, std::set<short>> postWildcardQuery(std::string word);

std::map<std::string, std::set<short>> freeWildcardQueryPermuterm(std::string word);

std::map<std::string, std::set<short>> freeWildcardQuery3Gramm(std::string word);

