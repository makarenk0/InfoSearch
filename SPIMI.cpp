#include "SPIMI.h"
#include <ctime>

void SPIMI::addWordToDictionary(const std::string& word, const int& value, double& memoryCounter)
{
    std::pair<std::unordered_map<std::string, std::set<int>>::iterator, bool> found = dataBuf.insert({ word,  {value} });
    if (!found.second) {
        if (found.first->second.insert(value).second) {
            memoryCounter += sizeof(int);
        }
    }
    else {
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
    std::ofstream output("Index\\SPIMI\\InvertedIndexMerged.txt");
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
        printInFileLine(output, topFileLines[minimal]);
        topFileLines[minimal].first.clear();
    }
    delete[] topFileLines;
    for (int i = 0; i < nfiles; i++) {
        readingStreams[i].close();
    }
    output.close();
}

SPIMI::SPIMI(const std::string& directoryPath) : Index(directoryPath)
{

}

void SPIMI::generateInvertedIndexBySPIMI(const std::string& directoryPath)
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    double availableMemory = 100000000;// 100 mb for clear data in RAM (the struct could take about 3 gb)
    std::cout<< "Start free ram" << statex.ullAvailPhys<<std::endl;
    std::cout<< "Allowed to use ram: " << availableMemory /1000000000 << std::endl;
    double memoryCounter = 0;
    int fileCounter = 0;
    std::string outputPath = "SPIMI\\invertedIndex";
   
    int j = 0;
    std::ifstream file;
    auto begin = clock();
    char buf;
    double wTimer = 0;
    for (auto i : number_filename) {
        ++j;
        if (j % 500 == 0) {  //every 500 files cout statistics
            auto end = clock();
            double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
            GlobalMemoryStatusEx(&statex);
            std::cout << "dataBuf size(only data in bytes):" << memoryCounter << std::endl;
            std::cout << "dataBuf size(number of elements):" << dataBuf.size() << std::endl;
            std::cout << "Rough Ram count of dataBuf: " << roughRamMapCount() << std::endl;
            std::cout << "Time spent:" << elapsedMs << std::endl;
            std::cout << "Time spent on word adding:" << wTimer << std::endl;
            std::cout << "Files read:" << i.first << std::endl << std::endl;
            wTimer = 0;
            begin = clock();
        }

        file.open(directoryPath + i.second);
        std::string word = "";
       
        while (!file.eof()) {
            buf = file.get();
            if (buf < 65 || buf>122 || (buf > 90 && buf < 97)) {
                if (!word.empty()) {

                    auto wbegin = clock();
                    addWordToDictionary(word, i.first, memoryCounter);
                    auto wend = clock();
                    wTimer += double(wend * 1.0 - wbegin) * 1000.0 / CLOCKS_PER_SEC;
                    word.clear();

                    if (memoryCounter > availableMemory) {
                        GlobalMemoryStatusEx(&statex);
                        printSortedIndex(outputPath, fileCounter);
                        ++fileCounter;
                        memoryCounter = 0;
                        std::cout << "File created!" << std::endl;
                    }

                }
            }
            else {
                word += buf;
            }
        }
        file.close();
    }
    printSortedIndex(outputPath, fileCounter);
    //mergeFiles("Index\\SPIMI");
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

void SPIMI::printSortedIndex(std::string& outputPath, int& fileCounter)
{
    std::map<std::string, std::set<int>> ordered;
    for (auto i : dataBuf) {
        ordered.insert({ i.first, i.second });
        dataBuf.erase(dataBuf.begin());
    }
    printInFile(ordered, outputPath + std::to_string(fileCounter));
    dataBuf.clear();
}

void SPIMI::printInFileLine(std::ofstream& output, std::pair<std::string, std::set<int>>& line)
{
    output << line.first << " ";
    for (auto i : line.second) {
        output << " " << i;
    }
    output << std::endl;
}

double SPIMI::roughRamMapCount()
{
    size_t n = dataBuf.bucket_count();
    float m = dataBuf.max_load_factor();
    if (m > 1.0) {
        return n * 1.0 * m;
    }
    else {
        return n;
    }
}
