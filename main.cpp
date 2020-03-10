
#include "SPIMI.h"
#include <ctime>


int main()
{
    const std::string directoryPath = "Col\\";
    //Index index(directoryPath);


    //inverted index generate
    //auto begin = clock();   //count time 
    //generateInvertedIndex(directoryPath);
    //auto end = clock();  //end counting
    //double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
    //std::cout << "Time spend in millis: " << elapsedMs << std::endl;


    //boolean search
    
   /* index.loadIndex(Index::IndexName::inverted);
    std::set<short> requestRes = index.booleanSearch("NOT zone OR NOT zones");

    for (auto i : requestRes) {
        std::cout << i << std::endl;
    }*/



    //auto begin = clock();   //count time 
    //index.generateBiwordIndex(directoryPath);
    //auto end = clock();  //end counting
    //double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
    //std::cout << "Time spend in millis: " << elapsedMs << std::endl;



     //phrase search
    /*index.loadIndex(Index::IndexName::biword);
    std::set<short> requestRes = index.phraseSearch("curious to recall some");
    for (auto i : requestRes) {
        std::cout << i << std::endl;
    }*/

    //Positional index generate
    //auto begin = clock();   //count time 
    //index.generatePositionalIndex(directoryPath);
    //auto end = clock();  //end counting
    //double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
    //std::cout << "Time spend in millis: " << elapsedMs << std::endl; 


   /* index.loadIndex(Index::IndexName::positional);
    std::set<short> requestRes = index.positionalSearch("forest frowned /2 either");
    for (auto i : requestRes) {
        std::cout << i << std::endl;
    }*/


    /*std::map<std::string, std::set<short>> query = index.postWildcardQuery("compute*");
    for (auto i : query) {
        std::cout << i.first << " in docs: ";
        for (auto j : i.second) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }*/

   /* std::map<std::string, std::set<short>> query = index.freeWildcardQueryPermuterm("comp*ter");
    for (auto i : query) {
        std::cout << i.first << " in docs: ";
        for (auto j : i.second) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }*/

    /*std::map<std::string, std::set<short>> query = index.freeWildcardQuery3Gramm("comp*ter");
    for (auto i : query) {
        std::cout<< i.first << " in docs: ";
        for (auto j : i.second) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }*/


    SPIMI test(directoryPath);
    auto begin = clock();   //count time 
    test.generateInvertedIndexBySPIMI(directoryPath);
   
    auto end = clock();  //end counting
    double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
    std::cout << "Time spend in millis: " << elapsedMs << std::endl;

    //test.mergeFiles("Index\\SPIMI\\");



    //MEMORYSTATUSEX statex;

    //statex.dwLength = sizeof(statex);

    //GlobalMemoryStatusEx(&statex);
    //std::cout << statex.ullAvailPhys<<std::endl;

    //std::string d = "fsdafgsdgfsdgsdgsdgsdgsd";

    //GlobalMemoryStatusEx(&statex);
    //std::cout << statex.ullAvailPhys<<std::endl;
    //_tprintf(TEXT("There is  %*ld percent of memory in use.\n"),
    //    WIDTH, statex.dwMemoryLoad);
    //_tprintf(TEXT("There are %*I64d total KB of physical memory.\n"),
    //    WIDTH, statex.ullTotalPhys / DIV);
    //_tprintf(TEXT("There are %*I64d free  KB of physical memory.\n"),
    //    WIDTH, statex.ullAvailPhys / DIV);
    //_tprintf(TEXT("There are %*I64d total KB of paging file.\n"),
    //    WIDTH, statex.ullTotalPageFile / DIV);
    //_tprintf(TEXT("There are %*I64d free  KB of paging file.\n"),
    //    WIDTH, statex.ullAvailPageFile / DIV);
    //_tprintf(TEXT("There are %*I64d total KB of virtual memory.\n"),
    //    WIDTH, statex.ullTotalVirtual / DIV);
    //_tprintf(TEXT("There are %*I64d free  KB of virtual memory.\n"),
    //    WIDTH, statex.ullAvailVirtual / DIV);

    //// Show the amount of extended memory available.

    //_tprintf(TEXT("There are %*I64d free  KB of extended memory.\n"),
    //    WIDTH, statex.ullAvailExtendedVirtual / DIV);


    return 0;
}

