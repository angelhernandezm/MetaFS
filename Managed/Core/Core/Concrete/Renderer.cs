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

Description: Renderer
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Core.Abstractions;
using DAL;

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
