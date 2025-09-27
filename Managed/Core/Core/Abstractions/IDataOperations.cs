namespace Core.Abstractions {
	public interface IDataOperations {
		/// <summary>
		/// Gets the logger.
		/// </summary>
		/// <value>
		/// The logger.
		/// </value>
		IGenericLogger Logger {
			get;
		}

		/// <summary>
		/// Gets the network ops.
		/// </summary>
		/// <value>
		/// The network ops.
		/// </value>
		INetworkOperations NetworkOps {
			get;
		}

		/// <summary>
		/// Gets the renderer.
		/// </summary>
		/// <value>
		/// The renderer.
		/// </value>
		IRenderer Renderer {
			get;
		}

		/// <summary>
		/// Creates the record.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="t">The t.</param>
		/// <returns></returns>
		bool CreateRecord<T>(T t);

		/// <summary>
		/// Gets the selected mac.
		/// </summary>
		/// <value>
		/// The selected mac.
		/// </value>
		string SelectedMac {
			get;
		}

		/// <summary>
		/// Gets the name of the machine.
		/// </summary>
		/// <value>
		/// The name of the machine.
		/// </value>
		string MachineName {
			get;

		}

		/// <summary>
		/// Gets the pc identifier.
		/// </summary>
		/// <value>
		/// The pc identifier.
		/// </value>
		int PcIdentifier {
			get;
		}

		/// <summary>
		/// Gets the file metadata.
		/// </summary>
		/// <param name="macAddress">The mac address.</param>
		/// <param name="computerName">Name of the computer.</param>
		/// <param name="fileName">Name of the file.</param>
		/// <returns></returns>
		IFileMetadata GetFileMetadata(string macAddress, string computerName, string fileName);

		/// <summary>
		/// Updates the file metadata.
		/// </summary>
		/// <param name="currentMetadata">The current metadata.</param>
		void UpdateFileMetadata(IFileMetadata currentMetadata);
	}
}
