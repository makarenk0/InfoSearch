﻿using System;
using System.Collections.Generic;
using System.Text;
using Engine;

namespace Interface
{
    class IndexModel
    {
        private Controller _controller;
        private String _indexingPath;
        private int _indexingPercentage;
        private UInt64 _indexingSizeLeft = 0;
        private UInt64 _indexingSizeLeftPrevious = 0;

        private UInt64 _indexingSizeDir = 0;
        private UInt64 _indexingSpeed = 0;



        public IndexModel(String path)
        {
            _controller = new Controller(); //creating controller which control all C++ code
            _controller.initIndex(path);  //this calls C++ dll which observe files in directory
        }

        public String IndexingPath{
            get { return _indexingPath; }
            set { _indexingPath = value; }
        }

        public int IndexingPercentage
        {
            get { return _indexingPercentage; }
            set { _indexingPercentage = value; }
        }
        public UInt64 IndexingSizeLeft
        {
            get { return _indexingSizeLeft; }
            set { _indexingSizeLeft = value; }
        }
        public UInt64 IndexingSpeed
        {
            get { return _indexingSpeed; }
            set { _indexingSpeed = value; }
        }

        public void InvertedIndexGenerate()
        {
           _controller.generateInvertedIndex();
        }
        public void BiwordIndexGenerate()
        {
            _controller.generateBiwordIndex();
        }
        public void PositionalIndexGenerate()
        {
            _controller.generatePositionalIndex();
        }


        private void IndexingPercentageUpdate()
        {
            UInt64 buf = (_indexingSizeDir - IndexingSizeLeft) * 100;
            IndexingPercentage = (int) (buf / (_indexingSizeDir)); 
        }
        public void IndexingSizeLeftUpdate()
        {
            IndexingSizeLeft = (UInt64)_controller.getIndexingSizeLeft();

            IndexingSpeed = _indexingSizeLeftPrevious - IndexingSizeLeft;

            _indexingSizeLeftPrevious = IndexingSizeLeft;
            IndexingPercentageUpdate();
        }

        public UInt64 IndexingDirSize
        {
            get {
                _indexingSizeDir = (UInt64)_controller.getIndexingDirSize();
                _indexingSizeLeftPrevious = _indexingSizeDir;
                return _indexingSizeDir; 
            }
        }
       

    }
}
