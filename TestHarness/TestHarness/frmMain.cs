using System;
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
