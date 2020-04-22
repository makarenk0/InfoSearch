#include "pch.h"
#include "Controller.h"

void Engine::Controller::initIndex(String^ path)
{
	std::string unmanaged = msclr::interop::marshal_as<std::string>(path);
	index = new ::Index(unmanaged);
}

void Engine::Controller::generateInvertedIndex()
{
	index->generateInvertedIndex();
}

void Engine::Controller::generateBiwordIndex()
{
	index->generateBiwordIndex();
}

void Engine::Controller::generatePositionalIndex()
{
	index->generatePositionalIndex();
}

UInt64 Engine::Controller::getIndexingDirSize()
{
	return index->getIndexingDirSize();
}

UInt64 Engine::Controller::getIndexingSizeLeft()
{
	return index->getIndexingSizeLeft();
}
