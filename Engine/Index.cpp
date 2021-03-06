#include "pch.h"
#include "Index.h"



void Index::saveNumberOfFiles(const std::string& directoryPath)
{
    std::ofstream file;
    file.open(directoryPath + "number_filename.txt");
    for (auto i : number_filename) {
        file << i.first << " - " << i.second << std::endl;
    }
    file.close();
}

Index::Index(const std::string directoryPath, const std::string savingPath) : _savingPath(savingPath), compressedSPIMIIndex(_savingPath, number_filename)
{
    _indexingDirSize = CommonIndexingFunctions::enumerateFilesInDir(directoryPath, number_filename);
}

void Index::generateInvertedIndex() {
    std::ifstream file;
    #pragma region Statistic
        _bytesToIndexLeft = _indexingDirSize;
    #pragma endregion

    for (auto i : number_filename) {
        file.open(i.second);
        std::string word = "";
        char buf;
        while (!file.eof()) {
            buf = file.get();
            #pragma region StatisticChange
                --_bytesToIndexLeft;
            #pragma endregion
            if (buf < 65 ||(buf>90&&buf<97)|| buf>122) {
                if (word != "") {
                    addWordToDictionary(invertedIndex, word, i.first);
                    word = "";
                }
            }
            else {
                word += buf;
            }
        }
        file.close();
    }
    printInFile(invertedIndex, "invertedIndex");
}

void Index::generateBiwordIndex()
{
    std::ifstream file;
    #pragma region Statistic
        _bytesToIndexLeft = _indexingDirSize;
    #pragma endregion

    for (auto i : number_filename) {
        file.open(i.second);
        std::string word = "", previousWord = "";
        bool firstWord = true;
        char buf;
        while (!file.eof()) {
            buf = file.get();
            #pragma region StatisticChange
                --_bytesToIndexLeft;
            #pragma endregion
            if (buf < 65 || buf>122) {
                if (!word.empty()) {
                    if (firstWord) {
                        firstWord = false;
                    }
                    else {
                        addWordToDictionary(biwordIndex, previousWord + word, i.first);
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
    printInFile(biwordIndex, "biwordIndex");
}

void Index::generatePositionalIndex()
{
    std::ifstream file;
    #pragma region Statistic
        _bytesToIndexLeft = _indexingDirSize;
    #pragma endregion

    for (auto i : number_filename) {
        file.open(i.second);
        std::string word = "";
        int position = 0;
        char buf;
        while (!file.eof()) {
            buf = file.get();
            #pragma region StatisticChange
                --_bytesToIndexLeft;
            #pragma endregion
            if (buf < 65 || buf>122) {
                if (word != "") {
                    positionalIndex.addNewWord(word, 1, i.first, position);
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
    positionalIndex.printInFile(_savingPath + "positionalIndex", number_filename.size());
}

void Index::generatePermutermIndex()
{
    for (auto i : invertedIndex) { //iteration through map keys
        std::string key = i.first;
        std::string word = key + '$';
        addWord(permutermIndex, word);
        for (int j = 0; j < key.length(); j++) {  //generating all permutations 
            word = word + word.at(0);
            word = word.substr(1, word.length());
            addWord(permutermIndex, word);
        }
    }
}

void Index::generateThreegramIndex()
{
    for (auto i : invertedIndex) {
        std::string word = '$'+i.first+'$';
        std::string buf = i.first;
        for (int j = 0; j < word.length()-2; j++) {
            addWordToDictionary(threegramIndex, word.substr(j, 3), buf);
        }
    }
}

void Index::generateBySPIMICompressedIndex()
{
    _bytesToIndexLeft = _indexingDirSize;
    compressedSPIMIIndex.generateInvertedIndexBySPIMI(_bytesToIndexLeft);
}

template<class T> void Index::addWordToDictionary(std::map<std::string, std::set<T>>& dictionary, const std::string& word, const T& value) {
    std::set<T> arr;
    arr.insert(value);
    std::pair<std::map<std::string, std::set<T>>::iterator, bool> found = dictionary.insert({ word, arr });
    if (!found.second) {
        found.first->second.insert(value);
    }
}

void Index::printInFile(std::map<std::string, std::set<short>>& dictionary, const std::string& indexName) {
    std::ofstream out;
    out.open(_savingPath + indexName +".txt");
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

void Index::loadIndex(IndexName name)
{
    switch (name) {
    case IndexName::inverted:
        if (invertedIndex.empty())
            if (!readFromFile("invertedIndex.txt", invertedIndex))
                generateInvertedIndex();
        break;
    case IndexName::biword:
        if (biwordIndex.empty())
            if (!readFromFile("biwordIndex.txt", biwordIndex))
                generateBiwordIndex();
        break;
    case IndexName::positional:
        if (positionalIndex.empty())
            if (!positionalIndex.readFromFile(_savingPath + "positionalIndex.txt"))
                generatePositionalIndex();
        break;
    } 
}

bool Index::readFromFile(const std::string& fileName, std::map<std::string, std::set<short>>& index)
{
    std::ifstream file;
    std::string lineBuf;
    file.open(_savingPath + fileName);
    if (file.fail()) { return false; }

    while (true) {
        getline(file, lineBuf);
        if (lineBuf.empty()) {
            break;
        }
        std::map<std::string, std::set<short>>::iterator currentElement = insertMapKey(lineBuf, index);
        std::string wordBuf = "";
        for (int i = 0; !lineBuf.empty(); i++) {
            readWord(lineBuf, wordBuf);
            currentElement->second.insert(std::stoi(wordBuf));
        }
    }
    file.close();
    return true;
}

std::map<std::string, std::set<short>>::iterator Index::insertMapKey(std::string& lineBuf, std::map<std::string, std::set<short>>& result)
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

void Index::readWord(std::string& line, std::string& value) {
    value = line.substr(0, line.find(" "));
    if (line.find(" ") == line.npos) {
        line.clear();
    }
    else {
        line = line.substr(line.find(" ") + 1, line.length());
    }
}

void Index::saveReadWord(const std::string& line, std::string& value) 
{
    value = line.substr(0, line.find(" "));
}

std::vector<std::string> Index::separateString(std::string line)
{
    std::string word = "";
    std::vector<std::string> result;
    saveReadWord(line, word);

    if (word != "(") line = line + " )";
    else readWord(line, word);
   
    while (line.size()!=1) {
        readWord(line, word);
        if (word == "(") {
            result.push_back(readInBrackets(line));
        }
        else if (word == "NOT") {
            readWord(line, word);
            result.push_back("NOT "+word);
        }
        else {
            result.push_back(word);
        }
    }
    return result;
}

std::string Index::readInBrackets(std::string& line)
{
    std::string result = "(", buf;
    int bracketCounter = 1;
    while (bracketCounter != 0) {
        readWord(line, buf);
        result += " ";
        result += buf;
        if (buf == "(") ++bracketCounter;
        else if (buf == ")") --bracketCounter;
    }

    return result;
}

std::set<short> Index::booleanSearch(const std::string& request)
{
    return booleanSearchGeneral(request, invertedIndex);
}

std::set<short> Index::phraseSearch(std::string request) 
{
    std::string wordBuf = "";
    std::string parsedRequest = "";
    for (int i = 0; !request.empty(); i++) {
        readWord(request, wordBuf);
        parsedRequest += wordBuf;
        if (i > 0 && !request.empty()) {
            parsedRequest += " AND ";
            parsedRequest += wordBuf;
        }
    }
    return booleanSearchGeneral(parsedRequest, biwordIndex);
}

std::set<short> Index::positionalSearch(std::string request)
{
    std::stack<std::map<short, std::set<int>>> postLists;
    while (!request.empty()) {  //fill stack with lists
        std::string buf1 = "";
        readWord(request, buf1);
        if (buf1.at(0) != '/') {
            std::map<short, std::set<int>> list1 = positionalIndex.getPostingsList(buf1);
            postLists.push(list1);
        }
        else {
            int k = std::stoi(buf1.substr(1, buf1.length()));
            readWord(request, buf1);
            std::map<short, std::set<int>> list1 = positionalIndex.getPostingsList(buf1);
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

std::list<std::string> Index::booleanSearchPaths(const std::string& request)
{
    std::list<std::string> result;
    for (auto i : booleanSearch(request)) {
        result.push_back(number_filename[i].path().u8string());
    }
    return result;
}

std::set<short> Index::notSearch(const std::set<short>& list1) {
    std::set<short> result;
    std::set<short>::iterator it1 = list1.begin();
    for (auto i : number_filename) {
        if (it1 == list1.end()||*it1 != i.first) {
            result.insert(i.first);
        }
        else {
            ++it1;
        }
    }
    return result;
}

std::set<std::string> Index::notSearch(const std::set<std::string>& list1) {
    return std::set<std::string>();
}

std::set<Index::PostingList> Index::notSearch(const std::set<PostingList>& list1) {
    return std::set<PostingList>();
}

std::map<short, std::set<int>> Index::positionalIntersect(std::map<short, std::set<int>>& list1, std::map<short, std::set<int>>& list2, const int& k)
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

std::map<short, std::set<int>> Index::positionalIntersect(std::map<short, std::set<int>>& list1, std::map<short, std::set<int>>& list2)  //TO DO: optimize
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

std::map<std::string, std::set<short>> Index::postWildcardQuery(std::string word)
{
    word = word.substr(0, word.length() - 1);
    std::map<std::string, std::set<short>> result;
    loadIndex(IndexName::inverted);

    Btree* tree = new Btree;
    for (auto i : invertedIndex) {
        addWord(tree, i.first);
    }
    
    std::list<std::string> words = getWords(tree, word);
    delete tree;
    for (auto i : words) {
        std::map<std::string, std::set<short>>::iterator found = invertedIndex.find(i);
        if (found != invertedIndex.end()) {
            result.insert(*found);
        }
    }
    return result;
}

std::map<std::string, std::set<short>> Index::freeWildcardQueryPermuterm(std::string word)
{
    std::map<std::string, std::set<short>> result;

    loadIndex(IndexName::inverted);
    generatePermutermIndex();

    word += '$';
    while (word.at(word.length() - 1) != '*') {
        word += word.at(0);
        word = word.substr(1, word.length());
    }
    word = word.substr(0, word.length()-1);  //delete "*" in the end
    std::list<std::string> words = getWords(permutermIndex, word);
    delete permutermIndex;

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
        std::map<std::string, std::set<short>>::iterator found = invertedIndex.find(i);
        if (found != invertedIndex.end()) {
            result.insert(*found);
        }
    }
    return result;
}

std::map<std::string, std::set<short>> Index::freeWildcardQuery3Gramm(std::string word)
{
    std::map<std::string, std::set<short>> result;

    loadIndex(IndexName::inverted);
    generateThreegramIndex();

    std::string request = "( ";

    std::string firstPart = '$' + word.substr(0, word.find('*'));
    std::string secondPart = word.substr(word.find('*')+1, word.length()) + '$';

    for (int i = 0; i < 1.0 * firstPart.length() - 2; i++) {
        request += firstPart.substr(i, 3);
        request += " AND ";
    }
    for (int i = 0; i < 1.0*secondPart.length() - 2; i++) {
        request += secondPart.substr(i, 3);
        request += " AND ";
    }
    if (request.length() > 3) {
        request = request.substr(0, request.length() - 4);
        request += ')';

        std::set<std::string> words;
        words = booleanSearchGeneral(request, threegramIndex);

        for (auto i : words) {
            if (i.length() == word.length()) {   //postfiltration
                result.insert(*invertedIndex.find(i));
            }
        }
    }
    return result;
}
