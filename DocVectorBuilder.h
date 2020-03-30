#pragma once
#include "Index.h"
#include <unordered_set>

class DocVectorBuilder : Index
{

public:
	DocVectorBuilder(const std::string& path);
	void generateVectorSpaceModel();
	void loadVectorSpaceModel();
	

	struct Term_freq {
	public:
		Term_freq(std::string term, int frequency) : _term(term), _frequency(frequency) {}
		Term_freq(const Term_freq& other) : _term(other._term), _frequency(other._frequency) {}
	
		std::string _term;
		mutable float _frequency;
		bool operator==(const Term_freq& other) const {
			return _term == other._term;
		}
		bool operator!=(const Term_freq& other) const {
			return _term != other._term;
		}
		bool operator<(const Term_freq& other) const {
			return _term < other._term;
		}
		bool operator>(const Term_freq& other) const {
			return _term > other._term;
		}
	};
	typedef std::set<Term_freq> MyVector;
	float scalarMultiply(MyVector& vec1, MyVector& vec2);


	float countCosSimilarity(MyVector vec1, MyVector vec2);

	std::map<int, MyVector> _vecSpaceModel;
	int vectorsNumber = 0;
private:

	
	const std::string _directoryPath;

	
	void saveVecModel(const std::string& path);
	void addWordToVector(const std::string& word);
	void castVectors(MyVector& vec1, MyVector& vec2);
	void normalizeVector(MyVector& vec);
};

