#pragma once
#include "tinyxml2.h"  //xml reader library added
#include "Index.h"
#include <tuple>
#include <list>
#include <string>
#include <unordered_map>

class ZoneIndex : Index
{
private:
    struct DocId_Relevance {
        int docId;
        mutable float relevance;
        bool operator>(const DocId_Relevance& other) const 
        {
            return docId > other.docId;
        }
        bool operator<(const DocId_Relevance& other) const
        {
            return docId < other.docId;
        }
    };
    void fillZoneIndex(tinyxml2::XMLElement* start, tinyxml2::XMLElement* lastEl, const int& docId);
    const std::string _directoryPath;
    void insertWord(const std::string& word, const int& docId);
    tinyxml2::XMLElement* findMetaData(tinyxml2::XMLElement* start);
    tinyxml2::XMLElement* findBody(tinyxml2::XMLElement* start);
    const int numberOfZones = 4;
    std::string _zones[4] = {"genre", "author", "book-title", "lang"};
    float _zoneWeights[4] = { 0.2, 0.4, 0.3, 0.1 };
    std::vector<std::string> _currentDocZones[4];
    void readZones(tinyxml2::XMLElement* start);
    char checkZones(const std::string& word);
    void stringToWords(std::string line, const int& docId);
    void printInFile(const std::string& indexName);
    std::string parseQuery(std::string query, std::vector<std::string>& words);
protected:
    
    std::map<std::string, std::set<PostingList>> zoneIndex;
public:
    ZoneIndex(const std::string& path);
    void generateZoneIndex();
    std::list<int> zoneIndexSearch(std::string query, std::unordered_map< std::string, std::string> zones);
   
};

