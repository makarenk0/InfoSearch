#pragma once
#include <set>
#include <list>
#include <map>
#include <string>
#include <fstream>
class PositionalIndex
{
private:
	typedef std::pair<int, std::map<short, std::set<int>>> frequency_postLists;
	typedef std::map<std::string, frequency_postLists> dataStruct;
	dataStruct data;
	dataStruct::iterator lastInsertedElement;
	std::map<short, std::set<int>>::iterator lastInsertedPosting;
	int fileNumber = 0;
	void readWord(std::string& line, std::string& value);
public:
	PositionalIndex();
	PositionalIndex(const std::string& fileName);
	void insertWordFromDictionary(std::string& word);
	void addNewWord(const std::string& word, const int& frequency, const short& bookNumber, const int& position);
	void addNewPosting(const short& docId);
	void addNewPosition(const int& position);
	void setWordFrequency(const int& frequency);
	void incrementFrequency();
	void printInFile(const std::string& fileName, const int& filesNumber);
	void readFromFile(const std::string& fileName);
	std::map<short, std::set<int>>& getPostingsList(const std::string& word);
};

