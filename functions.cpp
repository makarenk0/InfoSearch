#include "functions.h"


void generateDictionary(const std::string filenames[], const int& fileNumber) {
    int termsNumber = 0;
    int allWordsNumber = 0;
    double collectionSize = 0;
    double dictionarySize = 0;

    std::map<std::string, std::set<short>> dictionary;
    std::ifstream file;

   
    for (short i = 0; i < fileNumber; i++) {
        file.open("Books\\" + filenames[i] + ".txt");
        std::string word = "";
        char buf;
        while (!file.eof()) {
            buf = file.get();
            collectionSize += 1;
            if (buf < 65 || buf>122) {
                if (word != "") {
                    addWordToDictionary(dictionary, word, i, termsNumber);
                    word = "";
                    ++allWordsNumber;
                }
            }
            else {
                word += buf;
            }
        }
        file.close();
    }
   
    
    printInFile(dictionary, fileNumber, termsNumber, allWordsNumber, collectionSize);
}

void addWordToDictionary(std::map<std::string, std::set<short>>& dictionary, std::string word, short& bookNumber, int& termsNumber) {
    std::set<short> arr;
    arr.insert(bookNumber);
    std::pair<std::map<std::string, std::set<short>>::iterator, bool> found = dictionary.insert({ word, arr });
    if (!found.second) {
        found.first->second.insert(bookNumber);
    }
    else {
        ++termsNumber;
    }
}

void printInFile(std::map<std::string, std::set<short>>& dictionary, const int& allFilesNumber, const int& termsNumber,
    const int& allWordsNumber, const double& collectionSize) {

    std::ofstream out;
    out.open("Books\\result.txt");
    for (auto i : dictionary) {
        out << i.first;
        for (auto j : i.second) {
            out << " " << j;
        }
        out << std::endl;
    }
    out << std::endl;
    out << "Number of words in whole collection: " << allWordsNumber << std::endl;
    out << "Number of terms in dictionary: " << termsNumber << std::endl;
    out << "Collection size in kilobytes: " << collectionSize / 1000 << std::endl;
    out.close();
}

void readDictionary(std::map<std::string, std::set<short>>& result, const std::string dictionaryName)
{
   
    std::string lineBuf;
    std::ifstream file("Books\\"+ dictionaryName);
    getline(file, lineBuf);
    while (lineBuf!="") {
        std::string wordBuf = "";
        std::map<std::string, std::set<short>>::iterator currentElement;
        for (int i = 0; lineBuf != wordBuf; i++) {
            readWord(lineBuf, wordBuf);
            if (i == 0) {
                currentElement = result.insert(result.end(), { wordBuf , std::set<short>() });
            }
            else {
                currentElement->second.insert(std::stoi(wordBuf));
            }
        }
        getline(file, lineBuf);
    }
    file.close();
}

void readWord(std::string& line, std::string& value) {
    value = line.substr(0, line.find(" "));
    line = line.substr(line.find(" ")+1, line.length());
}

std::set<short> booleanSearch(std::string request, std::map<std::string, std::set<short>>& dictionary, const int& docNumber)
{
    std::string wordBuf = "";

    std::set<short> word1List;
    std::set<short> word2List;
    std::set<short> result;


    std::stack<std::string> operations;
    std::stack<std::set<short>> lists;


    readWord(request, wordBuf);
    while (true) {
        readWord(request, wordBuf);
        if (wordBuf == "(") {
            continue;
        }
        else if (wordBuf == ")") {
            if (operations.top()=="AND") {
                word1List = lists.top();
                lists.pop();
                word2List = lists.top();
                lists.pop();
                andSearch(word1List, word2List, result);
                lists.push(result);
   
            }
            operations.pop();
            if (operations.size() == 0) {
                break;
            }
            result.clear();
        }
        else {
            if (wordBuf == "NOT") {
                readWord(request, wordBuf);
                word1List = dictionary.find(wordBuf)->second;
                notSearch(word1List, docNumber, result);
                lists.push(result);
                result.clear();
            }
            else {
                if (wordBuf == "AND") {
                    operations.push(wordBuf);
                }
                else {
                    word1List = dictionary.find(wordBuf)->second;
                    lists.push(word1List);
                }
            }
        }
        
        }
    return lists.top();

}

void andSearch(const std::set<short>& list1, const std::set<short>& list2, std::set<short>& result) {
    std::set<short>::iterator it1 = list1.begin();
    std::set<short>::iterator it2 = list2.begin();
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
}

void orSearch(const std::set<short>& list1, const std::set<short>& list2, std::set<short>& result) {
    std::set<short>::iterator it1 = list1.begin();
    std::set<short>::iterator it2 = list2.begin();
    while (it1 != list1.end()) {
            result.insert(*it1);
            ++it1;
    }
    while (it2 != list2.end()) {
        result.insert(*it2);
        ++it2;
    }
}

void notSearch(const std::set<short>& list1, const int& docNumber, std::set<short>& result) {
    std::set<short>::iterator it1 = list1.begin();
    for (int i = 0; i < docNumber; i++) {
        if (*it1 != i) {
            result.insert(i);
            
        }
        else {
            ++it1;
        }
    }
}
