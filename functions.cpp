#include "functions.h"



std::vector<std::string> filesInDir(const std::string& directoryPath) 
{
    WIN32_FIND_DATA FindFileData = {0};
    HANDLE hFind;
    setlocale(LC_ALL, "");

    std::wstring stemp = std::wstring(directoryPath.begin(), directoryPath.end());
    stemp += L"*.txt";
    LPCWSTR path = stemp.c_str();
   
    std::vector<std::string> fileNames;
   
    hFind = FindFirstFile(path, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        throw "Folder 'Books' is empty";
    }
    else {
        std::wstring ws(FindFileData.cFileName);  //TO DO: reduce the code
        fileNames.push_back(std::string(ws.begin(), ws.end()));
    }
    while(FindNextFile(hFind, &FindFileData) != 0) {
        std::wstring ws(FindFileData.cFileName);
        fileNames.push_back(std::string(ws.begin(), ws.end()));
    }
    return fileNames;
}

void invertedIndex(const std::string& directoryPath) {
    std::vector<std::string> fileNames = filesInDir(directoryPath);
    int fileNumber = fileNames.size();

    std::map<std::string, std::set<short>> dictionary;
    std::ifstream file;

    for (short i = 0; i < fileNumber; i++) {
        file.open(directoryPath + fileNames[i]);
        std::string word = "";
        char buf;
        while (!file.eof()) {
            buf = file.get();
     
            if (buf < 65 || buf>122) {
                if (word != "") {
                    addWordToDictionary(dictionary, word, i);
                    word = "";
                }
            }
            else {
                word += buf;
            }
        }
        file.close();
    }
    printInFile(dictionary, fileNumber, "invertedIndex");
}

void biwordIndex(const std::string& directoryPath)
{
    std::vector<std::string> fileNames = filesInDir(directoryPath);
    int fileNumber = fileNames.size();

    std::map<std::string, std::set<short>> dictionary;
    std::ifstream file;

    for (short i = 0; i < fileNumber; i++) {
        file.open(directoryPath + fileNames[i]);
        std::string word = "", previousWord = "";
        bool firstWord = true;
        char buf;
        while (!file.eof()) {
            buf = file.get();
            if (buf < 65 || buf>122) {
                if (!word.empty()) {
                    if (firstWord) {
                        firstWord = false;
                    }
                    else {
                        addWordToDictionary(dictionary, previousWord + " " + word, i);
                    }
                    previousWord = word;
                    word = "";
                }
            }
            else {
                word += buf;
            }
        }
        file.close();
    }
    printInFile(dictionary, fileNumber, "biwordIndex");
}

void positionalIndex(const std::string& directoryPath)
{
    std::vector<std::string> fileNames = filesInDir(directoryPath);
    int fileNumber = fileNames.size();

    PositionalIndex dictionary;
    std::ifstream file;
   

    for (short i = 0; i < fileNumber; i++) {
        file.open(directoryPath + fileNames[i]);
        std::string word = "";
        int position = 0;
        char buf;
        while (!file.eof()) {
            buf = file.get();

            if (buf < 65 || buf>122) {
                if (word != "") {
                    dictionary.addNewWord(word, 1, i, position);
                    ++position;
                    word = "";
                }
            }
            else {
                word += buf;
            }
        }
        file.close();
    }
    dictionary.printInFile("positionalIndex", fileNumber);
}

void permutermIndex(Btree* tree, std::map<std::string, std::set<short>>& invertedIndex)
{
    for (auto i : invertedIndex) { //iteration through map keys
        std::string key = i.first;
        std::string word = key + '$';
        addWord(tree, word);
        for (int j = 0; j < key.length(); j++) {  //generating all permutations 
            word = word + word.at(0);
            word = word.substr(1, word.length());
            addWord(tree, word);
        }
    }
}

void addWordToDictionary(std::map<std::string, std::set<short>>& dictionary, const std::string& word, short& bookNumber) {
    std::set<short> arr;
    arr.insert(bookNumber);
    std::pair<std::map<std::string, std::set<short>>::iterator, bool> found = dictionary.insert({ word, arr });
    if (!found.second) {
        found.first->second.insert(bookNumber);
    }
}

void printInFile(std::map<std::string, std::set<short>>& dictionary, const int& allFilesNumber, const std::string& dicName) {
    std::ofstream out;
    out.open("Dictionaries\\"+dicName+".txt");
    out << allFilesNumber << std::endl;
    for (auto i : dictionary) {
        out << i.first <<" ";
        for (auto j : i.second) {
            out << " " << j;
        }
        out << std::endl;
    }
    out << std::endl;
    out.close();
}

void readDictionary(std::map<std::string, std::set<short>>& result, const std::string dictionaryName, int& filesNumber)
{
    std::string lineBuf;
    std::ifstream file("Dictionaries\\"+ dictionaryName);
    getline(file, lineBuf);
    filesNumber = std::stoi(lineBuf);
    while (true) {
        getline(file, lineBuf);
        if (lineBuf.empty()) {
            break;
        }
        std::map<std::string, std::set<short>>::iterator currentElement = insertMapKey(lineBuf, result);

        std::string wordBuf = "";
        for (int i = 0; !lineBuf.empty(); i++) {
            readWord(lineBuf, wordBuf);
            currentElement->second.insert(std::stoi(wordBuf));
        }
    }
    file.close();
}

std::map<std::string, std::set<short>>::iterator insertMapKey(std::string& lineBuf, std::map<std::string, std::set<short>>& result)
{
        std::string wordBuf = "";
        readWord(lineBuf, wordBuf);
        std::string wordLine = wordBuf;
        readWord(lineBuf, wordBuf);
        if (!wordBuf.empty()) {   // if we read biword index
            wordLine += " ";
            wordLine += wordBuf;
            lineBuf = lineBuf.substr(1, lineBuf.length());
        }
        return result.insert(result.end(), { wordLine , std::set<short>() });  //return iterator on inserted element
}

void readWord(std::string& line, std::string& value) {
    value = line.substr(0, line.find(" "));
    if (line.find(" ") == line.npos) {
        line.clear();
    }
    else {
        line = line.substr(line.find(" ") + 1, line.length());
    }
}

void saveReadWord(const std::string& line, std::string& value) {
    value = line.substr(0, line.find(" "));
}

std::set<short> booleanSearch(std::string request, std::map<std::string, std::set<short>>& dictionary, const int& docNumber, const bool& biword)
{
    std::string wordBuf = "";

    std::set<short> word1List;  //buffers
    std::set<short> word2List;
    std::set<short> result;
    
    std::stack<binOperations> operations;
    std::stack<std::set<short>> lists;

    int bracketCounter = 1;
    int currentSize = 0;
    bool end = false;
    readWord(request, wordBuf);
    if (wordBuf != "(") {
        throw "Invalid form of query!";
    }
    while (!end) {
        readWord(request, wordBuf);
        if (wordBuf == "(") {
            ++bracketCounter;
            currentSize = lists.size();
            continue;
        }
        else if (wordBuf == ")"||request.empty()) {
            if(bracketCounter) --bracketCounter;
            word1List = lists.top();
            lists.pop();
            word2List = lists.top();
            lists.pop();
            switch (operations.top()) {
            case AND:
                andSearch(word1List, word2List, result);
                lists.push(result);
                break;
            case OR:
                orSearch(word1List, word2List, result);
                lists.push(result);
                break;
            }
            operations.pop();
            result.clear();
            if (operations.empty()&& !bracketCounter) {
                end = true;
            }
        }
        else {
            if (wordBuf == "AND") {
                operations.push(AND);
            }
            else if (wordBuf == "OR") {
                operations.push(OR);
            }
            else if (wordBuf == "NOT") {
                readWord(request, wordBuf);
                word1List = dictionary.find(wordBuf)->second;
                notSearch(word1List, docNumber, result);
                lists.push(result);
                result.clear();
            }
            else {
                std::string wordLine = wordBuf;
                if (biword) {  //read two words if biword
                    readWord(request, wordBuf);
                    wordLine += " ";
                    wordLine += wordBuf;
                }
                if (request == ")" && operations.empty()) end = true;
                word1List = dictionary.find(wordLine)->second;
                lists.push(word1List);
            }
        }
        
        }
    return lists.top();

}

std::set<short> phraseSearch(std::string request, std::map<std::string, std::set<short>>& dictionary, const int& docNumber) 
{
    std::string wordBuf = "";
    std::string parsedRequest = "( ";
    for (int i = 0; !request.empty(); i++) {
        readWord(request, wordBuf);
        wordBuf += " ";
        parsedRequest += wordBuf;
        if (i % 2 == 1&& !request.empty()) {
            parsedRequest += "AND ";
            parsedRequest += wordBuf;
        }
    }
    parsedRequest += ")";
    return booleanSearch(parsedRequest, dictionary, docNumber, true);
}

std::set<short> positionalSearch(std::string request, PositionalIndex& dictionary)
{
    std::stack<std::map<short, std::set<int>>> postLists;
    while (!request.empty()) {  //fill stack with lists
        std::string buf1 = "";
        readWord(request, buf1);
        if (buf1.at(0) != '/') {
            std::map<short, std::set<int>> list1 = dictionary.getPostingsList(buf1);
            postLists.push(list1);
        }
        else {
            int k = std::stoi(buf1.substr(1, buf1.length()));
            readWord(request, buf1);
            std::map<short, std::set<int>> list1 = dictionary.getPostingsList(buf1);
            std::map<short, std::set<int>> list2 = positionalIntersect(list1, postLists.top(), k);
            postLists.pop();
            postLists.push(list2);
        }
    }

    while (postLists.size() > 1) {
        std::map<short, std::set<int>> list2 = postLists.top();
        postLists.pop();
        std::map<short, std::set<int>> list1 = postLists.top();
        postLists.pop();
        postLists.push(positionalIntersect(list1, list2));
    }
    
    std::set<short> result;
    std::map<short, std::set<int>>::iterator it = postLists.top().begin();
    while (it != postLists.top().end()) {
        result.insert(it->first);
        ++it;
    }
    return result;
}

void sortBinOperations(std::stack<std::set<short>>& lists, std::stack<binOperations>& operations, int currentSize)  // TO DO
{
    
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
        if (it1 == list1.end()||*it1 != i) {
            result.insert(i);
        }
        else {
            ++it1;
        }
    }
}

std::map<short, std::set<int>> positionalIntersect(std::map<short, std::set<int>>& list1, std::map<short, std::set<int>>& list2, const int& k)
{
    std::map<short, std::set<int>> result;
    std::map<short, std::set<int>>::iterator it1 = list1.begin();
    std::map<short, std::set<int>>::iterator it2 = list2.begin();
    while (it1 != list1.end() && it2 != list2.end()) {
        if (it1->first == it2->first) {
            std::list<int> positions;
            std::set<int> plist1 = it1->second;
            std::set<int> plist2 = it2->second;
            std::set<int>::iterator pit1 = plist1.begin();
            std::set<int>::iterator pit2 = plist2.begin();
            while (pit1 != plist1.end()) {
                while (pit2 != plist2.end()) {
                    if (abs(*pit1 - *pit2) <= k) {
                        positions.push_back(*pit2);
                    }
                    else if (*pit2 > * pit1) {
                        break;
                    }
                    ++pit2;
                }
                while (!positions.empty() && abs(positions.front() - *pit1) > k) {
                    positions.pop_front();
                }
                std::list<int>::iterator posIt = positions.begin();
                while (posIt != positions.end()) {
                    result.insert({ it1->first, {*pit1, *posIt} });
                    ++posIt;
                }
                ++pit1;
            }
            ++it1;
            ++it2;            
        }
        else if (it1->first < it2->first) {
            ++it1;
        }
        else {
            ++it2;
        }
    }
    return result;
}

std::map<short, std::set<int>> positionalIntersect(std::map<short, std::set<int>>& list1, std::map<short, std::set<int>>& list2)  //TO DO: optimize
{
    std::map<short, std::set<int>> result;
    std::map<short, std::set<int>>::iterator it1 = list1.begin();
    std::map<short, std::set<int>>::iterator it2 = list2.begin();
    while (it1 != list1.end() && it2 != list2.end()) {
        if (it1->first == it2->first) {
            std::list<int> positions;
            std::set<int> plist1 = it1->second;
            std::set<int> plist2 = it2->second;
            std::set<int>::iterator pit1 = plist1.begin();
            std::set<int>::iterator pit2 = plist2.begin();
            while (pit1 != plist1.end()) {
                while (pit2 != plist2.end()) {
                    if (*pit2 - *pit1 == 1) {
                        positions.push_back(*pit2);
                    }
                    else if (*pit2 > *pit1) {
                        break;
                    }
                    ++pit2;
                }
                std::list<int>::iterator posIt = positions.begin();
                while (posIt != positions.end()) {
                    result.insert({ it1->first, {*pit1, *posIt} });
                    ++posIt;
                }
                ++pit1;
            }
            ++it1;
            ++it2;
        }
        else if (it1->first < it2->first) {
            ++it1;
        }
        else {
            ++it2;
        }
    }
    return result;
}

std::map<std::string, std::set<short>> postWildcardQuery(std::string word)
{
    word = word.substr(0, word.length() - 1);
    int filesNumber = 0;
    std::map<std::string, std::set<short>> simpleInvertedIndex;
    std::map<std::string, std::set<short>> result;
    readDictionary(simpleInvertedIndex, "invertedIndex.txt", filesNumber);
    Btree* tree = new Btree;
    for (auto i : simpleInvertedIndex) {
        addWord(tree, i.first);
    }
    
    std::list<std::string> words = getWords(tree, word);
    delete tree;
    for (auto i : words) {
        std::map<std::string, std::set<short>>::iterator found = simpleInvertedIndex.find(i);
        if (found != simpleInvertedIndex.end()) {
            result.insert(*found);
        }
    }
    return result;
}

std::map<std::string, std::set<short>> freeWildcardQuery(std::string word)
{
    std::map<std::string, std::set<short>> result;
    std::map<std::string, std::set<short>> simpleInvertedIndex;
    int fileNumber = 0;
    readDictionary(simpleInvertedIndex, "invertedIndex.txt", fileNumber);

    word += '$';
    Btree* tree = new Btree;
    permutermIndex(tree, simpleInvertedIndex);
    while (word.at(word.length() - 1) != '*') {
        word += word.at(0);
        word = word.substr(1, word.length());
    }
    word = word.substr(0, word.length()-1);  //delete "*" in the end
    std::list<std::string> words = getWords(tree, word);
    delete tree;

    std::list<std::string>::iterator element = words.begin();
    for (auto i : words) {
        while (i.at(i.length()-1) != '$') {
            i += i.at(0);
            i = i.substr(1, word.length()+1);
        }
        i = i.substr(0, word.length());
        *element = i;
        ++element;
    }

    for (auto i : words) {
        std::map<std::string, std::set<short>>::iterator found = simpleInvertedIndex.find(i);
        if (found != simpleInvertedIndex.end()) {
            result.insert(*found);
        }
    }
    return result;
}
