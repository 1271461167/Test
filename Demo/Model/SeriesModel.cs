using Demo.Common;
using LiveCharts;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Demo.Model
{
    public class SeriesModel:NotifyBase
    {
		public string Name { get; set; }

		public SeriesCollection Series {  get; set; }

		public ObservableCollection<CourseSeries> SeriesList { get; set; }
	}
}
