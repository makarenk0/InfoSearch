using Interface.Tools.MVVM;
using Microsoft.WindowsAPICodePack.Dialogs;
using System;
using System.Threading;

namespace Interface.ViewModel
{
    class IndexViewModel : BaseViewModel
    {
        private IndexModel _model;
        private int _percentage = 0;
        private RelayCommand<object> _chooseDirCommand;


        TimerCallback tm;
        Timer timer;
        public IndexViewModel()
        {
            tm = new TimerCallback(IncreaseNum);
            timer = new Timer(tm, _percentage, 0, 1000);
        }

        private void IncreaseNum(object state)
        {
            Value1 += 1;
            if(Value1 == 100)
            {
                Value1 = 0;
            }
        }

        public String IndexingPath
        {
            get { return _model?.IndexingPath ?? "none"; }
            set {
                _model.IndexingPath = value;
                OnPropertyChanged();
            }
        }

        public int Value1
        {
            get { return _percentage; }
            set { 
                _percentage = value;
                OnPropertyChanged();
            }
        }

        #region Commands
        public RelayCommand<object> ChooseDirCommand
        {
            get
            {
                return _chooseDirCommand ?? (_chooseDirCommand = new RelayCommand<object>(ChooseDirInplementation,
                    o => true));
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
        }
        #endregion
    }
}
