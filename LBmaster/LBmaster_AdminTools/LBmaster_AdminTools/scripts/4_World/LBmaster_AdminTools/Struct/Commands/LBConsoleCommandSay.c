class LBConsoleCommandSay : LBConsoleCommandBase {

	[LBConsoleCommandHandler.Get().Register("LBConsoleCommandSay")]
	override void Register() {
		prefix = "say";
	}

	override bool HasPermission() {
		return LBAdmins.Get().HasPermission("admin.send.message_multi");
	}

	override bool IsValid() {
		return rawParameter.Length() > 5;
	}

	override void Execute() {
		ScriptRPC rpc = new ScriptRPC();
		array<PlayerIdentity> empty = new array<PlayerIdentity>();
		rpc.Write(empty);
		string message = rawParameter.Substring(5, rawParameter.Length() - 5).Trim();
		rpc.Write(message);
		rpc.Send(null, LBAdminToolRPCs.MESSAGE_PLAYER, true);
	}

}