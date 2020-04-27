using Interface.Tools.Managers;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Interface.Tools.DataStorage
{
    internal class SerializedDataStorage : IDataStorage
    {
        private readonly Dictionary<String, int> _indexedDirs;
      

        internal SerializedDataStorage()
        {
            try
            {
                _indexedDirs = SerializationManager.Deserialize<Dictionary<String, int>>(FileFolderHelper.StorageFilePath);
            }
            catch (FileNotFoundException)
            {
                _indexedDirs = new Dictionary<String, int>();
            }
        }


        public int GetPathNum(String path)
        {
            return _indexedDirs[path];
        }

        public void AddPath(String path)
        {
            if (_indexedDirs.Values.Count() == 0) { _indexedDirs.Add(path, 1); }
            else {
                if (!_indexedDirs.ContainsKey(path))
                {
                    _indexedDirs.Add(path, _indexedDirs.Values.Max() + 1);
                }
                else { return; }
            }
            SaveChanges();
        }

        public void DeletePath(String path)
        {
            _indexedDirs.Remove(path);
            SaveChanges();
        }

        public List<String> PathsList
        {
            get { return _indexedDirs.Keys.ToList(); }
        }

        private void SaveChanges()
        {
            SerializationManager.Serialize(_indexedDirs, FileFolderHelper.StorageFilePath);
        }

    }
}

