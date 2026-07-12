class LBConsoleCommandModifierBase : LBConsoleCommandToggleBase {

	LBPlayerModifier modifier;

	override bool HasPermission() {
		LBModifierInfo info = LBModifierManager.Get().cache.Get(modifier);
		if (info && info.permOwn != "")
			return LBAdmins.Get().HasPermission(info.permOwn);
		return false;
	}

	override bool IsModifierEnabled() {
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
			return false;
		return player.LBIsModifierActive(modifier);
	}

	override void SendChange(bool enable) {
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
			return;

		array<PlayerIdentity> nullList = null;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(nullList);
		rpc.Write(modifier);
		rpc.Write(enable);
		rpc.Send(null, LBAdminToolRPCs.SET_MODIFIER_REMOTE, true);
	}

}