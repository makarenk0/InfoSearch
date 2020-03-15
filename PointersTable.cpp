#include "PointersTable.h"
#include <iostream>

PointersTable::PointersTable()
{
}

PointersTable::PointersTable(const int& size)
{
	rowArray.reserve(size);
}

PointersTable::~PointersTable()
{
}

void PointersTable::setPtrTo(const int& elementN, std::string::iterator ptr)
{
	Row empty;
	empty.wordPtr = ptr;
	rowArray.push_back(empty);
}

void PointersTable::setPostingList(const int& elementN, const int& numberInBlock, std::vector<char> bytes)
{
	 rowArray[elementN].postingList[numberInBlock].assign(bytes.begin(), bytes.end());	
}

void PointersTable::setTermFrequency(const int& elementN, const int& numberInBlock, const int& frequency)
{
	rowArray[elementN].frequncy[numberInBlock] = frequency;
}
