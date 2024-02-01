using Demo.Access;
using Demo.Common;
using Demo.Model;
using Demo.View;
using MySql.Data.MySqlClient;
using System;
using System.Threading.Tasks;
using System.Windows;
namespace Demo.ViewModel
{
    public class LoginViewModel : CommandBase
    {
        public CommandBase LoginCommand { get; set; } = new CommandBase();
        public static LoginModel loginModel { get; set; } = new LoginModel();
        public LocalDataAccess access { get; set; } = LocalDataAccess.GetInstance();

        public LoginViewModel()
        {
            LoginCommand.DoCanExecute = new Func<object, bool>((obj) => { return true; });
            LoginCommand.DoExecute = new Action<object>((obj) => { DoLogin(obj); });
        }

        private async void DoLogin(object obj)
        {
            loginModel.ShowProgress=Visibility.Visible;
            Window loginView = obj as Window;
            loginModel.Info = "";
            if (string.IsNullOrEmpty(loginModel.UserName))
            {
                loginModel.Info = "用户名不能为空";
                loginModel.ShowProgress = Visibility.Collapsed;
                return;
            }
            if (string.IsNullOrEmpty(loginModel.Password))
            {
                loginModel.Info = "密码不能为空";
                loginModel.ShowProgress = Visibility.Collapsed;
                return;
            }
            try
            {
                MySqlParameter[] mySqlParameters = new MySqlParameter[]
                {
                    new MySqlParameter("@username",loginModel.UserName),
                    new MySqlParameter("@password",loginModel.Password)
                 };
                UserRole user=new UserRole();
                await Task.Run(() => {user=access.CheckUserInfo(mySqlParameters); });
                if (user == null) { loginModel.Info = "账号或密码错误";return; }
                GlobalValues.role = user;
                loginView.DialogResult = true;
            }
            catch (Exception ex) 
            {
                loginModel.Info = ex.Message;
                loginModel.ShowProgress = Visibility.Collapsed;
                return;
            }
        }
    }
}
