using Core.Abstractions;
using System;

namespace Core.Concrete {
	public class GenericLogger : IGenericLogger {
		private static volatile IGenericLogger instance;
		private static readonly object syncRoot = new object();


		private GenericLogger() {

		}

		public static IGenericLogger Instance {
			get {
				if (instance == null) {
					lock (syncRoot) {
						if (instance == null)
							instance = new GenericLogger();
					}
				}
				return instance;
			}
		}

		public void LogError(Exception ex) {
			throw new NotImplementedException();
		}

		public void LogError(string message) {
			throw new NotImplementedException();
		}
	}
}
