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

Description: FileMetadata Model
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Core.Abstractions;

namespace Core.Concrete {
	public class FileMetadata : IFileMetadata {
		/// <summary>
		/// Gets or sets the pc identifier.
		/// </summary>
		/// <value>
		/// The pc identifier.
		/// </value>
		public int PcId {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the metadata identifier.
		/// </summary>
		/// <value>
		/// The metadata identifier.
		/// </value>
		public int MetadataId {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the template identifier.
		/// </summary>
		/// <value>
		/// The template identifier.
		/// </value>
		public int TemplateId {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the metadata.
		/// </summary>
		/// <value>
		/// The metadata.
		/// </value>
		public System.Xml.Linq.XDocument Metadata {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the template body.
		/// </summary>
		/// <value>
		/// The template body.
		/// </value>
		public System.Xml.Linq.XDocument TemplateBody {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the name of the file.
		/// </summary>
		/// <value>
		/// The name of the file.
		/// </value>
		public string FileName {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the full path.
		/// </summary>
		/// <value>
		/// The full path.
		/// </value>
		public string FullPath {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the category.
		/// </summary>
		/// <value>
		/// The category.
		/// </value>
		public string Category {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the sub category.
		/// </summary>
		/// <value>
		/// The sub category.
		/// </value>
		public string SubCategory {
			get;
			set;
		}
	}
}
