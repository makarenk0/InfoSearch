#pragma once

#include "Index.h"
#include <string>
#include <msclr\marshal_cppstd.h>

using namespace System;

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
	};
}
