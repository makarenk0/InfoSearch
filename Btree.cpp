#include "Btree.h"

Btree::Btree()
{
}

Btree::~Btree()
{
	for (auto i : nextNodes) {
		delete i.second;
	}
}


Btree* Btree::getNextNode(const char& symbol)
{
	std::map<char, Btree*>::iterator it = nextNodes.find(symbol);
	if (it == nextNodes.end()) {
		return nullptr;
	}
	return it->second;
}

Btree* Btree::addNewNode(const char& symbol)
{
	std::pair<std::map<char, Btree*>::iterator, bool> inserted = nextNodes.insert({ symbol, new Btree() });
	return inserted.first->second;
}

const std::map<char, Btree*>& Btree::getNodesList()
{
	return nextNodes;
}

std::list<std::string> getWords(Btree* ptr, const std::string& toFind)
{
	std::list<std::string> result;
	for (int i = 0; i < toFind.length(); i++) {
		ptr = ptr->getNextNode(toFind.at(i));
		if (ptr == nullptr) {
			return result;  //no such word
		}
	}
	for (auto i : ptr->getNodesList()) {
		result.push_back(toFind + i.first);
	}
	return result;
}

void addWord(Btree* ptr, const std::string& toAdd)
{
	Btree* search = ptr;
	for (int i = 0; i < toAdd.length(); i++) {
		search = ptr->getNextNode(toAdd.at(i));
		if (search == nullptr) {
			ptr = ptr->addNewNode(toAdd.at(i));
		}
		else {
			ptr = search;
		}
	}
}
