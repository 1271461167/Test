using Demo.Access;
using Demo.Model;
using LiveCharts;
using LiveCharts.Defaults;
using LiveCharts.Wpf;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Demo.ViewModel
{
    public class MainPageViewModel
    {
        public MainPageModel mainPageModel { get; set; }=new MainPageModel();
        public ObservableCollection<SeriesModel> SeriesModelList { get; set; }= new ObservableCollection<SeriesModel>();
        public MainPageViewModel()
        {
            mainPageModel.Value = 20;
            InitSeriesList();
        }
        private void InitSeriesList()
        {
            var seriesList = LocalDataAccess.GetInstance().GetCourseSeries();
            this.mainPageModel.ItemCount = seriesList.Max(c=>c.SeriesList.Count);
            foreach (var item in seriesList)
            {
                SeriesModelList.Add(item);
            }
        }
    }
}
