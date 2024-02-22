using Demo.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Demo.ViewModel
{
    public class MainPageViewModel
    {
        public MainPageModel mainPageModel { get; set; }=new MainPageModel();
        public MainPageViewModel()
        {
            mainPageModel.Value = 20;
        }

    }
}
