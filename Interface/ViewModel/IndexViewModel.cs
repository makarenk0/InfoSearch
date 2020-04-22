using Interface.Tools.MVVM;
using Microsoft.WindowsAPICodePack.Dialogs;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace Interface.ViewModel
{
    class IndexViewModel : BaseViewModel
    {
        private IndexModel _model;
        private delegate void IndexGeneratingFunction(); 
        private bool _coreIsFree = true;
        private int _spinnerPosition = 0;
        private RelayCommand<object> _chooseDirCommand;
        private RelayCommand<object> _invertedIndexCommand;

        private TimeSpan _indexingTimer;
        private Dictionary<int, IndexGeneratingFunction> _indexGenFunctions;


       
        public IndexViewModel()
        {
            _indexGenFunctions = new Dictionary<int, IndexGeneratingFunction>();

            IndexGeneratingFunction function0 = InvertedIndexGenerate;
            IndexGeneratingFunction function1 = BiwordIndexGenerate;
            IndexGeneratingFunction function2 = PositionalIndexGenerate;
            //IndexGeneratingFunction function3 = _model.InvertedIndexGenerate;
            //IndexGeneratingFunction function4 = _model.InvertedIndexGenerate;
            _indexGenFunctions.Add(0, function0);
            _indexGenFunctions.Add(1, function1);
            _indexGenFunctions.Add(2, function2);

            //_indexGenFunctions.Add(3, function3);
            //_indexGenFunctions.Add(4, function4);
        }

        #region FunctionsForDelegating
        private void InvertedIndexGenerate()
        {
            _model.InvertedIndexGenerate();
        }
        private void BiwordIndexGenerate()
        {
            _model.BiwordIndexGenerate();
        }
        private void PositionalIndexGenerate()
        {
            _model.PositionalIndexGenerate();
        }
        #endregion

        #region PropertiesAndCommandsForView
        public UInt64 WholeDirSize
        {
            get { return _model?.IndexingDirSize / 1000000 ?? 0; }
        }
        public UInt64 SizeLeft
        {
            get { return _model?.IndexingSizeLeft / 1000000 ?? 0; }
        }
        public UInt64 IndexingSpeed
        {
            get { return _model?.IndexingSpeed / 1000 ?? 0; }
        }

        public String IndexingPath
        {
            get { return _model?.IndexingPath ?? ""; }
            set {
                _model.IndexingPath = value;
                OnPropertyChanged();
            }
        }

        public int IndexingPercentage
        {
            get { return _model?.IndexingPercentage ?? 0; }
            set {
                _model.IndexingPercentage = value;
                OnPropertyChanged();
            }
        }

        public String IndexingTime
        {
            get { return _indexingTimer.ToString(); }
        }

        public RelayCommand<object> ChooseDirCommand
        {
            get
            {
                return _chooseDirCommand ?? (_chooseDirCommand = new RelayCommand<object>(ChooseDirInplementation,
                    o => CoreIsFree ));
            }
        }

        public RelayCommand<object> InvertedIndexCommand
        {
            get
            {
                return _invertedIndexCommand ?? (_invertedIndexCommand = new RelayCommand<object>(IndexGenInplementation,
                    o => CoreIsFree && !String.IsNullOrEmpty(IndexingPath)));
            }
        }

        

        public String SpinnerVisibility
        {
            get { return CoreIsFree ? "Hidden" : "Visible"; }
        }

        public int SpinnerPosition
        {
            get { return _spinnerPosition; }
            set { 
                _spinnerPosition = value;
                OnPropertyChanged();
            }
        }
        #endregion


        private bool CoreIsFree
        {
            get { return _coreIsFree; }
            set
            {
                _coreIsFree = value;
                OnPropertyChanged("SpinnerVisibility");
            }
        }

        private void ChooseDirInplementation(object obj)
        {

            var dialog = new CommonOpenFileDialog();
            dialog.IsFolderPicker = true;
            CommonFileDialogResult result = dialog.ShowDialog();
            if (result.ToString() == "Ok")
            {
               _model = new IndexModel(dialog.FileName.ToString());  // creating a model to test C++ then
                IndexingPath = dialog.FileName.ToString();
            }
            OnPropertyChanged("WholeDirSize");
        }

        private async void IndexGenInplementation(object obj)
        {
            String buf = obj as String;
            int functionNum = Int32.Parse(buf);

            _indexingTimer = new TimeSpan(0, 0, 0);
            TimerCallback tm = new TimerCallback(IndexingPercentageIncrease); 
            Timer timer = new Timer(tm, 0, 1000, 1000);
            

            CoreIsFree = false;
            SpinnerPosition = functionNum + 3;
            await Task.Run(() => {
                _indexGenFunctions[functionNum]();
                timer.Dispose();
                CoreIsFree = true;
            });
            
        }

        private void IndexingPercentageIncrease(object obj)
        {
            _indexingTimer += new TimeSpan(0, 0, 1);
            _model.IndexingSizeLeftUpdate();
            OnPropertyChanged("IndexingTime");
            OnPropertyChanged("SizeLeft");
            OnPropertyChanged("IndexingPercentage");
            OnPropertyChanged("IndexingSpeed");
        }
    }
}
