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

Description: UiElement model
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using Core.Abstractions;

namespace Core.Concrete {
	public class UiElement : IUiElement {

		/// <summary>
		/// Gets or sets the class.
		/// </summary>
		/// <value>
		/// The class.
		/// </value>
		public string Class {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the rect.
		/// </summary>
		/// <value>
		/// The rect.
		/// </value>
		public System.Drawing.Rectangle Rect {
			get;
			set;
		}

	
		/// <summary>
		/// Gets or sets the name.
		/// </summary>
		/// <value>
		/// The name.
		/// </value>
		public string Name {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the text.
		/// </summary>
		/// <value>
		/// The text.
		/// </value>
		public string Text {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the maximum length.
		/// </summary>
		/// <value>
		/// The maximum length.
		/// </value>
		public int MaxLength {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets a value indicating whether [readonly].
		/// </summary>
		/// <value>
		///   <c>true</c> if [readonly]; otherwise, <c>false</c>.
		/// </value>
		public bool ReadOnly {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the tab order.
		/// </summary>
		/// <value>
		/// The tab order.
		/// </value>
		public int TabOrder {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the value.
		/// </summary>
		/// <value>
		/// The value.
		/// </value>
		public string Value {
			get;
			set;
		}
	}

}