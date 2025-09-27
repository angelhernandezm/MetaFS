using System.Drawing;

namespace Core.Abstractions {
	public interface IUiElement {
		/// <summary>
		/// Gets or sets the class.
		/// </summary>
		/// <value>
		/// The class.
		/// </value>
		string Class {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the rect.
		/// </summary>
		/// <value>
		/// The rect.
		/// </value>
		Rectangle Rect {
			get;
			set;
		}


		/// <summary>
		/// Gets or sets the name.
		/// </summary>
		/// <value>
		/// The name.
		/// </value>
		string Name {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the text.
		/// </summary>
		/// <value>
		/// The text.
		/// </value>
		string Text {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the maximum length.
		/// </summary>
		/// <value>
		/// The maximum length.
		/// </value>
		int MaxLength {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets a value indicating whether [readonly].
		/// </summary>
		/// <value>
		///   <c>true</c> if [readonly]; otherwise, <c>false</c>.
		/// </value>
		bool ReadOnly {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the tab order.
		/// </summary>
		/// <value>
		/// The tab order.
		/// </value>
		int TabOrder {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the value.
		/// </summary>
		/// <value>
		/// The value.
		/// </value>
		string Value {
			get;
			set;
		}
	}
}
