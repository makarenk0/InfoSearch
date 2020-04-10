#include "pch.h"
#include "Controller.h"

void Engine::Controller::initIndex(String^ path)
{
	msclr::interop::marshal_context context;
	std::string standardString = "Index\\";
	index = new ::Index(standardString);
	//throw gcnew System::NotImplementedException();
}
