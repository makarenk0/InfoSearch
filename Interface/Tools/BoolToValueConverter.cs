using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Markup;

namespace Interface.Tools
{
    public abstract class BoolToValueConverter<T> : MarkupExtension, IValueConverter
    {
        #region Constructors and Destructors

        protected BoolToValueConverter()
        {
            this.TrueValue = default(T);
            this.FalseValue = default(T);
        }

        #endregion

        #region Public Properties

        public T FalseValue { get; set; }

        public T TrueValue { get; set; }

        #endregion

        #region Public Methods and Operators

        public object Convert(object value, Type targetType,
                              object parameter, CultureInfo culture)
        {
            return System.Convert.ToBoolean(value) ? this.TrueValue : this.FalseValue;
        }

        // Override if necessary
        public virtual object ConvertBack(object value, Type targetType,
                                          object parameter, CultureInfo culture)
        {
            return value.Equals(this.TrueValue);
        }

        public override object ProvideValue(IServiceProvider serviceProvider)
        {
            return this;
        }

        #endregion
    }
}
