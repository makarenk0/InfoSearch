#include "IndexCompression.h"
#include <iostream>


void IndexCompression::loadCompressedDictionary(const std::string& dictionaryPath)
{
    std::ifstream file(dictionaryPath);
    while (!file.eof()) {
        std::string word;
        std::getline(file, word);
        char byte = word.size();
        _dictionary.append(byte + word);
        ++dictionarySize;
    }
}

int IndexCompression::unpackFrequency(std::vector<char>& postingList)
{
    int frequncy = 0;
    for (int i = 0; ; i++) {
        int frequencyPart = postingList[i]&127;
        frequencyPart = frequencyPart << (i * 7);
        frequncy = frequncy | frequencyPart;
        if (postingList[i+1] & 128) break;
    }
    return frequncy;
}

void IndexCompression::generatePtrTable(const std::string& dictionaryPath, const std::string& postingsPath)
{
    loadCompressedDictionary(dictionaryPath);
    _table = PointersTable(ceil(dictionarySize/4.0));
    std::ifstream file(postingsPath, std::ios::binary);
    std::string::iterator wordPtr = _dictionary.begin();
    for (int i = 0, j = -1; i < dictionarySize-1; i++) {
        if (i % 4 == 0) {
            ++j;
            _table.setPtrTo(j, wordPtr); 
            wordPtr += (unsigned)(*wordPtr) + 1.0;
        }
        std::vector<char> postingList = getPostingListBytes(file);
        _table.setPostingList(j, i % 4, std::vector<char>(postingList.begin() + 1, postingList.end()));
        _table.setTermFrequency(j, i % 4, unpackFrequency(postingList));
    }
}

std::vector<char> IndexCompression::getPostingListBytes(std::ifstream& stream)
{
    std::vector<char> postingList;
    char byte;
    while(true) {
        stream.read(&byte, sizeof(char));
        if (byte == 0) {
            break;
        }
        postingList.push_back(byte);
    }
    return postingList;
}

IndexCompression::IndexCompression()
{
}

void IndexCompression::getEncodedByteArray(std::vector<unsigned char>& byteArr, const std::vector<int>& postingsList)
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
