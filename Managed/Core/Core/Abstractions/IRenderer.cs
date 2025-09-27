using System.Collections.Generic;
using System.Windows.Forms;

namespace Core.Abstractions {
	public interface IRenderer {
		/// <summary>
		/// Gets or sets the view.
		/// </summary>
		/// <value>
		/// The view.
		/// </value>
		UserControl View { get; set;}

		/// <summary>
		/// Dynamicallies the render controls.
		/// </summary>
		/// <param name="metadata">The metadata.</param>
		void DynamicallyRenderControls(IFileMetadata metadata);

		/// <summary>
		/// Rehydrates from UI.
		/// </summary>
		/// <returns></returns>
		IEnumerable<IUiElement> RehydrateFromUi();

	}
}
