#include "functions.h"
#include <ctime>


int main()
{
    const int fileNumber = 10;
   
    const std::string filenames[fileNumber] = { "book1", "book2", "book3", "book4", "book5", "book6", "book7", "book8", "book9", "book10"};

    const std::string dictionaryName = "result.txt";

    auto begin = clock();   //count time 
    generateDictionary(filenames, fileNumber);
    auto end = clock();  //end counting
    double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
    std::cout << "Time spend in millis: " << elapsedMs << std::endl;

    std::map<std::string, std::set<short>> read;

    readDictionary(read, dictionaryName);

    std::set<short> requestRes = booleanSearch("( zone AND zones )", read, fileNumber);
    
    for (auto i : requestRes) {
        std::cout << i << std::endl;
    }
 
    return 0;
}

