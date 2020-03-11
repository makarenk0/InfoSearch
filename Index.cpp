#include "Index.h"

void Index::enumerateFilesInDir(const std::string& directoryPath)
{
    WIN32_FIND_DATA FindFileData = {0};
    HANDLE hFind;
    setlocale(LC_ALL, "");

    std::wstring stemp = std::wstring(directoryPath.begin(), directoryPath.end());
    stemp += L"*.txt";
    LPCWSTR path = stemp.c_str();

    number_filename.clear();
    hFind = FindFirstFile(path, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        throw "Folder "+ directoryPath +" is empty";
    }
    else {
        std::wstring ws(FindFileData.cFileName);  //TO DO: reduce the code
        number_filename.insert({ 1, std::string(ws.begin(), ws.end()) });
    }
    for (int i = 2; FindNextFile(hFind, &FindFileData) != 0; i++) {
        std::wstring ws(FindFileData.cFileName);
        number_filename.insert({ i, std::string(ws.begin(), ws.end()) });
    }
}

void Index::saveNumberOfFiles(const std::string& directoryPath)
{
    std::ofstream file;
    file.open(directoryPath + "number_filename.txt");
    for (auto i : number_filename) {
        file << i.first << " - " << i.second << std::endl;
    }
    file.close();
}

Index::Index(const std::string& directoryPath)
{
    enumerateFilesInDir(directoryPath);
}

Index::Index()
{
}

void Index::generateInvertedIndex(const std::string& directoryPath) {
    std::ifstream file;
    for (auto i : number_filename) {
        file.open(directoryPath + i.second);
        std::string word = "";
        char buf;
        while (!file.eof()) {
            buf = file.get();
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

void Index::generateBiwordIndex(const std::string& directoryPath)
{
    std::ifstream file;
    for (auto i : number_filename) {
        file.open(directoryPath + i.second);
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

void Index::generatePositionalIndex(const std::string& directoryPath)
{
    std::ifstream file;
    for (auto i : number_filename) {
        file.open(directoryPath + i.second);
        std::string word = "";
        int position = 0;
        char buf;
        while (!file.eof()) {
            buf = file.get();
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
    positionalIndex.printInFile("positionalIndex", number_filename.size());
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
    out.open("Index\\"+ indexName +".txt");
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
        if(invertedIndex.empty()) readFromFile("invertedIndex.txt", invertedIndex);
        break;
    case IndexName::biword:
        if (biwordIndex.empty()) readFromFile("biwordIndex.txt", biwordIndex);
        break;
    case IndexName::positional:
        if (positionalIndex.empty()) positionalIndex.readFromFile("positionalIndex.txt");
        break;
    } 
}

void Index::readFromFile(const std::string& fileName, std::map<std::string, std::set<short>>& index)
{
    std::ifstream file;
    std::string lineBuf;
    file.open("Index\\"+ fileName);
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


template<class T> std::set<T> Index::booleanSearchGeneral(std::string request, std::map<std::string, std::set<T>>& dictionary)
{
    std::vector<std::string> requestParts = separateString(request);

    if (requestParts.size() == 1) {
        if (requestParts[0].substr(0, 3) == "NOT") return notSearch(dictionary.find(requestParts[0].substr(4, requestParts[0].length()-3))->second);
        else return dictionary.find(requestParts[0])->second;
    }
    
    std::stack<std::set<T>> lists;
    lists.push(booleanSearchGeneral(requestParts[0], dictionary));

    for (std::vector<std::string>::iterator i = requestParts.begin()+1; i!= requestParts.end(); i++) {   // AND
        if (*i == "AND") {
            std::set<T> list1 = lists.top();
            lists.pop();
            i += 1;
            lists.push(andSearch(list1, booleanSearchGeneral(*i, dictionary)));
            
        }
        else if (*i == "OR") {
            i += 1;
            lists.push(booleanSearchGeneral(*i, dictionary));
        }
    }
    while (lists.size() != 1) {   //OR
        std::set<T> list1 = lists.top();
        lists.pop();
        std::set<T> list2 = lists.top();
        lists.pop();
        lists.push(orSearch(list1, list2));
    }
    return lists.top();
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


template<class T> std::set<T> Index::andSearch(const std::set<T>& list1, const std::set<T>& list2) {
    std::set<T> result;
    std::set<T>::iterator it1 = list1.begin();
    std::set<T>::iterator it2 = list2.begin();
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
    return result;
}

template<class T> std::set<T> Index::orSearch(const std::set<T>& list1, const std::set<T>& list2) {
    std::set<T> result;
    std::set<T>::iterator it1 = list1.begin();
    std::set<T>::iterator it2 = list2.begin();
    while (it1 != list1.end()) {
            result.insert(*it1);
            ++it1;
    }
    while (it2 != list2.end()) {
        result.insert(*it2);
        ++it2;
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

std::set<std::string> Index::notSearch(const std::set<std::string>& list1) 
{
    return std::set<std::string>();
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

    for (int i = 0; i < 1.0*firstPart.length() - 2; i++) {
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
