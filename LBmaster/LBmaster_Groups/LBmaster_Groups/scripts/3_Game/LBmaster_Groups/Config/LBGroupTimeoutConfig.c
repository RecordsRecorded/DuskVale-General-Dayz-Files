class LBGroupTimeoutConfig : LBConfigLoader<LBGroupTimeoutConfig_> {

	[SetPriority(LBConfigPriority.HIGH)]
	override void InitVars() {
		InitVarsInternal("LBGroup", "Timeout.json", LBConfigType.CONFIG, true, "groups.change.timeout"); // (easy)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

}
/*Config that handles the group swap timeouts so player do not swap groups to gain advantages (disabled by default). Current timeouts are saved in the TimeoutPlayers.json
*/
class LBGroupTimeoutConfig_ : LBConfigBase {

	static const int CURRENT_VERSION = 3;

	bool enableTimeout = false; // Set to 1 to enable the timeouts
	bool triggerOnLeave = false; // Enable to trigger the timeout when the player left his group or was kicked. If disabled the timeout is triggered when joining a group
	int timeoutDifferentGroupDurationSeconds = 86400; // Time in seconds a player needs to wait until he can join a different group than his last group (The timer will start when the player joined the first group and not when he left)
	int timeoutSameGroupDurationSeconds = 3600; // Time in seconds a player needs to wait until he can join the same group he was in before (The timer starts when he joins the first group)

}