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
	struct PostingList {
		int docId = 0;
		mutable std::vector<int> frequencies = { 0 };
		mutable char zoneMask = 0;
		bool operator<(const PostingList& other) const {
			return docId < other.docId;
		}
		bool operator>(const PostingList& other) const {
			return docId > other.docId;
		}
		bool operator==(const PostingList& other) const {
			if (docId == other.docId) {
				frequencies.push_back(other.frequencies[0]);
				zoneMask = zoneMask | other.zoneMask;
				return true;
			}
			return false;
		}
	};


	void enumerateFilesInDir(const std::string& directoryPath);

	void readWord(std::string& line, std::string& value);

	void saveReadWord(const std::string& line, std::string& value);

	std::map<std::string, std::set<short>>::iterator insertMapKey(std::string& lineBuf, std::map<std::string, std::set<short>>& result);

	void saveNumberOfFiles(const std::string& directoryPath);

	std::map<short, std::string> number_filename;

	template<class T> void addWordToDictionary(std::map<std::string, std::set<T>>& index, const std::string& word, const T& value);

	void printInFile(std::map<std::string, std::set<short>>& index, const std::string& indexName);

	template<class T> std::set<T> booleanSearchGeneral(std::string request, std::map<std::string, std::set<T>>& dictionary) {
		std::vector<std::string> requestParts = separateString(request);

		if (requestParts.size() == 1) {
			if (requestParts[0].substr(0, 3) == "NOT") return notSearch(dictionary.find(requestParts[0].substr(4, requestParts[0].length() - 3))->second);
			else {
				std::map<std::string, std::set<T>>::iterator found = dictionary.find(requestParts[0]);
				if (found != dictionary.end()) return found->second;
				return std::set<T>();
			}
		}

		std::stack<std::set<T>> lists;
		lists.push(booleanSearchGeneral(requestParts[0], dictionary));

		for (std::vector<std::string>::iterator i = requestParts.begin() + 1; i != requestParts.end(); i++) {   // AND
			if (*i == "AND") {
				std::set<T> list1 = lists.top();
				lists.pop();
				i += 1;
				lists.push(andSearch(list1, booleanSearchGeneral(*i, dictionary)));

			}
			else if (*i == "OR") {
				i += 1;
				lists.push(booleanSearchGeneral(*i, dictionary));
			}
		}
		while (lists.size() != 1) {   //OR
			std::set<T> list1 = lists.top();
			lists.pop();
			std::set<T> list2 = lists.top();
			lists.pop();
			lists.push(orSearch(list1, list2));
		}
		return lists.top();
	}

	template<class T> std::set<T> andSearch(const std::set<T>& list1, const std::set<T>& list2) {
		std::set<T> result;
		std::set<T>::iterator it1 = list1.begin();
		std::set<T>::iterator it2 = list2.begin();
		while (it1 != list1.end() && it2 != list2.end()) {
			if (*it1 == *it2) {
				result.insert(*it1);
				++it1;
				++it2;
			}
			else if (*it1 < *it2) {
				++it1;
			}
			else {
				++it2;
			}
		}
		return result;
	}

	std::map<std::string, std::set<short>> invertedIndex;  //access for DocVectorBuilder
private:
	std::map<std::string, std::set<short>> biwordIndex;
	std::map<std::string, std::set<std::string>> threegramIndex;
	PositionalIndex positionalIndex;
	Btree* permutermIndex = new Btree;

	void readFromFile(const std::string& fileName, std::map<std::string, std::set<short>>& index);

	std::vector<std::string> separateString(std::string line);

	std::string readInBrackets(std::string& line);

	template<class T> std::set<T> orSearch(const std::set<T>& list1, const std::set<T>& list2) {
		std::set<T> result;
		std::set<T>::iterator it1 = list1.begin();
		std::set<T>::iterator it2 = list2.begin();
		while (it1 != list1.end()) {
			result.insert(*it1);
			++it1;
		}
		while (it2 != list2.end()) {
			result.insert(*it2);
			++it2;
		}
		return result;
	}

	std::set<short> notSearch(const std::set<short>& list1);
	std::set<std::string> notSearch(const std::set<std::string>& list1);
	std::set<PostingList> notSearch(const std::set<PostingList>& list1);

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