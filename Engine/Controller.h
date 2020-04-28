#pragma once

#include "Index.h"
#include <string>
#include <msclr\marshal_cppstd.h>
#include <cliext/list>

using namespace System;
using namespace System::Collections;

namespace Engine {
	public ref class Controller
	{
	private:
		::Index* index;
	public:
		void initIndex(String^ path, String^ savingPath);

		void generateInvertedIndex();	
		void generateBiwordIndex();
		void generatePositionalIndex();
		void generateIndexBySPIMI();

		UInt64 getIndexingDirSize();
		UInt64 getIndexingSizeLeft();

		Generic::List<String^>^ booleanSearch(String^ request);
	};
}
