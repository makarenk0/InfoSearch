#include "ClusterBuilder.h"


void ClusterBuilder::printClustersInConsole()
{
	for (auto& i : _clusters) {
		std::cout << "Leading doc id:" << i._leadingDoc->first << std::endl; 
		std::cout << "Followers: ";
		for (auto& j : i._followers) {
	    	std::cout << j->first <<" , ";
		}
		std::cout << std::endl<<std::endl;
	}
}

ClusterBuilder::ClusterBuilder(const std::string& path) : _vecBuilder(path)
{
	_vecBuilder.generateVectorSpaceModel();
}

void ClusterBuilder::testCos()
{
	DocVectorBuilder::MyVector vec1 = _vecBuilder._vecSpaceModel[3];
	DocVectorBuilder::MyVector vec2 = _vecBuilder._vecSpaceModel[2];
	float sim = _vecBuilder.countCosSimilarity(vec1, vec2);
}

void ClusterBuilder::generateClusters()
{
	srand(time(0));
	int numberLeadDocs = sqrt(_vecBuilder.vectorsNumber);
	std::set<int> leadNumbers;
	for (int i = 0; i < numberLeadDocs; i++) {
		int docNumber = rand() % _vecBuilder.vectorsNumber + 1;
		leadNumbers.insert(docNumber);
		Cluster newClaster(_vecBuilder._vecSpaceModel.find(docNumber));
		_clusters.push_back(newClaster);
	}

	std::map<int, DocVectorBuilder::MyVector>::iterator it;
	for (it = _vecBuilder._vecSpaceModel.begin(); it != _vecBuilder._vecSpaceModel.end(); it++) {

		if (!leadNumbers.empty()&&it->first == *leadNumbers.begin()) {
			leadNumbers.erase(leadNumbers.begin());
			continue;
		}

		std::pair<float, std::map<int, DocVectorBuilder::MyVector>::iterator> maxSim(0, it);
		std::list<Cluster>::iterator itCluster = _clusters.begin();
		std::list<Cluster>::iterator suitableCluster;
		for (itCluster = _clusters.begin(); itCluster != _clusters.end(); itCluster++) {
			float similarity = _vecBuilder.countCosSimilarity(it->second, itCluster->_leadingDoc->second);
			if (similarity > maxSim.first) {
				maxSim.first = similarity;
				maxSim.second = itCluster->_leadingDoc;
				suitableCluster = itCluster;
			}
		}
		suitableCluster->_followers.push_back(it);
	}
	printClustersInConsole();
}
