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

Description: MetaFsProps control
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Core.Abstractions;
using Core.Concrete;
using System.Runtime.InteropServices;


namespace MetaFSPropPage {
	[ClassInterface(ClassInterfaceType.None)]
	[ComSourceInterfaces(typeof(IMetaFsProps))]
	[Guid("917937B1-1AD8-4951-A417-3BED065089A0")]
	public partial class MetaFsProps : UserControl, IMetaFsProps, IChangeMonitor {

		/// <summary>
		/// The idapply
		/// </summary>
		private const int IDAPPLY = 0x3021;

		/// <summary>
		/// The idok
		/// </summary>
		private const int IDOK = 0x1;

		/// <summary>
		/// The idcancel
		/// </summary>
		private const int IDCANCEL = 0x1;

		/// <summary>
		/// The w m_ im e_ setcontext
		/// </summary>
		private const int WM_IME_SETCONTEXT = 0x281;

		/// <summary>
		/// The w m_ application
		/// </summary>
		private const int WM_APP = 0x8000;


		/// <summary>
		/// Gets or sets the current metadata.
		/// </summary>
		/// <value>
		/// The current metadata.
		/// </value>
		protected IFileMetadata CurrentMetadata {
			get;
			set;
		}

		/// <summary>
		/// Gets the data.
		/// </summary>
		/// <value>
		/// The data.
		/// </value>
		protected IDataOperations Data {
			get;
			private set;
		}

		/// <summary>
		/// Gets the control HWND.
		/// </summary>
		/// <returns></returns>
		public IntPtr GetControlHwnd() {
			return Handle;
		}

		/// <summary>
		/// Gets or sets the selected file.
		/// </summary>
		/// <value>The selected file.</value>
		public string SelectedFile {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets a value indicating whether this instance has changes.
		/// </summary>
		/// <value><c>true</c> if this instance has changes; otherwise, <c>false</c>.</value>
		public bool HasChanges {
			get;
			set;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="MetaFsProps"/> class.
		/// </summary>
		public MetaFsProps() {
			InitializeComponent();
		}

		/// <summary>
		/// Updates the or retrieve metadata.
		/// </summary>
		/// <param name="fileName">Name of the file.</param>
		public void UpdateOrRetrieveMetadata(string fileName = null) {
			try {
				if (!string.IsNullOrEmpty(fileName)) {
					CurrentMetadata = Data.GetFileMetadata(Data.SelectedMac, Data.MachineName, fileName);
					Data.Renderer.DynamicallyRenderControls(CurrentMetadata);
				} else if (CurrentMetadata != null)
					Data.UpdateFileMetadata(CurrentMetadata);
			} catch (Exception ex) {
				MessageBox.Show("Unable to connect to metadata store", "Information",
								MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

				Data.Logger.LogError(ex);
			}
		}

		/// <summary>
		/// Handles the Load event of the MetaFSProps control.
		/// </summary>
		/// <param name="sender">The source of the event.</param>
		/// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
		private void MetaFSProps_Load(object sender, EventArgs e) {
			if (!DesignMode) {
				Data = DataOperations.GetInstance(this);
			}
		}

		/// <summary>
		/// WNDs the proc.
		/// </summary>
		/// <param name="m">The Windows <see cref="T:System.Windows.Forms.Message" /> to process.</param>
		protected override void WndProc(ref Message m) {
			switch (m.Msg) {
				case WM_IME_SETCONTEXT:
					if (Controls[0].Controls.Count == 0)
						UpdateOrRetrieveMetadata(SelectedFile);
					break;
				case WM_APP:
					if (m.WParam.ToInt32().Equals(IDAPPLY)) {
						if (HasChanges)
							UpdateOrRetrieveMetadata();

					} else if (m.WParam.ToInt32().Equals(IDOK)) {
						UpdateOrRetrieveMetadata();
					}
					break;
			}
			base.WndProc(ref m);
		}
	}
}
