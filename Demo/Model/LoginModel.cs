using Demo.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Demo.Model
{
    public class LoginModel:NotifyBase
    {
		private string _user_name;

		public string UserName
		{
			get { return _user_name; }
			set { _user_name = value;this.DoNotify(); }
		}

		private string _password;

		public string Password
		{
			get { return _password; }
			set 
			{
				_password = value;
				this.DoNotify();
			}
		}

		private string _info;

		public string Info
		{
			get { return _info; }
			set { _info = value;this.DoNotify(); }
		}
		private Visibility _showProgress=Visibility.Collapsed;

		public Visibility ShowProgress
		{
			get { return _showProgress; }
			set { _showProgress = value;this.DoNotify(); }
		}


	}
}
