using System.Collections.Generic;
using System.Windows.Forms;

namespace Core.Abstractions {
	public interface IControlFactory {
		/// <summary>
		/// Creates the specified definition.
		/// </summary>
		/// <param name="definition">The definition.</param>
		/// <param name="changeMonitor">The change monitor.</param>
		/// <returns>IEnumerable{Control}.</returns>
		IEnumerable<Control> Create(ITemplateDefinition definition, IChangeMonitor changeMonitor);
	}
}
