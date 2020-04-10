#include "ZoneIndex.h"


void ZoneIndex::fillZoneIndex(tinyxml2::XMLElement* start, tinyxml2::XMLElement* lastEl, const int& docId) {
    int i = 0;
    do {
        if (i != 0) {
            start = start->NextSiblingElement();
        }
        if (start->FirstChild()->NoChildren()) {
            std::string line = start->GetText();
            //--------------------Word unparse
            stringToWords(line, docId);
            //----------------------
        }
        else {
            fillZoneIndex(start->FirstChildElement(), start->LastChildElement(), docId);
        }
        ++i;
    } while (start != lastEl);
}

void ZoneIndex::insertWord(const std::string& word, const int& docId)
{
    std::set<PostingList> arr;
    arr.insert({ docId, {1}, checkZones(word) });
    std::pair<std::map<std::string, std::set<PostingList>>::iterator, bool> foundWord = zoneIndex.insert({ word, arr });
    if (!foundWord.second) {
        std::pair<std::set<PostingList>::iterator, bool> foundDocId = foundWord.first->second.insert({ docId, {1}, checkZones(word) });
        if (!foundDocId.second) {
            (*foundDocId.first).frequencies[0] += 1;
        }
    }
}

tinyxml2::XMLElement* ZoneIndex::findBody(tinyxml2::XMLElement* start)
{
    start = start->FirstChildElement();
    start = start->NextSiblingElement();
    while (std::string(start->Value()) != "body") {
        start = start->FirstChildElement();
    }
    return start;
}

void ZoneIndex::readZones(tinyxml2::XMLElement* start)
{
    for (int i = 0; i < numberOfZones; i++) {
        tinyxml2::XMLElement* searchNode = start->FirstChildElement();
        while (std::string(searchNode->Value()) != _zones[i]) {
            searchNode = searchNode->NextSiblingElement();
        }
        std::string line = searchNode->GetText();
        while (!line.empty()) {
            std::string buf = "";
            readWord(line, buf);
            _currentDocZones[i].push_back(buf);
        }
         
    }
}

char ZoneIndex::checkZones(const std::string& word)
{
    char result = 0;
    for (int i = 0; i < numberOfZones; i++) {
        result = (result << 1);
        for (auto j : _currentDocZones[i]) {
            if (word == j) {
                result |= 1;
            }
        }
    }
    return result;
}

void ZoneIndex::stringToWords(std::string line, const int& docId)
{
    std::string word = "";
    for (int j = 0; j < line.length(); j++) {
        char buf = line.at(j);
        if (buf < 65 || (buf > 90 && buf < 97) || buf>122) {
            if (word != "") {
                insertWord(word, docId);
                word = "";
            }
        }
        else {
            word += buf;
        }
    }
}

tinyxml2::XMLElement* ZoneIndex::findMetaData(tinyxml2::XMLElement* start)
{
    while (std::string(start->Value())!="title-info") {
        start = start->FirstChildElement();
    }
    return start;
}

ZoneIndex::ZoneIndex(const std::string& path) : Index(path)
{
}

void ZoneIndex::generateZoneIndex()
{
    for (auto i : number_filename) {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(i.second.path().string().c_str());

        tinyxml2::XMLElement* startNode = doc.FirstChildElement();
        tinyxml2::XMLElement* metaNode = findMetaData(startNode);
        tinyxml2::XMLElement* bodyNode = findBody(startNode);
        tinyxml2::XMLElement* endBodyNode = bodyNode->LastChildElement();

        readZones(metaNode);
        for (int c = 0; c < numberOfZones; c++) {  //adding zones as words in index
            std::string zone = "";
            for (auto j : _currentDocZones[c]) {
                zone += j+" ";
            }
            stringToWords(zone, i.first);
            _currentDocZones[c].clear();
        }
       
        fillZoneIndex(bodyNode->FirstChildElement(), endBodyNode, i.first);
    }
    printInFile("zoneIndex");
}

std::list<int> ZoneIndex::zoneIndexSearch(std::string query, std::unordered_map<std::string, std::string> zones)
{
    std::vector<std::string> words;
    std::string parsedQuery = parseQuery(query, words);
    
    std::set<PostingList> searchResult = booleanSearchGeneral(parsedQuery, zoneIndex);
    std::vector<DocId_Relevance> docIdsRaw;
    for (auto i : searchResult) {
        docIdsRaw.push_back({ i.docId, 0 });
    }

    int counter = -1;
    for (auto j : zones) {
        ++counter;
        parsedQuery = parseQuery(j.second, words);
        std::set<PostingList> searchZones = booleanSearchGeneral(parsedQuery, zoneIndex);
        if (searchZones.empty()) continue;  //skipping zone if there are no matches

        int c = -1;
        for (auto &k : searchResult) { 
            ++c;
            std::set<PostingList> res = andSearch(std::set<PostingList>({ k }), searchZones);
            if (res.empty()) continue;
            if ((res.begin()->zoneMask >> (numberOfZones - 1 - counter)) & 1) {
                docIdsRaw[c].relevance += _zoneWeights[counter];
            }
               
        }
    }

    std::set <std::pair < float , int >> sortedByRelevance;
    for (auto &i : docIdsRaw) {
        sortedByRelevance.insert({ i.relevance, i.docId});
    }

    std::list<int> docIds;
    for (auto &i : sortedByRelevance) {
        docIds.push_front(i.second);
    }
    return docIds;
}

void ZoneIndex::printInFile(const std::string& indexName) {
    std::ofstream out;
    out.open("Index\\" + indexName + ".txt");
    for (auto i : zoneIndex) {
        out << i.first << " ";
        for (auto &j : i.second) {
            out << " " << j.docId << " " << j.frequencies[0] << " " << j.zoneMask;
        }
        out << std::endl;
    }
    out << std::endl;
    out.close();
}

std::string ZoneIndex::parseQuery(std::string query, std::vector<std::string>& words)
{
    std::string parsedQuery = "";
    std::string word = "";
    while (!query.empty()) {
        readWord(query, word);
        words.push_back(word);
        parsedQuery += word + " AND ";
    }
    parsedQuery = parsedQuery.substr(0, parsedQuery.length() - 5);
    return parsedQuery;
}




