#pragma once
#include "DocVectorBuilder.h"
#include <ctime>
#include <queue>
#include <functional>
class ClusterBuilder
{
private:
	DocVectorBuilder _vecBuilder;
	struct Cluster {
	public:
		Cluster(std::map<int, DocVectorBuilder::MyVector>::iterator vec) : _leadingDoc(vec){}
		std::map<int, DocVectorBuilder::MyVector>::iterator _leadingDoc;	
		std::vector<std::map<int, DocVectorBuilder::MyVector>::iterator> _followers;
	};
	std::list<Cluster> _clusters;
	void printClustersInConsole();
public:
	ClusterBuilder(const std::string& path);
	void testCos();
	void generateClusters();
};

