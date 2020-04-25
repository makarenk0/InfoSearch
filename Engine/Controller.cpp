#include "pch.h"
#include "Controller.h"

void Engine::Controller::initIndex(String^ path, String^ savingPath)
{
	std::string unmanagedPath = msclr::interop::marshal_as<std::string>(path);
	std::string unmanagedSavingPath = msclr::interop::marshal_as<std::string>(savingPath);
	index = new ::Index(unmanagedPath, unmanagedSavingPath);
}

void Engine::Controller::generateInvertedIndex()
{
	index->loadIndex(::Index::IndexName::inverted);
}

void Engine::Controller::generateBiwordIndex()
{
	index->loadIndex(::Index::IndexName::biword);
}

void Engine::Controller::generatePositionalIndex()
{
	index->loadIndex(::Index::IndexName::positional);
}

void Engine::Controller::generateIndexBySPIMI()
{
	index->generateBySPIMICompressedIndex();
}

UInt64 Engine::Controller::getIndexingDirSize()
{
	return index->getIndexingDirSize();
}

UInt64 Engine::Controller::getIndexingSizeLeft()
{
	return index->getIndexingSizeLeft();
}
