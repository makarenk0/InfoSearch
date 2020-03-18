using Interface.Tools.MVVM;
using System;

namespace Interface.ViewModel
{
    class IndexViewModel : BaseViewModel
    {
        private IndexModel _model;
        private String _result;
        private RelayCommand<object> _proceedCommand;

        public String Result
        {
            get { return _result; }
            set { 
                _result = value;
                OnPropertyChanged();
            }
        }


        #region Commands
        public RelayCommand<object> ProceedCommand
        {
            get
            {
                return _proceedCommand ?? (_proceedCommand = new RelayCommand<object>(ProceedInplementation,
                    o => CanExecuteCommand()));
            }
        }

        private void ProceedInplementation(object obj)
        {
            _model = new IndexModel("Index\\");  // creating a model to test C++ then
            Result = "Successful";
            

        }
        #endregion

        private bool CanExecuteCommand()
        {
            return true;
        }
    }
}
