#include "pch.h"
#include "PositionalIndex.h"


PositionalIndex::PositionalIndex()
{

}

PositionalIndex::PositionalIndex(const std::string& fileName)
{
    readFromFile(fileName);
}

void PositionalIndex::insertWordFromDictionary(std::string& line)
{

    std::string wordBuf = "";
    readWord(line, wordBuf);
    std::string word = wordBuf;
    readWord(line, wordBuf);
    int frequency = std::stoi(wordBuf);
    readWord(line, wordBuf);
    std::map<short, std::set<int>> list;
    frequency_postLists buf = { frequency, list };
    lastInsertedElement = data.insert(data.end(), {word, buf});
}

void PositionalIndex::addNewWord(const std::string& word, const int& frequency, const short& bookNumber, const int& position)
{
    std::set<int> arr;
    arr.insert(position);
    std::map<short, std::set<int>> lists;
    lists.insert({ bookNumber, arr });
    frequency_postLists buf(1, lists);
    std::pair<DataStruct::iterator, bool> foundWord = data.insert({ word, buf });
    if (!foundWord.second) {
        foundWord.first->second.first += 1;
        std::pair<std::map<short, std::set<int>>::iterator, bool> foundInDoc = foundWord.first->second.second.insert({ bookNumber , arr });
        if (!foundInDoc.second) {
            foundInDoc.first->second.insert(position);
        }
    }
}

void PositionalIndex::addNewPosting(const short& docId)
{
    std::set<int> list;
    std::pair< std::map<short, std::set<int>>::iterator, bool> buf = lastInsertedElement->second.second.insert({ docId, list });
    lastInsertedPosting = buf.first;
}

void PositionalIndex::addNewPosition(const int& position)
{
    lastInsertedPosting->second.insert(position);
}

void PositionalIndex::setWordFrequency(const int& frequency)
{
    lastInsertedElement->second.first = frequency;
}

void PositionalIndex::incrementFrequency() {
    ++lastInsertedElement->second.first;
}

void PositionalIndex::printInFile(const std::string& fileName, const int& filesNumber)
{
    std::ofstream out;
    out.open("Index\\" + fileName + ".txt");
    out << filesNumber << std::endl;
    for (auto i : data) {
        out << i.first << " " << i.second.first << " ; ";
        for (auto j : i.second.second) {
            out << j.first << " : ";
            for (auto k : j.second) {
                out << k << " ";
            }
            out << "; ";
        }
        out << std::endl;
    }
    out << std::endl;
    out.close();
}

void PositionalIndex::readFromFile(const std::string& fileName)
{
    std::string lineBuf;
    std::ifstream file("Index\\" + fileName);
    getline(file, lineBuf);
    fileNumber = std::stoi(lineBuf);
    while (true) {
        getline(file, lineBuf);
        if (lineBuf.empty()) {
            break;
        }
        insertWordFromDictionary(lineBuf);
        std::string wordBuf = "";
        for (int i = 0; !lineBuf.empty(); i++) {
            readWord(lineBuf, wordBuf);
            this->addNewPosting((short)(std::stoi(wordBuf)));
            readWord(lineBuf, wordBuf);  //read ":"
            readWord(lineBuf, wordBuf);  //read first number
            for (int j = 0; wordBuf != ";"; j++) {
                this->addNewPosition(std::stoi(wordBuf));
                readWord(lineBuf, wordBuf);
            }
        }
    }
    file.close();
}

bool PositionalIndex::empty()
{
    return data.empty() ? true : false;
}

void  PositionalIndex::readWord(std::string& line, std::string& value) {
    value = line.substr(0, line.find(" "));
    if (line.find(" ") == line.npos) {
        line.clear();
    }
    else {
        line = line.substr(line.find(" ") + 1, line.length());
    }
}

std::map<short, std::set<int>>& PositionalIndex::getPostingsList(const std::string& word)
{
    return data.find(word)->second.second;
}