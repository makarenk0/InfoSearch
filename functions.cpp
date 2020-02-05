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
                    addWordToDictionary(dictionary, word, i, position);
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
    printInFile(dictionary, fileNumber, "positionalIndex");
}

void addWordToDictionary(std::map<std::string, std::set<short>>& dictionary, const std::string& word, short& bookNumber) {
    std::set<short> arr;
    arr.insert(bookNumber);
    std::pair<std::map<std::string, std::set<short>>::iterator, bool> found = dictionary.insert({ word, arr });
    if (!found.second) {
        found.first->second.insert(bookNumber);
    }
}

void addWordToDictionary(PositionalIndex& dictionary, const std::string& word, const short& bookNumber, const int& position)
{
    std::set<int> arr;
    arr.insert(position);
    std::map<short, std::set<int>> lists;
    lists.insert({ bookNumber, arr });
    std::pair<int, std::map<short, std::set<int>>> frequncy_lists(1, lists);
    std::pair<PositionalIndex::iterator, bool> foundWord = dictionary.insert({ word, frequncy_lists });
    if (!foundWord.second) {
        foundWord.first->second.first +=1;
        std::pair<std::map<short, std::set<int>>::iterator, bool> foundInDoc = foundWord.first->second.second.insert({ bookNumber , arr});
        if (!foundInDoc.second) {
            foundInDoc.first->second.insert(position);
        }
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

void printInFile(PositionalIndex& dictionary, const int& allFilesNumber, const std::string& dicName)
{
    std::ofstream out;
    out.open("Dictionaries\\" + dicName + ".txt");
    out << allFilesNumber << std::endl;
    for (auto i : dictionary) {
        out << i.first << " "<< i.second.first<<" ; ";
        for (auto j : i.second.second) {
            out << j.first<<" : ";
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

void readDictionary(PositionalIndex& result, const std::string dictionaryName, int& filesNumber)
{
    std::string lineBuf;
    std::ifstream file("Dictionaries\\" + dictionaryName);
    getline(file, lineBuf);
    filesNumber = std::stoi(lineBuf);
    while (true) {
        getline(file, lineBuf);
        if (lineBuf.empty()) {
            break;
        }
        PositionalIndex::iterator currentElement = insertMapKey(lineBuf, result);

        std::string wordBuf = "";
        for (int i = 0; !lineBuf.empty(); i++) {
            readWord(lineBuf, wordBuf);
            std::set<int> list;
            std::map<short, std::set<int>> doc_pos;
            std::map<short, std::set<int>>::iterator current = doc_pos.insert(doc_pos.end(), {(short)(std::stoi(wordBuf)), list });
            readWord(lineBuf, wordBuf);  //read ":"
            for (int j = 0; wordBuf != ";"; j++) {
                readWord(lineBuf, wordBuf);
                current->second.insert(std::stoi(wordBuf));
            }
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

PositionalIndex::iterator insertMapKey(std::string& lineBuf, PositionalIndex& result)
{
    std::string wordBuf = "";
    readWord(lineBuf, wordBuf);
    std::string wordLine = wordBuf;
    readWord(lineBuf, wordBuf);
    int frequency = std::stoi(wordBuf);
    std::map<short, std::set<int>> list;
    readWord(lineBuf, wordBuf);
    return result.insert(result.end(), { wordLine , std::pair<int, std::map<short, std::set<int>>>({frequency, list}) });
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
