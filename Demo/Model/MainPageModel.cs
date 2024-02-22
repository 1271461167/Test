using Demo.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Demo.Model
{
    public class MainPageModel:NotifyBase
    {
		private double _value;

		public double Value
		{
			get { return _value; }
			set { _value = value;this.DoNotify(); }
		}

	}
}
