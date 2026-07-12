class LBConsoleCommandToggleBase : LBConsoleCommandBase {

	override void Register() {
		parameter.Insert("(toggle/enable/disable)");
	}

	override bool IsValid() {
		if (parsedParameter.Count() == 0)
			return false;
		string mode = parsedParameter.Get(0);
		return mode == "toggle" || mode == "enable" || mode == "disable";
	}

	override void Execute() {
		string mode = parsedParameter.Get(0);
		bool enable = false;
		if (mode == "toggle")
			enable = !IsModifierEnabled();
		else if (mode == "enable")
			enable = true;
		SendChange(enable);
	}

	bool IsModifierEnabled() {
		return false;
	}

	void SendChange(bool enable) {
	}

}