class LBLastCheatsConfig : LBConfigLoader<LBLastCheatsConfig_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "lastCheatsEnabled.json", LBConfigType.CONFIG, true); // (easy) do not edit
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}
// This config contains information about the last cheats enabled by the admin. They can be restored when needed
class LBLastCheatsConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 1;
	LBLastCheatsSaveLevel saveLevel = LBLastCheatsSaveLevel.NEVER; // If or when the settings should be restored
	string lastServer = ""; // IP:Port of the last connected server to know if the player relogged
	bool full_bright = false; // If full bright was enabled
	int enabledModifiers = 0; // Other enabled Player Modifiers

	void RestoreEnabled(LBLastCheatsSaveLevel level) {
		if (level != saveLevel)
			return;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(saveLevel);
		rpc.Write(full_bright);
		rpc.Write(enabledModifiers);
		rpc.Send(null, LBAdminToolRPCs.RESTORE_MODIFIERS, true);
	}

	void CheckRelogged() {
		string newIp = LBWorldTools.GetServerString();
		if (lastServer == newIp) {
			RestoreEnabled(LBLastCheatsSaveLevel.RELOG);
		} else {
			lastServer = newIp;
			LBLastCheatsConfig.Loader.Save();
		}
	}

}