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

Description: Implementation of GenericLogger
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Core.Abstractions;

namespace Core.Concrete {
	public class GenericLogger : IGenericLogger {
		private static volatile IGenericLogger instance;
		private static readonly object syncRoot = new object();


		private GenericLogger() {

		}

		public static IGenericLogger Instance {
			get {
				if (instance == null) {
					lock (syncRoot) {
						if (instance == null)
							instance = new GenericLogger();
					}
				}
				return instance;
			}
		}

		public void LogError(Exception ex) {
			throw new NotImplementedException();
		}

		public void LogError(string message) {
			throw new NotImplementedException();
		}
	}
}
