using Interface.Tools;
using Interface.Tools.Navigation;
using Interface.ViewModel;
using System.Windows;
using System.Windows.Controls;

namespace Interface.View
{
    /// <summary>
    /// Логика взаимодействия для UserView.xaml
    /// </summary>
    public partial class EngineControlView : UserControl, INavigatable
    {
        public EngineControlView()
        {
            DataContext = new EngineControlViewModel();
            InitializeComponent();
        }
    }

    public class LoadBoolToVisibilityConverter : BoolToValueConverter<Visibility>
    {
        #region Constructors and Destructors
        public LoadBoolToVisibilityConverter()
        {
            this.TrueValue = Visibility.Visible;
            this.FalseValue = Visibility.Hidden;
        }
        #endregion
    }
}
