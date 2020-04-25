using System;
using System.IO;

namespace Interface.Tools
{
    internal static class FileFolderHelper
    {
        private static readonly string AppDataPath =
            Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);

        internal static readonly string AppFolderPath =
            Path.Combine(AppDataPath, "InfoRetrieval");

        internal static readonly string StorageFilePath =
            Path.Combine(AppFolderPath, "Storage.indexedpath");

        internal static bool CreateFolderAndCheckFileExistance(string filePath)
        {
            var file = new FileInfo(filePath);
            return CreateFolderAndCheckFileExistance(file);
        }

        internal static bool CreateFolderAndCheckFileExistance(this FileInfo file)
        {
            if (!file.Directory.Exists)
            {
                file.Directory.Create();
            }
            return file.Exists;
        }
        internal static String CreateOrPickIndexesFolder(int folderId)
        {
            String path = AppFolderPath + "\\Section" + folderId +"\\";
            if (Directory.Exists(path))
            {
                return path;
            }
            Directory.CreateDirectory(path);
            return path;
        }
    }
}