#include "SPIMI.h"
#include <ctime>

void SPIMI::addWordToDictionary(const std::string& word, const int& value, double& memoryCounter)
{
    std::pair<std::unordered_map<std::string, std::set<int>>::iterator, bool> found = _dataBuf.insert({ word,  {value} });
    if (!found.second) {
        if (found.first->second.insert(value).second) {
            memoryCounter += sizeof(int);
        }
    }
    else {
        charsNumber += word.length();
        ++wordsNumber;

        memoryCounter += word.length() + sizeof(int);
    }
}

std::pair<std::string, std::set<int>> SPIMI::getLineFromFile(std::ifstream& stream)
{
    std::string line = "";
    std::string word = "";
    std::pair<std::string, std::set<int>> result;
    getline(stream, line);
    if (!line.empty()) {
        readWord(line, word);
        result.first = word;
        readWord(line, word);
        while (!line.empty()) {
            readWord(line, word);
            result.second.insert(std::stoi(word));
        }
    }
    return result;
}

void SPIMI::mergeFiles(const std::string& directoryPath)
{
    enumerateFilesInDir(directoryPath);
    std::vector<std::ifstream> readingStreams;

    std::ofstream dictionaryOut("Index\\SPIMI\\Compressed\\InvertedIndexMergedDictionary.txt");
    std::ofstream postingsOut("Index\\SPIMI\\Compressed\\InvertedIndexMergedPostings.txt", std::ios::binary);
   

    for (auto i : number_filename) {
        readingStreams.push_back(std::ifstream(directoryPath + "\\" + i.second));
    }
    size_t nfiles = number_filename.size();
    std::pair<std::string, std::set<int>>* topFileLines = new std::pair<std::string, std::set<int>>[nfiles];

    while (true) {
        int minimal = 0;
        for (int i = 0; i < nfiles; i++) {
            if ((topFileLines[i].first).empty()) {
                topFileLines[i] = getLineFromFile(readingStreams[i]);
            }
            if (topFileLines[i].first < topFileLines[minimal].first) {
                minimal = i;
            }
        }
        if (topFileLines[minimal].first == "\0") {
            break;
        }

        for (int i = 0; i < nfiles; i++) {
            if (i != minimal && topFileLines[i].first == topFileLines[minimal].first) {
                topFileLines[minimal].second.insert(topFileLines[i].second.begin(), topFileLines[i].second.end());
                topFileLines[i].first.clear();
            }
        }
       // printWordInDictionary(dictionaryOut, topFileLines[minimal].first);
        printPostingList(postingsOut, topFileLines[minimal].second);


        topFileLines[minimal].first.clear();
    }
    delete[] topFileLines;
    for (int i = 0; i < nfiles; i++) {
        readingStreams[i].close();
    }
    dictionaryOut.close();
    postingsOut.close();
}

SPIMI::SPIMI(const std::string& directoryPath) : Index(directoryPath)
{

}

void SPIMI::generateInvertedIndexBySPIMI(const std::string& directoryPath)
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    
    double freeRam = statex.ullAvailPhys;
    std::cout<< "Free machine ram (gigabytes): " << freeRam <<std::endl;
    std::cout<< "Allowed to use ram for one block: " << oneBlockMemory /1000000000 << " gigabytes" << std::endl;
   
    
  
    std::ifstream file;
    std::string outputPath = "SPIMI\\invertedIndex";
    int j = 0;
    auto begin = clock();
    auto end = clock();
    char buf;
    double wTimer = 0;
    double memoryCounter = 0;
    int blockCounter = 0;


    for (auto i : number_filename) {
        ++j;

        //-------------------------statistic--------------------------------
        if (j % 500 == 0) {  //every 500 files print statistics
            end = clock();
            double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
            GlobalMemoryStatusEx(&statex);
            std::cout << "dataBuf size(only data in bytes):" << memoryCounter << std::endl;
            std::cout << "dataBuf size(number of elements):" << _dataBuf.size() << std::endl;
            std::cout << "Time spent:" << elapsedMs << std::endl;
            std::cout << "Time spent on word adding in hash table:" << wTimer << std::endl;
            std::cout << "Files read:" << i.first << std::endl << std::endl;
            wTimer = 0;
            begin = clock();
        }
        //--------------------------------------------------------------------

        file.open(directoryPath + i.second);
        std::string word = "";
       
        while (!file.eof()) {
            buf = file.get();
            if (buf < 65 || buf>122 || (buf > 90 && buf < 97)) {
                if (!word.empty()) {

                    auto wbegin = clock();
                    addWordToDictionary(word, i.first, memoryCounter);    //---------------Adding word in structure
                    auto wend = clock();
                    wTimer += double(wend * 1.0 - wbegin) * 1000.0 / CLOCKS_PER_SEC;
                    word.clear();

                    if (memoryCounter > oneBlockMemory) {
                        GlobalMemoryStatusEx(&statex);
                        printSortedIndex(outputPath, blockCounter);
                        ++blockCounter;
                        memoryCounter = 0;
                        std::cout << "Block was written on hard drive!" << std::endl;
                    }

                }
            }
            else {
                word += buf;
            }
        }
        file.close();
    }
    printSortedIndex(outputPath, blockCounter);


    std::cout << "Merge started" << std::endl;
    begin = clock();
    mergeFiles("Index\\SPIMI\\");   //------------------------Merge blocks on hard drive
    end = clock();
    std::cout << "Merge ended succesfully in "<< double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC<<" seconds"<< std::endl;
}

void SPIMI::printSortedIndex(std::string& outputPath, int& fileCounter)
{
    std::map<std::string, std::set<int>> ordered;
    for (auto i : _dataBuf) {
        ordered.insert({ i.first, i.second });
        _dataBuf.erase(_dataBuf.begin());
    }
    printInFile(ordered, outputPath + std::to_string(fileCounter));
    _dataBuf.clear();
}

void SPIMI::printInFile(std::map<std::string, std::set<int>>& dictionary, const std::string& indexName) {
    std::ofstream out;
    out.open("Index\\" + indexName + ".txt");
    for (auto i : dictionary) {
        out << i.first << " ";
        for (auto j : i.second) {
            out << " " << j;
        }
        out << std::endl;
    }
    out << std::endl;
    out.close();
}

void SPIMI::printPostingList(std::ofstream& output, const std::set<int>& postingList)
{
    std::vector<char> byteLine = _compressor.getEncodedByteArray(postingList);
    for (auto i : byteLine) {
        output.write((char*)&i, sizeof(i));
    }
    output.write((char*)0, sizeof(char));
}

void SPIMI::printWordInDictionary(std::ofstream& output, const std::string& word)
{
    output << word << "\n";
}
