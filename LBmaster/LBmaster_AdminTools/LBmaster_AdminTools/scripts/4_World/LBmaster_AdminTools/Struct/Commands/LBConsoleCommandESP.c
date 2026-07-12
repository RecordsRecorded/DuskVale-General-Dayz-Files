class LBConsoleCommandESP : LBConsoleCommandToggleBase {

	[LBConsoleCommandHandler.Get().Register("LBConsoleCommandESP")]
	override void Register() {
		prefix = "esp";
	}

	override bool HasPermission() {
		return LBAdmins.Get().HasPermission("admin.cheat.esp");
	}

	override bool IsModifierEnabled() {
		return LBESPManager.enabled;
	}

	override void SendChange(bool enable) {
		if (enable)
			LBESPManager.EnableESP();
		else
			LBESPManager.DisableESP();
	}

}