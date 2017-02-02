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

Description: Network Operations
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;
using Core.Abstractions;

namespace Core.Concrete {
	public class NetworkOperations : INetworkOperations {
		private static volatile INetworkOperations instance;
		private static readonly object syncRoot = new object();

		/// <summary>
		/// Prevents a default instance of the <see cref="NetworkOperations"/> class from being created.
		/// </summary>
		private NetworkOperations() {

		}


		/// <summary>
		/// Gets the instance.
		/// </summary>
		/// <value>
		/// The instance.
		/// </value>
		public static INetworkOperations Instance {
			get {
				if (instance == null) {
					lock (syncRoot) {
						if (instance == null)
							instance = new NetworkOperations();
					}
				}
				return instance;
			}
		}




		public string GetMacAddress() {
			var index = 0;
			var retval = new StringBuilder();

			var firstNic = NetworkInterface.GetAllNetworkInterfaces().FirstOrDefault();

			if (firstNic == null)
				throw new NullReferenceException("NIC not found. One is required at least");

			var bytes = firstNic.GetPhysicalAddress().GetAddressBytes();

			bytes.ToList().ForEach(x => {
				retval.Append(bytes[index++].ToString("X2"));

				if (index != bytes.Length)
					retval.Append("-");

			});

			return retval.ToString();
		}
	}
}
