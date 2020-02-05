#include "functions.h"
#include <ctime>


int main()
{
    const std::string dictionaryName = "positionalIndex.txt";

    const std::string directoryPath = "Books\\";

    //auto begin = clock();   //count time 
    //invertedIndex(directoryPath);
    //auto end = clock();  //end counting
    //double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
    //std::cout << "Time spend in millis: " << elapsedMs << std::endl;


   /* //boolean search
    std::map<std::string, std::set<short>> read;
    int filesNumber = 0;
    readDictionary(read, dictionaryName, filesNumber);
    std::set<short> requestRes = booleanSearch("( zone AND zones AND NOT zoom AND NOT zigzag )", read, filesNumber);*/


    // //phrase search
    //std::map<std::string, std::set<short>> read;
    //int filesNumber = 0;
    //readDictionary(read, dictionaryName, filesNumber);
    //std::set<short> requestRes = phraseSearch("zeal Some", read, filesNumber);
    

    /* //output
    for (auto i : requestRes) {
        std::cout << i << std::endl;
    }*/


    //auto begin = clock();   //count time 
    //positionalIndex(directoryPath);
    //auto end = clock();  //end counting
    //double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
    //std::cout << "Time spend in millis: " << elapsedMs << std::endl; 

    PositionalIndex read;
    int filesNumber = 0;
    readDictionary(read, dictionaryName, filesNumber);

 
    return 0;
}

