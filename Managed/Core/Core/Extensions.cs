using System;
using System.ComponentModel;
using System.IO;
using System.Xml.Linq;

namespace DAL {
	public static class Extensions {
		/// <summary>
		/// Loads as x document.
		/// </summary>
		/// <param name="xml">The XML.</param>
		/// <returns></returns>
		public static XDocument LoadAsXDocument(this string xml) {
			XDocument retval = null;

			if (!string.IsNullOrEmpty(xml)) {
				using (var sr = new StringReader(xml))
					retval = XDocument.Load(sr);
			}
			return retval;
		}


		/// <summary>
		/// Invokes the ex.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="control">The control.</param>
		/// <param name="action">The action.</param>
		public static void InvokeThroughLambda<T>(this T control, Action<T> action) where T : ISynchronizeInvoke {
			if (control.InvokeRequired)
				control.Invoke(action, new object[] { control });
			else
				action(control);
		}
	}
}

