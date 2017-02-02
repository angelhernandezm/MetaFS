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

Description: IRenderer Interface
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Core.Abstractions {
	public interface IRenderer {
		/// <summary>
		/// Gets or sets the view.
		/// </summary>
		/// <value>
		/// The view.
		/// </value>
		UserControl View { get; set;}

		/// <summary>
		/// Dynamicallies the render controls.
		/// </summary>
		/// <param name="metadata">The metadata.</param>
		void DynamicallyRenderControls(IFileMetadata metadata);

		/// <summary>
		/// Rehydrates from UI.
		/// </summary>
		/// <returns></returns>
		IEnumerable<IUiElement> RehydrateFromUi();

	}
}
