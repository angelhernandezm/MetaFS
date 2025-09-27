using System.Xml.Linq;

namespace Core.Abstractions {
	public interface IFileMetadata {
		/// <summary>
		/// Gets or sets the pc identifier.
		/// </summary>
		/// <value>
		/// The pc identifier.
		/// </value>
		int PcId {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the metadata identifier.
		/// </summary>
		/// <value>
		/// The metadata identifier.
		/// </value>
		int MetadataId {
			get;
			set;
		}

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
		/// Gets or sets the metadata.
		/// </summary>
		/// <value>
		/// The metadata.
		/// </value>
		XDocument Metadata {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the template body.
		/// </summary>
		/// <value>
		/// The template body.
		/// </value>
		XDocument TemplateBody {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the name of the file.
		/// </summary>
		/// <value>
		/// The name of the file.
		/// </value>
		string FileName {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the full path.
		/// </summary>
		/// <value>
		/// The full path.
		/// </value>
		string FullPath {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the category.
		/// </summary>
		/// <value>
		/// The category.
		/// </value>
		string Category {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the sub category.
		/// </summary>
		/// <value>
		/// The sub category.
		/// </value>
		string SubCategory {
			get;
			set;
		}
	}
}
