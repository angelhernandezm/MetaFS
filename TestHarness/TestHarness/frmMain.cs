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

Description: Implementation of TestHarness main form
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestHarness {
	public partial class frmMain : Form {
		/// <summary>
		/// Initializes a new instance of the <see cref="frmMain"/> class.
		/// </summary>
		public frmMain() {
			InitializeComponent();
		}

		/// <summary>
		/// Handles the Click event of the btnSelectFile control.
		/// </summary>
		/// <param name="sender">The source of the event.</param>
		/// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
		private void btnSelectFile_Click(object sender, EventArgs e) {
			var openFileDlg = new OpenFileDialog();

			if (openFileDlg.ShowDialog().Equals(DialogResult.OK) && !string.IsNullOrEmpty(openFileDlg.FileName))
				metaFSProps1.UpdateOrRetrieveMetadata(openFileDlg.FileName);
		}

		/// <summary>
		/// Handles the Click event of the btnSaveMetadata control.
		/// </summary>
		/// <param name="sender">The source of the event.</param>
		/// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
		private void btnSaveMetadata_Click(object sender, EventArgs e) {
			metaFSProps1.UpdateOrRetrieveMetadata();
			

		}
	}
}
