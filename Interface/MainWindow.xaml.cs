using Interface.ViewModel;
using System.Windows;


namespace Interface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            DataContext = new IndexViewModel();
            InitializeComponent();
        }
    }
}
