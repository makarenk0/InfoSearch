
#include "SPIMI.h"
#include <ctime>
#include <bitset>


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
    //auto begin = clock();   //count time 
    //test.generateInvertedIndexBySPIMI(directoryPath);
    //auto end = clock();  //end counting
    //double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
    //std::cout << "Time spend in millis: " << elapsedMs << std::endl;
    
    IndexCompression compressor = test.getCompressor();
       
   
    
    return 0;
}

