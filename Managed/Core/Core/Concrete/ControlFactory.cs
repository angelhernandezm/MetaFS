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

Description: Control Factory
*/

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Core.Abstractions;

namespace Core.Concrete {
	/// <summary>
	/// Class ControlFactory.
	/// </summary>
	public class ControlFactory : IControlFactory {

		/// <summary>
		/// Gets or sets the logger.
		/// </summary>
		/// <value>
		/// The logger.
		/// </value>
		private IGenericLogger Logger {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the change monitor.
		/// </summary>
		/// <value>The change monitor.</value>
		private IChangeMonitor ChangeMonitor {
			get;
			set;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="ControlFactory"/> class.
		/// </summary>
		public ControlFactory() {
			Logger = GenericLogger.Instance;
		}

		/// <summary>
		/// Creates the specified definition.
		/// </summary>
		/// <param name="definition">The definition.</param>
		/// <param name="changeMonitor">The change monitor.</param>
		/// <returns>IEnumerable{Control}.</returns>
		/// <exception cref="System.ArgumentNullException">Template definition was expected.</exception>
		public IEnumerable<Control> Create(ITemplateDefinition definition, IChangeMonitor changeMonitor) {
			IEnumerable<Control> retval = null;

			if (definition == null || !definition.TemplateElements.Any())
				throw new ArgumentNullException("Template definition was expected.");

			if (changeMonitor == null)
				throw new ArgumentNullException("ChangeMonitor definition was expected.");

			ChangeMonitor = changeMonitor;
			retval = CreateHelper(definition);

			return retval;
		}

		/// <summary>
		/// Creates the helper.
		/// </summary>
		/// <param name="definition">The definition.</param>
		/// <returns></returns>
		private IEnumerable<Control> CreateHelper(ITemplateDefinition definition) {
			var retval = new List<Control>();

			var flag = BindingFlags.CreateInstance | BindingFlags.Instance | BindingFlags.Public;

			var targetAsm = AppDomain.CurrentDomain.GetAssemblies()
				.FirstOrDefault(x => string.Equals(x.GetName().Name, "System.Windows.Forms", StringComparison.OrdinalIgnoreCase));

			if (targetAsm != null) {
				definition.TemplateElements.ToList().ForEach(x => {

					try {
						var controlType = targetAsm.GetTypes().FirstOrDefault(z => z.IsSubclassOf(typeof(Control)) && !z.IsAbstract &&
																				   z.Name.ToUpperInvariant().Contains(x.Class.ToUpperInvariant()));

						// Expected to have a default constructor without any parameters
						var control = controlType.GetConstructors(flag).FirstOrDefault().Invoke(null) as Control;

						PropertySetter(control, x);

						// Does it have a TextChanged event?
						var ei = control.GetType().GetEvent("TextChanged");

						if (ei != null)
							ei.AddEventHandler(control, new EventHandler((sender, args) => ChangeMonitor.HasChanges = true));

						retval.Add(control);

					} catch (Exception ex) {
						Logger.LogError(ex);
					}

				});
			}

			return retval;
		}

		/// <summary>
		/// Properties the setter.
		/// </summary>
		/// <param name="targetControl">The target control.</param>
		/// <param name="props">The props.</param>
		private void PropertySetter(Control targetControl, IUiElement props) {
			targetControl.Name = props.Name;
			targetControl.Enabled = !props.ReadOnly;
			targetControl.TabIndex = props.TabOrder;
			targetControl.Location = new Point(props.Rect.X, props.Rect.Y);

			if ((targetControl as Label) != null) {
				targetControl.Text = props.Text;

				if (props.Rect.Width == -1)
					targetControl.AutoSize = true;
				else
					targetControl.Width = props.Rect.Width;

			} else if ((targetControl as TextBox) != null) {
				targetControl.Text = props.Value;
				targetControl.Width = props.Rect.Width;
				((TextBox)targetControl).MaxLength = props.MaxLength;
			}
		}
	}
}
