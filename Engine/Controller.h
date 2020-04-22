#pragma once

#include "SPIMI.h"
#include <string>
#include <msclr\marshal_cppstd.h>

using namespace System;

namespace Engine {
	public ref class Controller
	{
	private:
		::Index* index;
	public:
		void initIndex(String^ path);

		void generateInvertedIndex();	
		void generateBiwordIndex();
		void generatePositionalIndex();

		UInt64 getIndexingDirSize();
		UInt64 getIndexingSizeLeft();
	};
}
