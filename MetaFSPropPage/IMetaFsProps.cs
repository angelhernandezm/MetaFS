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

Description: IMetaFsProps Interface
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace MetaFSPropPage {
	/// <summary>
	/// 
	/// </summary>
	[Guid("EBD2C511-4EEB-488A-9BF1-EE14FB631C92")]
	public interface IMetaFsProps {
		/// <summary>
		/// Gets the control HWND.
		/// </summary>
		/// <returns>IntPtr.</returns>
		IntPtr GetControlHwnd();

		/// <summary>
		/// Gets or sets the selected file.
		/// </summary>
		/// <value>The selected file.</value>
		string SelectedFile {
			get;
			set;
		}
	}
}
