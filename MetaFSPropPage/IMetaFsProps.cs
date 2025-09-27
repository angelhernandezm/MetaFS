using System;
using System.Runtime.InteropServices;

namespace MetaFSPropPage {
	/// <summary>
	/// 
	/// </summary>
	[Guid("EBD2C511-4EEB-488A-9BF1-EE14FB631C92")]
	public interface IMetaFsProps {
		/// <summary>
		/// Gets the control HWND.
		/// </summary>
		/// <returns>IntPtr.</returns>
		IntPtr GetControlHwnd();

		/// <summary>
		/// Gets or sets the selected file.
		/// </summary>
		/// <value>The selected file.</value>
		string SelectedFile {
			get;
			set;
		}
	}
}
