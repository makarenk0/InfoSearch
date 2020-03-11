#include "IndexCompression.h"


IndexCompression::IndexCompression()
{
}

std::vector<char> IndexCompression::getEncodedByteArray(const std::set<int>& postingsList)
{
	std::vector<char> byteArr;
	for (auto i : postingsList) {
		int docId = i;
		for (int j = 0; j < 4; j++) {
			unsigned char byte = docId & 127;
			if (j == 0) {
				byte |= (1 << 7);
			}
			else {
				byte &= ~(1 << 7);
			}
			byteArr.push_back(byte);
			docId = docId >> 7;
		}

	}
	return byteArr;
}
