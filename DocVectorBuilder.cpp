#include "DocVectorBuilder.h"
#include <cassert>


float DocVectorBuilder::scalarMultiply(MyVector& vec1, MyVector& vec2)
{

    MyVector::iterator it1 = vec1.begin();
    MyVector::iterator it2 = vec2.begin();
    float result = 0;
    assert(vec1.size() == vec2.size());
    for (int i = 0; i < vec1.size(); i++) {
        result += it1->_frequency * it2->_frequency;
        ++it1;
        ++it2;
    }
    return result;
}

float DocVectorBuilder::countCosSimilarity(MyVector vec1, MyVector vec2)
{
    castVectors(vec1, vec2);
    normalizeVector(vec1);
    normalizeVector(vec2);
    return scalarMultiply(vec1, vec2);
}

void DocVectorBuilder::saveVecModel(const std::string& path)
{
}

void DocVectorBuilder::addWordToVector(const std::string& word)
{
    std::pair<MyVector::iterator, bool> found = _vecSpaceModel[_vecSpaceModel.size()].insert(Term_freq(word, 1));
    if (!found.second) {
        found.first->_frequency += 1;
    }
}

void DocVectorBuilder::castVectors(MyVector& vec1, MyVector& vec2)
{
        MyVector::iterator it1 = vec1.begin();
        MyVector::iterator it2 = vec2.begin();
        while (it1 != vec1.end() && it2 != vec2.end()) {
            if (*it1 < *it2 || it2 == vec2.end()) {
                Term_freq buf(it1->_term, 0.0);
                vec2.insert(buf);
                ++it1;
            }
            else if (*it1 > * it2 || it1 == vec1.end()) {
                Term_freq buf(it2->_term, 0.0);
                vec1.insert(buf);
                ++it2;
            }
            else {
                ++it1;
                ++it2;
            }
        }
}

void DocVectorBuilder::normalizeVector(MyVector& vec)
{
    float vecLength = 0;
    for (auto& i : vec) {
        vecLength += i._frequency * i._frequency;
    }
    vecLength = sqrt(vecLength);
    for (auto& i : vec) {
        i._frequency = i._frequency / vecLength;
    }
}

DocVectorBuilder::DocVectorBuilder(const std::string& path) : Index(path), _directoryPath(path)
{
}

void DocVectorBuilder::generateVectorSpaceModel()
{
    std::ifstream file;
    for (auto i : number_filename) {
        file.open(_directoryPath + i.second);
        _vecSpaceModel.insert({ int(i.first), MyVector() });
        std::string word = "";
        char buf;
        while (!file.eof()) {
            buf = file.get();
            if (buf < 65 || (buf > 90 && buf < 97) || buf>122) {
                if (word != "") {
                    addWordToDictionary(invertedIndex, word, i.first);
                    addWordToVector(word);
                    word = "";
                }
            }
            else {
                word += buf;
            }
        }
        file.close();
    }
    vectorsNumber = _vecSpaceModel.size();
}

void DocVectorBuilder::loadVectorSpaceModel()
{
}
