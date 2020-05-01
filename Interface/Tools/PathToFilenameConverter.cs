using System;
using System.Windows.Data;
using System.Windows.Markup;
using System.Globalization;

namespace Interface.Tools
{
    public class PathToFilenameConverter : IValueConverter
    {

        #region Public Methods and Operators

        public object Convert(object value, Type targetType,
                              object parameter, CultureInfo culture)
        {
            return value.ToString().Substring(value.ToString().LastIndexOf('\u005c') + 1);
        }

        // Override if necessary
        public virtual object ConvertBack(object value, Type targetType,
                                          object parameter, CultureInfo culture)
        {
            return value;
        }

        #endregion
    }
}
