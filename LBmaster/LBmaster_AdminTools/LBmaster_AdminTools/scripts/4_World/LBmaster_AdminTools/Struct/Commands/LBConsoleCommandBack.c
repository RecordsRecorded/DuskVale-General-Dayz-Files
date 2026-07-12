class LBConsoleCommandBack : LBConsoleCommandBase {

	[LBConsoleCommandHandler.Get().Register("LBConsoleCommandBack")]
	override void Register() {
		prefix = "back";
	}

	override bool HasPermission() {
		return LBAdmins.Get().HasPermission("admin.teleport.back");
	}

	override void Execute() {
		ScriptRPC rpc = new ScriptRPC();
		array<PlayerIdentity> empty = new array<PlayerIdentity>();
		rpc.Write(empty);
		rpc.Send(null, LBAdminToolRPCs.BRING_BACK, true);
	}

}