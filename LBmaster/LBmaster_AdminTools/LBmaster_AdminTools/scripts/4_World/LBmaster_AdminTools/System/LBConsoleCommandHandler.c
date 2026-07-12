class LBConsoleCommandHandler {

	static ref LBConsoleCommandHandler g_LBConsoleCommandHandler;

	static LBConsoleCommandHandler Get() {
		if (!g_LBConsoleCommandHandler)
			g_LBConsoleCommandHandler = new LBConsoleCommandHandler();
		return g_LBConsoleCommandHandler;
	}

	ref TStringSet registeredCommands = new TStringSet();

	LBConsoleCommandHandler Register(string commandClassname) {
		registeredCommands.Insert(commandClassname);
		return null;
	}

	array<ref LBConsoleCommandBase> GetAvailableCommands() {
		array<ref LBConsoleCommandBase> handlers = new array<ref LBConsoleCommandBase>();
		foreach (string classname : registeredCommands) {
			typename tn = classname.ToType();
			if (!tn)
				continue;
			LBConsoleCommandBase handler;
			Class.CastTo(handler, tn.Spawn());
			if (handler) {
				handler.Register();
				handlers.Insert(handler);
			}
		}
		bool sorted = false;
		while (!sorted) {
			sorted = true;
			for (int i = 0; i < handlers.Count() - 1; ++i) {
				LBConsoleCommandBase prev = handlers.Get(i);
				LBConsoleCommandBase next = handlers.Get(i + 1);
				if (prev.prefix > next.prefix) {
					handlers.Set(i, next);
					handlers.Set(i + 1, prev);
					sorted = false;
				}
			}
		}
		return handlers;
	}

}