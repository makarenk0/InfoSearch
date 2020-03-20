
#include "SPIMI.h"
#include <ctime>
#include <bitset>

#include "tinyxml2.h"
#include "ZoneIndex.h"





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


   /* SPIMI test(directoryPath);*/
    //auto begin = clock();   //count time 
    //test.generateInvertedIndexBySPIMI(directoryPath);
    //auto end = clock();  //end counting
    //double elapsedMs = double(end * 1.0 - begin) * 1000.0 / CLOCKS_PER_SEC;
    //std::cout << "Time spend in millis: " << elapsedMs << std::endl;
    
    /*IndexCompression compressor = test.getCompressor();*/

   /* tinyxml2::XMLDocument doc;
    doc.LoadFile("XMLcollection\\test.txt");


  
    tinyxml2::XMLElement* st = doc.FirstChildElement();
    tinyxml2::XMLElement* end = doc.LastChildElement();

    std::string s = st->Value();
    std::cout << s;*/

    ZoneIndex zoneIndex("XMLcollection\\");
    zoneIndex.generateZoneIndex();


    std::unordered_map<std::string, std::string> zones({ {{"genre"},{"fantast"}},{{"author"},{"Joanne Kathleen Rowling"}},{{"book-title"},{"Harry Potter"}},{{"lang"},{"en"}} });
    std::list<int> res = zoneIndex.zoneIndexSearch("Harry", zones);
    for (auto i : res) {
        std::cout << i << std::endl;
    }

    //std::cout<< doc.FirstChildElement("text")->GetText();
    
//std::set<std::pair<short, short>> test;
//test.insert({ 2, 1 });
//test.insert({ 1, 3 });    
//test.insert({ 4, 0 });
//for (auto i : test) {
//    std::cout << i.second<<std::endl;
    return 0;
}

