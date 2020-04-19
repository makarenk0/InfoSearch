#include "pch.h"
#include "Controller.h"

void Engine::Controller::initIndex(String^ path)
{
	std::string unmanaged = msclr::interop::marshal_as<std::string>(path);
	index = new ::Index(unmanaged);
}
