
using Interface.Tools.DataStorage;
using Interface.Tools.Managers;
using Interface.Tools.MVVM;
using Interface.Tools.Navigation;
using System.Windows;

namespace Interface.ViewModel
{
    internal class MainWindowViewModel : BaseViewModel, IContentOwner
    {
        #region Fields
        private INavigatable _content;
        #endregion

        #region Properties

        public INavigatable Content
        {
            get { return _content; }
            set
            {
                _content = value;
                OnPropertyChanged();
            }
        }


        #endregion

        internal MainWindowViewModel()
        {
            NavigationManager.Instance.Initialize(new InitializationNavigationModel(this));
            StationManager.Initialize(new SerializedDataStorage());
            NavigationManager.Instance.Navigate(ViewType.EngineControl);
        }

    }
}
