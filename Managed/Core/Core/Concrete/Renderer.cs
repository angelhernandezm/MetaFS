using Core.Abstractions;
using DAL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Core.Concrete {
	public class Renderer : IRenderer {
		/// <summary>
		/// Gets or sets the view.
		/// </summary>
		/// <value>
		/// The view.
		/// </value>
		public UserControl View {
			get;
			set;
		}

		/// <summary>
		/// Dynamicallies the render controls.
		/// </summary>
		/// <param name="metadata">The metadata.</param>
		public void DynamicallyRenderControls(IFileMetadata metadata) {
			if (View == null)
				throw new NullReferenceException("View has not been set");

			if (metadata == null)
				throw new NullReferenceException("Metadata was not found");

			RenderHelper(metadata);
		}

		/// <summary>
		/// Rehydrates from UI.
		/// </summary>
		/// <returns></returns>
		public IEnumerable<IUiElement> RehydrateFromUi() {
			var retval = new List<IUiElement>();
			var container = View.Controls.Find("mainContainer", true).FirstOrDefault();

			if (container != null) {
				container.Controls.Cast<Control>().Where(x => !(x is Label)).ToList()
					.ForEach(z => retval.Add(new UiElement() {
						Name = z.Name, Value = z.Text
					}));
			}

			return retval;
		}

		/// <summary>
		/// Renders the helper.
		/// </summary>
		/// <param name="metadata">The metadata.</param>
		private void RenderHelper(IFileMetadata metadata) {
			var template = new TemplateDefinition(metadata);
			var controls = (new ControlFactory()).Create(template, View as IChangeMonitor);
			var container = View.Controls.Find("mainContainer", true).FirstOrDefault();

			if (container != null) {
				container.InvokeThroughLambda(x => x.Controls.Clear());
				container.InvokeThroughLambda(x => x.Controls.AddRange(controls.ToArray()));
			}
		}
	}
}
