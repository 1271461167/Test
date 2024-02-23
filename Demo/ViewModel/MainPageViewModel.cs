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
        public ObservableCollection<SeriesModel> SeriesModelList { get; set; }=new ObservableCollection<SeriesModel>();
        public MainPageViewModel()
        {
            mainPageModel.Value = 20;
            InitSeriesList();
        }
        private void InitSeriesList()
        {
            SeriesModelList.Add(new SeriesModel 
            {
                Name="123",
                Series=new LiveCharts.SeriesCollection {
                    new PieSeries { 
                        Title="111",
                        Values=new ChartValues<ObservableValue> { new ObservableValue(100) },
                        DataLabels=false
                    },
                    new PieSeries {
                        Title="222",
                        Values=new ChartValues<ObservableValue> { new ObservableValue(123) },
                        DataLabels=false
                    }
                },
                SeriesList=new ObservableCollection<CourseSeries>
                {
                    new CourseSeries {Name="云课堂",CurrentValue=24,IsGrowing=false,GrowRate=4},
                    new CourseSeries {Name="B站",CurrentValue=24,IsGrowing=true,GrowRate=4},
                    new CourseSeries {Name="云课堂",CurrentValue=24,IsGrowing=false,GrowRate=4}
                }
                });
            SeriesModelList.Add(new SeriesModel
            {
                Name = "123",
                Series = new LiveCharts.SeriesCollection {
                    new PieSeries {
                        Title="111",
                        Values=new ChartValues<ObservableValue> { new ObservableValue(23) },
                        DataLabels=false
                    },
                    new PieSeries {
                        Title="222",
                        Values=new ChartValues<ObservableValue> { new ObservableValue(123) },
                        DataLabels=false
                    }
                },
                SeriesList = new ObservableCollection<CourseSeries>
                {
                    new CourseSeries {Name="云课堂",CurrentValue=24,IsGrowing=false,GrowRate=4},
                    new CourseSeries {Name="B站",CurrentValue=24,IsGrowing=true,GrowRate=4},
                    new CourseSeries {Name="云课堂",CurrentValue=24,IsGrowing=false,GrowRate=4}
                }
            });
        }
    }
}
