#pragma once
#include <map>
#include <set>
#include <string>
#include <list>

class Btree
{
private:
	std::map<char, Btree*> nextNodes;
public:
	Btree();
	~Btree();
	Btree* getNextNode(const char& symbol);
	Btree* addNewNode(const char& symbol);
	const std::map<char, Btree*>& getNodesList();
};

std::list<std::string> getWords(Btree* ptr, const std::string& toFind);

void addWord(Btree* ptr, const std::string& toAdd);

