namespace Core.Abstractions {
	public interface IChangeMonitor {
		bool HasChanges { get; set; }
	}
}
