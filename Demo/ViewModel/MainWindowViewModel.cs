using Demo.Common;
using Demo.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Demo.ViewModel
{
    public class MainWindowViewModel:CommandBase
    {
        public static UserModel User { get; set; }=new UserModel();
        public static MainWindowModel ViewModel { get; set; }= new MainWindowModel();
        public CommandBase NavChangedCommand { get; set; }=new CommandBase();
        public MainWindowViewModel() 
        {
            NavChangedCommand.DoCanExecute = new Func<object, bool>((obj) => { return true; });
            NavChangedCommand.DoExecute = new Action<object>((obj) => { DoNavChanged(obj); });
            DoNavChanged("MainPage");
        }

        private void DoNavChanged(object obj)
        {
            Type type=Type.GetType("Demo.View."+obj.ToString());
            object oframe = Activator.CreateInstance(type);
            FrameworkElement frameworkElement = (FrameworkElement)oframe;
            ViewModel.MainContent = frameworkElement;
        }
    }
}
