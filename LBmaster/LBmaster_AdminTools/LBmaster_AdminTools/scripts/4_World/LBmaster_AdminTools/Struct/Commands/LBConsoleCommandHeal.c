class LBConsoleCommandHeal : LBConsoleCommandBase {

	[LBConsoleCommandHandler.Get().Register("LBConsoleCommandHeal")]
	override void Register() {
		prefix = "heal";
	}

	override bool HasPermission() {
		return LBAdmins.Get().HasPermission("admin.heal.self");
	}

	override void Execute() {
		ScriptRPC rpc = new ScriptRPC();
		array<PlayerIdentity> empty = new array<PlayerIdentity>();
		rpc.Write(empty);
		rpc.Send(null, LBAdminToolRPCs.HEAL_PLAYER, true);
	}

}