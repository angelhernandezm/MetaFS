namespace TestHarness {
	partial class frmMain {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.btnSelectFile = new System.Windows.Forms.Button();
			this.btnSaveMetadata = new System.Windows.Forms.Button();
			this.metaFSProps1 = new MetaFSPropPage.MetaFsProps();
			this.SuspendLayout();
			// 
			// btnSelectFile
			// 
			this.btnSelectFile.Location = new System.Drawing.Point(115, 456);
			this.btnSelectFile.Name = "btnSelectFile";
			this.btnSelectFile.Size = new System.Drawing.Size(112, 23);
			this.btnSelectFile.TabIndex = 1;
			this.btnSelectFile.Text = "&Select File...";
			this.btnSelectFile.UseVisualStyleBackColor = true;
			this.btnSelectFile.Click += new System.EventHandler(this.btnSelectFile_Click);
			// 
			// btnSaveMetadata
			// 
			this.btnSaveMetadata.Location = new System.Drawing.Point(240, 456);
			this.btnSaveMetadata.Name = "btnSaveMetadata";
			this.btnSaveMetadata.Size = new System.Drawing.Size(112, 23);
			this.btnSaveMetadata.TabIndex = 2;
			this.btnSaveMetadata.Text = "&Save Metadata";
			this.btnSaveMetadata.UseVisualStyleBackColor = true;
			this.btnSaveMetadata.Click += new System.EventHandler(this.btnSaveMetadata_Click);
			// 
			// metaFSProps1
			// 
			this.metaFSProps1.Location = new System.Drawing.Point(12, 12);
			this.metaFSProps1.Name = "metaFSProps1";
			this.metaFSProps1.Size = new System.Drawing.Size(340, 425);
			this.metaFSProps1.TabIndex = 0;
			// 
			// frmMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(369, 493);
			this.Controls.Add(this.btnSaveMetadata);
			this.Controls.Add(this.btnSelectFile);
			this.Controls.Add(this.metaFSProps1);
			this.Name = "frmMain";
			this.Text = "Test Harness";
			this.ResumeLayout(false);

		}

		#endregion

		private MetaFSPropPage.MetaFsProps metaFSProps1;
		private System.Windows.Forms.Button btnSelectFile;
		private System.Windows.Forms.Button btnSaveMetadata;
	}
}

