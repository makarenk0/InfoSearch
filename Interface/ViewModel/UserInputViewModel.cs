using Interface.Tools;
using Interface.Tools.Managers;
using Interface.Tools.MVVM;
using Interface.Tools.Navigation;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;

namespace Interface.ViewModel
{
    class UserInputViewModel : BaseViewModel
    {
        private RelayCommand<object> _goToEngineControlCommand;
        private RelayCommand<object> _searchCommand;
        private RelayCommand<object> _openFileCommand;

        private List<String> _requestResults;

        public List<String> RequestResults
        {
            get {
                
                return _requestResults; 
            }
            set 
            { 
                _requestResults = value;
                OnPropertyChanged();
            }
        }

        public List<String> FoundFileNames
        {
            get
            {
                List<String> _foundFileNames = new List<String>();
                foreach (var path in _requestResults)
                {
                    _foundFileNames.Add(path.Substring(path.LastIndexOf('\u005c') + 1));
                }
                return _foundFileNames;
            }
        }


        public UserInputViewModel()
        {
            _requestResults = new List<String>();
        }
        #region PropertiesAndCommandsForView
        public String SearchInput
        {
            set { ViewModelsSynchronizer._indexModel.SearchRequest = value; }
        }


        public RelayCommand<object> GoToEngineControl
        {
            get
            {
                return _goToEngineControlCommand ?? (_goToEngineControlCommand = new RelayCommand<object>(GoToEngineControlImp,
                    o => true));
            }
        }

        public RelayCommand<object> SearchCommand
        {
            get
            {
                return _searchCommand ?? (_searchCommand = new RelayCommand<object>(SearchImp,
                    o => true));
            }
        }
        public RelayCommand<object> OpenFileCommand
        {
            get
            {
                return _openFileCommand ?? (_openFileCommand = new RelayCommand<object>(OpenFileImp,
                    o => true));
            }
        }





        #endregion

        private void GoToEngineControlImp(object obj)
        {
            NavigationManager.Instance.Navigate(ViewType.EngineControl);
        }

        private async void SearchImp(object obj)
        {
            await Task.Run(() =>
            {
                ViewModelsSynchronizer._indexModel.BooleanSearch();
                RequestResults = ViewModelsSynchronizer._indexModel.SearchResults;
            });
        }   
        
        private void OpenFileImp(object obj)
        {
            if (File.Exists(obj.ToString()))
            {
                Process.Start(new ProcessStartInfo("explorer.exe", obj.ToString()));
            }
        }




    }
}
