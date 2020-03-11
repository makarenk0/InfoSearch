#include "IndexCompression.h"


IndexCompression::IndexCompression()
{
}

void IndexCompression::getEncodedByteArray(std::vector<unsigned char>& byteArr, const std::set<int>& postingsList)
{
    for (auto i : postingsList) {
        int docId = i;
        for (int j = 0; j < 4 && docId != 0; j++) {
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
}
