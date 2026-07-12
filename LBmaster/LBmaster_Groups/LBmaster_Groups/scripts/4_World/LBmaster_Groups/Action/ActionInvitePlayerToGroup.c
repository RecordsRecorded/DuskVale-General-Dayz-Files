class ActionInvitePlayerToGroup : ActionInteractBase {

	static int inviteEnabledTime = 0;
	string targetName = "Player";

	void ActionInvitePlayerToGroup() {
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override void CreateConditionComponents() {
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINone;
	}

	override string GetText() {
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (player && player.GetLBGroup()) {
			return LBTranslatedString("#lb_ag_action_invite_with", {"%player%", targetName, "%group%", player.GetLBGroup().name}).Get();
		}
		return LBTranslatedString("#lb_ag_action_invite_no", {"%player%", targetName}).Get();
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		int now = LBDate.Init(true).GetTimestamp();
		if (now - inviteEnabledTime > 60 && g_Game.IsClient())
			return false;
		LBGroup grp = player.GetLBGroup();
		if (!grp)
			return false;
		LBGroupPermission perms = player.GetPermission();
		if (!perms || !perms.canInvite)
			return false;
		PlayerBase targetPlayer = PlayerBase.Cast(target.GetObject());
		if (!targetPlayer)
			return false;
		PlayerIdentity identity = targetPlayer.GetIdentity();
		if (!identity)
			return false;
		if (LBGroupSettingsConfig.Get.inviteActionShowName)
			targetName = identity.GetName();
		else
			targetName = "Player";
		return true;
	}

	static void EnableInviteAction() {
		inviteEnabledTime = LBDate.Init(true).GetTimestamp();
	}
}
