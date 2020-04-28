using Interface.Tools;
using Interface.Tools.Managers;
using Interface.Tools.MVVM;
using Interface.Tools.Navigation;
using Microsoft.WindowsAPICodePack.Dialogs;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading;
using System.Threading.Tasks;



namespace Interface.ViewModel
{
    class EngineControlViewModel : BaseViewModel
    {
        private IndexModel _model;
        private delegate void IndexGeneratingFunction(); 
        private bool _coreIsFree = true;
        private int _spinnerPosition = 0;
        private RelayCommand<object> _addNewDirCommand;
        private RelayCommand<object> _generateIndexCommand;
        private RelayCommand<object> _goToSearchingCommand;

        private TimeSpan _indexingTimer;
        private Dictionary<int, IndexGeneratingFunction> _indexGenFunctions;
        private bool[] _loadStatus;
        private TimeSpan[] _indexesTimeSpent; // = { new TimeSpan(0), new TimeSpan(0), new TimeSpan(0), new TimeSpan(0) };

        private ObservableCollection<String> _indexedPaths;


       
        public EngineControlViewModel()
        {
            
            _indexedPaths = new ObservableCollection<String>(StationManager.DataStorage.PathsList);
            _indexGenFunctions = new Dictionary<int, IndexGeneratingFunction>();
            _loadStatus = new bool[4];
            _indexesTimeSpent = new TimeSpan[4];

            IndexGeneratingFunction function0 = InvertedIndexGenerate;
            IndexGeneratingFunction function1 = BiwordIndexGenerate;
            IndexGeneratingFunction function2 = PositionalIndexGenerate;
            IndexGeneratingFunction function3 = IndexBySPIMIGenerate;
            //IndexGeneratingFunction function4 = _model.InvertedIndexGenerate;
            _indexGenFunctions.Add(0, function0);
            _indexGenFunctions.Add(1, function1);
            _indexGenFunctions.Add(2, function2);
            _indexGenFunctions.Add(3, function3);
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
        private void IndexBySPIMIGenerate()
        {
            _model.IndexBySPIMIGenerate();
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
            set { 
                _model.IndexingSizeLeft = value;
                OnPropertyChanged();
            }
        }
        public UInt64 IndexingSpeed
        {
            get { return _model?.IndexingSpeed / 1000 ?? 0; }
            set { _model.IndexingSpeed = value; }
        }

        public String IndexingPath
        {
            get { return _model?.IndexingPath ?? ""; }
            set {
                ChooseIndexingDir(value);
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
            set
            {
                _indexingTimer = TimeSpan.Parse(value);
                OnPropertyChanged();
            }
        }

        public TimeSpan[] IndexesTimeSpent
        {
            get { return _indexesTimeSpent; }
        }

        public bool[] LoadStatus
        {
            get
            {
                return _loadStatus;
            }
            set
            {
                _loadStatus = value;
                OnPropertyChanged();
            }
        }

        public ObservableCollection<String> IndexedPathsList
        {
            get { return _indexedPaths; }
            private set
            {
                _indexedPaths = value;
                OnPropertyChanged();
            }
        }

        public RelayCommand<object> ChooseDirCommand
        {
            get
            {
                return _addNewDirCommand ?? (_addNewDirCommand = new RelayCommand<object>(AddNewDirInplementation,
                    o => CoreIsFree ));
            }
        }

        public RelayCommand<object> GenerateIndexCommand
        {
            get
            {
                return _generateIndexCommand ?? (_generateIndexCommand = new RelayCommand<object>(IndexGenInplementation,
                    o => CoreIsFree && !String.IsNullOrEmpty(IndexingPath)));
            }
        }

        public RelayCommand<object> GoToSearchingCommand
        {
            get
            {
                return _goToSearchingCommand ?? (_goToSearchingCommand = new RelayCommand<object>(GoToSearchingInplementation,
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

        private void AddNewDirInplementation(object obj)
        {

            var dialog = new CommonOpenFileDialog();
            dialog.IsFolderPicker = true;
            CommonFileDialogResult result = dialog.ShowDialog();
            if (result.ToString() == "Ok")
            {
                String newPath = dialog.FileName.ToString();
                StationManager.DataStorage.AddPath(newPath);
                IndexedPathsList = new ObservableCollection<String>(StationManager.DataStorage.PathsList);
                IndexingPath = newPath;
            }
           
        }

        private void ChooseIndexingDir(String path)
        {
            LoadStatus = new bool[4];
            _indexesTimeSpent = new TimeSpan[4];
            int id = StationManager.DataStorage.GetPathNum(path);
            _model = new IndexModel(path, FileFolderHelper.CreateOrPickIndexesFolder(id));  // creating a model to test C++ then
            ViewModelsSynchronizer._indexModel = _model;
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
            SpinnerPosition = functionNum + 1;
            await Task.Run(() => {
                _indexGenFunctions[functionNum]();
                timer.Dispose();
                CoreIsFree = true;
                SaveAndResetStatistics(functionNum);
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

        private void SaveAndResetStatistics(int functionNum)
        {
            _loadStatus[functionNum] = true;
            _indexesTimeSpent[functionNum] = _indexingTimer;
            OnPropertyChanged("LoadStatus");
            OnPropertyChanged("IndexesTimeSpent");
            SizeLeft = 0;
            IndexingPercentage = 100;
            IndexingSpeed = 0;
            IndexingTime = "0:0:0";
        }

        private void GoToSearchingInplementation(object obj)
        {
            NavigationManager.Instance.Navigate(ViewType.UserInput);
        }
    }
}
