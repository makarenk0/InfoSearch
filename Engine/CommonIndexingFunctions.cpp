#include "pch.h"
#include "CommonIndexingFunctions.h"


unsigned long long CommonIndexingFunctions::enumerateFilesInDir(const std::string& directoryPath, std::map<short, fs::directory_entry>& number_filename)
{
    int filesCounter = 1;
    unsigned long long wholeSize = 0;

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            wholeSize += fs::file_size(entry);
            number_filename.insert({ filesCounter, entry });
            ++filesCounter;
        }
    }
    return wholeSize;
}