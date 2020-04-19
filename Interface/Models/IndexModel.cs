using System;
using System.Collections.Generic;
using System.Text;
using Engine;

namespace Interface
{
    class IndexModel
    {
        private Controller _controller;
        private String _indexingPath;

        public IndexModel(String path)
        {
            _controller = new Controller(); //creating controller which control all C++ code
            _controller.initIndex(path);  //this calls C++ dll which observe files in directory
        }

        public String IndexingPath{
            get { return _indexingPath; }
            set { _indexingPath = value; }
        }
    }
}
