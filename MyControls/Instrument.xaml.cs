using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace MyControls
{
    /// <summary>
    /// Instrument.xaml 的交互逻辑
    /// </summary>
    public partial class Instrument : UserControl
    {



        public int Min
        {
            get { return (int)GetValue(MinProperty); }
            set { SetValue(MinProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Min.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty MinProperty =
            DependencyProperty.Register("Min", typeof(int), typeof(Instrument), new PropertyMetadata(default(int), new PropertyChangedCallback(OnPropertyChanged)));



        public int Max
        {
            get { return (int)GetValue(MaxProperty); }
            set { SetValue(MaxProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Max.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty MaxProperty =
            DependencyProperty.Register("Max", typeof(int), typeof(Instrument), new PropertyMetadata(default(int), new PropertyChangedCallback(OnPropertyChanged)));



        public double Value
        {
            get { return (double)GetValue(ValueProperty); }
            set { SetValue(ValueProperty, value); }
        }

        // Using a DependencyProperty as the backing store for MyProperty.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ValueProperty =
            DependencyProperty.Register("Value", typeof(double), typeof(Instrument),
                new PropertyMetadata(default(double), new PropertyChangedCallback(OnPropertyChanged)));

        private static void OnPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            Instrument instrument = d as Instrument;
            instrument.Refresh();
        }

        public Instrument()
        {
            InitializeComponent();
            this.SizeChanged += Ellipse_SizeChanged;
        }

        private void Ellipse_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            double miniSize = Math.Min(this.RenderSize.Width, this.RenderSize.Height);
            this.backEllipse.Width = miniSize;
            this.backEllipse.Height = miniSize;
        }

        private void Refresh()
        {
            double r = this.backEllipse.Width / 2;
            this.mainCanvas.Children.Clear();
            int max = this.Max, min = this.Min;
            int scaleText = min;
            int scaleAreaCount = 100;
            double step = 270.0 / scaleAreaCount;
            for (int i = 0; i <= scaleAreaCount; i++)
            {
                Line line = new Line();
                Line line1 = new Line();
                line.X1 = r + (r - 8) * Math.Cos((i * step - 45) * Math.PI / 180);
                line.Y1 = r - (r - 8) * Math.Sin((i * step - 45) * Math.PI / 180);
                line.X2 = r + (r - 14) * Math.Cos((i * step - 45) * Math.PI / 180);
                line.Y2 = r - (r - 14) * Math.Sin((i * step - 45) * Math.PI / 180); ;
                line.Stroke = Brushes.Black;
                line.StrokeThickness = 1;
                if (i % 10 == 0)
                {
                    line1.X1 = r + (r - 8) * Math.Cos((i * step - 45) * Math.PI / 180);
                    line1.Y1 = r - (r - 8) * Math.Sin((i * step - 45) * Math.PI / 180);
                    line1.X2 = r + (r - 20) * Math.Cos((i * step - 45) * Math.PI / 180);
                    line1.Y2 = r - (r - 20) * Math.Sin((i * step - 45) * Math.PI / 180); ;
                    line1.Stroke = Brushes.Black;
                    line1.StrokeThickness = 1;
                    TextBlock textBlock = new TextBlock();
                    textBlock.Text = (scaleText + (max - min) / scaleAreaCount * i).ToString();
                    textBlock.Foreground = Brushes.Black;
                    textBlock.Width = 30;
                    textBlock.TextAlignment = TextAlignment.Center;
                    textBlock.FontSize = 10;
                    Canvas.SetLeft(textBlock, r + (r - 30) * Math.Sin((i * step + 45) * Math.PI / 180) - 15);
                    Canvas.SetTop(textBlock, r + (r - 30) * Math.Cos((i * step + 45) * Math.PI / 180) - 5);
                    this.mainCanvas.Children.Add(textBlock);
                }
                this.mainCanvas.Children.Add(line);
                this.mainCanvas.Children.Add(line1);
            }
            string sData = "M{0} {1} A{0} {0} 0 1 1 {1} {2}";
            sData = string.Format(sData, r / 2, r, r * 1.5);
            var converter = TypeDescriptor.GetConverter(typeof(Geometry));
            this.circle.Data = (Geometry)converter.ConvertFromString(sData);

            sData = "M{0} {1} ,{1} {2},{1} {3}";
            sData = string.Format(sData, 0.5 * r, r, r - 4, r + 4);
            converter = TypeDescriptor.GetConverter(typeof(Geometry));
            this.pointer.Data = (Geometry)converter.ConvertFromString(sData);
            DoubleAnimation doubleAnimation = new DoubleAnimation(-this.Value * 270.0 / (max - min) + 225,new Duration(TimeSpan.FromMilliseconds(200)));
            this.rtPointer.BeginAnimation(RotateTransform.AngleProperty,doubleAnimation);
        }
    }
}
