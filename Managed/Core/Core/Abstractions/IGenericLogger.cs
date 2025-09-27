using System;

namespace Core.Abstractions {
	public interface IGenericLogger {
		void LogError(Exception ex);
		void LogError(string message);
	}
}
