using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Demo.Model
{
    public class CourseSeries
    {
        public string Name { get; set; }
        public int CurrentValue { get; set; }
        public bool IsGrowing { get; set; }
        public int GrowRate { get; set; }
    }
}
