// Copyright (C) 2015 Angel Hernandez Matos / Bonafide Ideas.
// You can redistribute this software and/or modify it under the terms of the 
// Microsoft Reciprocal License (Ms-RL).  This program is distributed in the hope 
// that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
// See License.txt for more details. 

/* C# compiler    : Microsoft (R) Visual C# Compiler version 12.0.31101.0 for C# 5
Creation date     : 09/06/2015
Developer         : Angel Hernandez Matos
e-m@il            : angel@bonafideideas.com
Website           : http://www.bonafideideas.com

Description: Extension methods
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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

