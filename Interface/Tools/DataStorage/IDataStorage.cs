using System;
using System.Collections.Generic;

namespace Interface.Tools.DataStorage
{
    internal interface IDataStorage
    {
        void AddPath(String path);

        int GetPathNum(String path);

        void DeletePath(String path);
    }
}
