using Interface.Tools;
using System.Windows;
using System.Windows.Controls;

namespace Interface.View
{
    /// <summary>
    /// Логика взаимодействия для UserView.xaml
    /// </summary>
    public partial class UserView : UserControl
    {
        public UserView()
        {
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
