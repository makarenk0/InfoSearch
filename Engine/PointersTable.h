#pragma once
#include <tuple>
#include <string>
#include <vector>
class PointersTable
{
private:
	struct Row {
		std::string::iterator wordPtr;
		std::vector<char> postingList[4];
		int frequncy[4];
	};
	std::vector<Row> rowArray;
public:
	PointersTable();
	PointersTable(const int& size);
	~PointersTable();
	void setPtrTo(const int& elementN, std::string::iterator ptr);
	void setPostingList(const int& elementN, const int& numberInBlock, std::vector<char> bytes);
	void setTermFrequency(const int& elementN, const int& numberInBlock, const int& frequency);
};

