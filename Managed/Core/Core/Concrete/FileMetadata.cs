using Core.Abstractions;

namespace Core.Concrete {
	public class FileMetadata : IFileMetadata {
		/// <summary>
		/// Gets or sets the pc identifier.
		/// </summary>
		/// <value>
		/// The pc identifier.
		/// </value>
		public int PcId {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the metadata identifier.
		/// </summary>
		/// <value>
		/// The metadata identifier.
		/// </value>
		public int MetadataId {
			get;
			set;
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
		/// Gets or sets the metadata.
		/// </summary>
		/// <value>
		/// The metadata.
		/// </value>
		public System.Xml.Linq.XDocument Metadata {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the template body.
		/// </summary>
		/// <value>
		/// The template body.
		/// </value>
		public System.Xml.Linq.XDocument TemplateBody {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the name of the file.
		/// </summary>
		/// <value>
		/// The name of the file.
		/// </value>
		public string FileName {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the full path.
		/// </summary>
		/// <value>
		/// The full path.
		/// </value>
		public string FullPath {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the category.
		/// </summary>
		/// <value>
		/// The category.
		/// </value>
		public string Category {
			get;
			set;

		}

		/// <summary>
		/// Gets or sets the sub category.
		/// </summary>
		/// <value>
		/// The sub category.
		/// </value>
		public string SubCategory {
			get;
			set;
		}
	}
}
