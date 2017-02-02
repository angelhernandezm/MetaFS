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

Description: Template definition model
*/


using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using System.Xml.Schema;
using Core.Abstractions;

namespace Core.Concrete {
	public class TemplateDefinition : ITemplateDefinition {
		/// <summary>
		/// Gets the file metadata.
		/// </summary>
		/// <value>
		/// The file metadata.
		/// </value>
		public IFileMetadata FileMetadata {
			get;
			private set;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="TemplateDefinition" /> class.
		/// </summary>
		/// <param name="metadata">The metadata.</param>
		/// <exception cref="System.NullReferenceException">Template definition was not found</exception>
		public TemplateDefinition(IFileMetadata metadata) {
			if (metadata == null)
				throw new NullReferenceException("File metadata was not found");

			if (metadata.TemplateBody == null)
				throw new NullReferenceException("Template definition was not found");

			FileMetadata = metadata;
			TemplateElements = new List<IUiElement>();
			ParseTemplateAndLoad();
		}

		/// <summary>
		/// Sets the property.
		/// </summary>
		/// <param name="prop">The property.</param>
		/// <param name="targetType">Type of the target.</param>
		/// <param name="value">The value.</param>
		/// <param name="convertType">Type of the convert.</param>
		private void SetProperty(PropertyInfo prop, object targetType, string value, Type convertType) {
			if (string.IsNullOrEmpty(value))
				throw new ArgumentException("Value cannot be null or empty");

			if (typeof(string) == convertType) {
				prop.SetValue(targetType, value);
			} else if (typeof(Rectangle) == convertType) {
				var split = value.Split(',');

				if (split.Length == 4) {
					var rect = new Rectangle(int.Parse(split[0]), int.Parse(split[1]), int.Parse(split[2]), int.Parse(split[3]));
					prop.SetValue(targetType, rect);
				} else
					throw new IndexOutOfRangeException("Missing values to create Rectangle object");
			} else if (typeof(int) == convertType) {
				prop.SetValue(targetType, int.Parse(value));
			} else if (typeof(bool) == convertType) {
				prop.SetValue(targetType, bool.Parse(value));
			}
		}

		/// <summary>
		/// Processes the attribute.
		/// </summary>
		/// <param name="element">The element.</param>
		/// <param name="attributeName">Name of the attribute.</param>
		/// <param name="node">The node.</param>
		private void ProcessAttribute(XElement element, string attributeName, IUiElement node) {
			XAttribute attr;
			var flag = BindingFlags.SetProperty | BindingFlags.GetProperty | BindingFlags.Instance | BindingFlags.Public;

			if ((attr = element.Attribute(attributeName)) != null) {
				var type = node.GetType();
				var prop = type.GetProperties(flag).FirstOrDefault(x => string.Equals(attributeName,
																	   x.Name, StringComparison.OrdinalIgnoreCase));

				if (prop != null) {
					SetProperty(prop, node, attr.Value, prop.PropertyType);

					// Set value (if metadata for selected file exists) 
					if (FileMetadata.Metadata != null) {
						var attributeVal = element.Attribute("name");
						var attributes = FileMetadata.Metadata.Elements("MetaFS-Data")
							.Elements("Data").Elements("UIElement").Attributes("name");

						if (attributes.Any() && attributeVal != null) {
							var fieldValue = attributes.FirstOrDefault(x => string.Equals(x.Value,
								attributeVal.Value, StringComparison.OrdinalIgnoreCase));
							node.Value = fieldValue != null ? fieldValue.Parent.Attribute("value").Value : string.Empty;
						}
					}
				}

			}
		}

		/// <summary>
		/// Creates the individual element.
		/// </summary>
		/// <param name="element">The element.</param>
		/// <returns></returns>
		private IUiElement CreateIndividualElement(XElement element) {
			var retval = new UiElement();
			var attributes = new List<string>() {"class", "rect", "name", "text", 
											    "taborder", "maxlength", "readonly", "value"};

			attributes.ForEach(x => ProcessAttribute(element, x, retval));

			return retval;
		}

		/// <summary>
		/// Parses the template and load.
		/// </summary>
		private void ParseTemplateAndLoad() {
			TemplateId = int.Parse(FileMetadata.TemplateBody.Descendants("MetaFS").Attributes("templateid").First().Value);

			FileMetadata.TemplateBody.Descendants("MetaFS").Descendants("UIElement")
				.ToList().ForEach(x => ((List<IUiElement>)TemplateElements).Add(CreateIndividualElement(x)));
		}

		/// <summary>
		/// Gets or sets the template identifier.
		/// </summary>
		/// <value>
		/// The template identifier.
		/// </value>
		public int TemplateId {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the template elements.
		/// </summary>
		/// <value>
		/// The template elements.
		/// </value>
		public IEnumerable<IUiElement> TemplateElements {
			get;
			set;
		}
	}
}