using Core.Abstractions;

namespace Core.Concrete {
	public class UiElement : IUiElement {

		/// <summary>
		/// Gets or sets the class.
		/// </summary>
		/// <value>
		/// The class.
		/// </value>
		public string Class {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the rect.
		/// </summary>
		/// <value>
		/// The rect.
		/// </value>
		public System.Drawing.Rectangle Rect {
			get;
			set;
		}

	
		/// <summary>
		/// Gets or sets the name.
		/// </summary>
		/// <value>
		/// The name.
		/// </value>
		public string Name {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the text.
		/// </summary>
		/// <value>
		/// The text.
		/// </value>
		public string Text {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the maximum length.
		/// </summary>
		/// <value>
		/// The maximum length.
		/// </value>
		public int MaxLength {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets a value indicating whether [readonly].
		/// </summary>
		/// <value>
		///   <c>true</c> if [readonly]; otherwise, <c>false</c>.
		/// </value>
		public bool ReadOnly {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the tab order.
		/// </summary>
		/// <value>
		/// The tab order.
		/// </value>
		public int TabOrder {
			get;
			set;
		}

		/// <summary>
		/// Gets or sets the value.
		/// </summary>
		/// <value>
		/// The value.
		/// </value>
		public string Value {
			get;
			set;
		}
	}

}