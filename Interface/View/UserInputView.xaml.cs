using Interface.Tools.Navigation;
using Interface.ViewModel;
using System.Windows.Controls;
using Interface.Tools;

namespace Interface.View
{
    /// <summary>
    /// Логика взаимодействия для UserInputView.xaml
    /// </summary>
    public partial class UserInputView : UserControl, INavigatable
    {
        public UserInputView()
        {
            DataContext = new UserInputViewModel();
            InitializeComponent();
        }
    }

}
