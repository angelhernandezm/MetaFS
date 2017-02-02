namespace MetaFSPropPage
{
    partial class MetaFsProps
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.mainContainer = new System.Windows.Forms.Panel();
			this.SuspendLayout();
			// 
			// mainContainer
			// 
			this.mainContainer.AutoScroll = true;
			this.mainContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainContainer.Location = new System.Drawing.Point(0, 0);
			this.mainContainer.Name = "mainContainer";
			this.mainContainer.Size = new System.Drawing.Size(340, 425);
			this.mainContainer.TabIndex = 0;
			// 
			// MetaFsProps
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.mainContainer);
			this.Name = "MetaFsProps";
			this.Size = new System.Drawing.Size(340, 425);
			this.Load += new System.EventHandler(this.MetaFSProps_Load);
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.Panel mainContainer;
    }
}
