﻿// Copyright (C) 2015 Angel Hernandez Matos / Bonafide Ideas.
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

Description: IControlFactory Interface
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Core.Abstractions {
	public interface IControlFactory {
		/// <summary>
		/// Creates the specified definition.
		/// </summary>
		/// <param name="definition">The definition.</param>
		/// <param name="changeMonitor">The change monitor.</param>
		/// <returns>IEnumerable{Control}.</returns>
		IEnumerable<Control> Create(ITemplateDefinition definition, IChangeMonitor changeMonitor);
	}
}
