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

Description: IUIElement Interface
*/

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Core.Abstractions {
	public interface IUiElement {
		/// <summary>
		/// Gets or sets the class.
		/// </summary>
		/// <value>
		/// The class.
		/// </value>
		string Class {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the rect.
		/// </summary>
		/// <value>
		/// The rect.
		/// </value>
		Rectangle Rect {
			get;
			set;
		}


		/// <summary>
		/// Gets or sets the name.
		/// </summary>
		/// <value>
		/// The name.
		/// </value>
		string Name {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the text.
		/// </summary>
		/// <value>
		/// The text.
		/// </value>
		string Text {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the maximum length.
		/// </summary>
		/// <value>
		/// The maximum length.
		/// </value>
		int MaxLength {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets a value indicating whether [readonly].
		/// </summary>
		/// <value>
		///   <c>true</c> if [readonly]; otherwise, <c>false</c>.
		/// </value>
		bool ReadOnly {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the tab order.
		/// </summary>
		/// <value>
		/// The tab order.
		/// </value>
		int TabOrder {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the value.
		/// </summary>
		/// <value>
		/// The value.
		/// </value>
		string Value {
			get;
			set;
		}
	}
}
