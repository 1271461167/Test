using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace Demo.Common
{
    public class PasswordHelper
    {

        static bool _isUpdating = false;
        public static string GetPassword(DependencyObject obj)
        {
            return (string)obj.GetValue(PasswordProperty);
        }

        public static void SetPassword(DependencyObject obj, string value)
        {
            obj.SetValue(PasswordProperty, value);
        }

        // Using a DependencyProperty as the backing store for MyProperty.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty PasswordProperty =
            DependencyProperty.RegisterAttached("Password", typeof(string), typeof(PasswordHelper), new FrameworkPropertyMetadata(default(string),new PropertyChangedCallback(OnPropertyChanged)));

        private static void OnPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            PasswordBox passwordBox=d as PasswordBox;
            passwordBox.PasswordChanged -= PasswordBox_PasswordChanged;
            if (!_isUpdating)
            passwordBox.Password = e.NewValue.ToString(); 
            passwordBox.PasswordChanged += PasswordBox_PasswordChanged;
        }



        public static readonly DependencyProperty AttachedProperty =
           DependencyProperty.RegisterAttached("Attached", typeof(bool), typeof(PasswordHelper), new FrameworkPropertyMetadata(default(bool), new PropertyChangedCallback(OnAttachedChanged)));

        private static void OnAttachedChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            PasswordBox passwordBox = d as PasswordBox;
            passwordBox.PasswordChanged += PasswordBox_PasswordChanged;
        }

        private static void PasswordBox_PasswordChanged(object sender, RoutedEventArgs e)
        {
            PasswordBox password = (PasswordBox)sender;
            _isUpdating = true;
            SetPassword(password,password.Password);
            _isUpdating=false;
        }

        public static bool GetAttached(DependencyObject obj)
        {
            return (bool)obj.GetValue(AttachedProperty);
        }

        public static void SetAttached(DependencyObject obj, bool value)
        {
            obj.SetValue(AttachedProperty, value);
        }
    }
}
