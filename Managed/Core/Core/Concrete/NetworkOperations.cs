using Core.Abstractions;
using System;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;

namespace Core.Concrete {
	public class NetworkOperations : INetworkOperations {
		private static volatile INetworkOperations instance;
		private static readonly object syncRoot = new object();

		/// <summary>
		/// Prevents a default instance of the <see cref="NetworkOperations"/> class from being created.
		/// </summary>
		private NetworkOperations() {

		}


		/// <summary>
		/// Gets the instance.
		/// </summary>
		/// <value>
		/// The instance.
		/// </value>
		public static INetworkOperations Instance {
			get {
				if (instance == null) {
					lock (syncRoot) {
						if (instance == null)
							instance = new NetworkOperations();
					}
				}
				return instance;
			}
		}

        /// <summary>
        /// Gets the mac address.
        /// </summary>
        /// <returns>System.String.</returns>
        /// <exception cref="System.NullReferenceException">NIC not found. One is required at least</exception>
        public string GetMacAddress() {
			var index = 0;
			var retval = new StringBuilder();

			var firstNic = NetworkInterface.GetAllNetworkInterfaces().FirstOrDefault();

			if (firstNic == null)
				throw new NullReferenceException("NIC not found. One is required at least");

			var bytes = firstNic.GetPhysicalAddress().GetAddressBytes();

			bytes.ToList().ForEach(x => {
				retval.Append(bytes[index++].ToString("X2"));

				if (index != bytes.Length)
					retval.Append("-");

			});

			return retval.ToString();
		}
	}
}
