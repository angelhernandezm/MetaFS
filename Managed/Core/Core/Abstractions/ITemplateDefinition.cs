using System.Collections.Generic;

namespace Core.Abstractions {
	public interface ITemplateDefinition {
		/// <summary>
		/// Gets or sets the template identifier.
		/// </summary>
		/// <value>
		/// The template identifier.
		/// </value>
		int TemplateId {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the template elements.
		/// </summary>
		/// <value>
		/// The template elements.
		/// </value>
		IEnumerable<IUiElement> TemplateElements {
			get;
			set;
		}


		/// <summary>
		/// Gets the file metadata.
		/// </summary>
		/// <value>
		/// The file metadata.
		/// </value>
		IFileMetadata FileMetadata {
			get;
		}
	}
}
