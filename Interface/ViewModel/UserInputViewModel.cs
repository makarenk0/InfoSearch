using Interface.Tools;
using Interface.Tools.Managers;
using Interface.Tools.MVVM;
using Interface.Tools.Navigation;
using System;
using System.Threading.Tasks;

namespace Interface.ViewModel
{
    class UserInputViewModel : BaseViewModel
    {
        private RelayCommand<object> _goToEngineControlCommand;
        private RelayCommand<object> _searchCommand;

        private String _testResults;

        public String TestResults
        {
            get { return _testResults; }
        }


        public UserInputViewModel()
        {

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
                _testResults = "";
                foreach (String docName in ViewModelsSynchronizer._indexModel.SearchResults)
                {
                    _testResults += docName + "\n";
                }
                OnPropertyChanged("TestResults");
            });
        }      




    }
}
