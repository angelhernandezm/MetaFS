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

Description: ITemplateDefinition Interface
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Core.Abstractions {
	public interface ITemplateDefinition {
		/// <summary>
		/// Gets or sets the template identifier.
		/// </summary>
		/// <value>
		/// The template identifier.
		/// </value>
		int TemplateId {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the template elements.
		/// </summary>
		/// <value>
		/// The template elements.
		/// </value>
		IEnumerable<IUiElement> TemplateElements {
			get;
			set;
		}


		/// <summary>
		/// Gets the file metadata.
		/// </summary>
		/// <value>
		/// The file metadata.
		/// </value>
		IFileMetadata FileMetadata {
			get;
		}
	}
}
