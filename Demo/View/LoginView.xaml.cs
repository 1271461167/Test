using Demo.Model;
using Demo.ViewModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Demo.View
{
    /// <summary>
    /// LoginView.xaml 的交互逻辑
    /// </summary>
    public partial class LoginView : Window
    {
        public LoginView()
        {
            InitializeComponent();
            this.DataContext = new LoginViewModel();
        }
    }
}
