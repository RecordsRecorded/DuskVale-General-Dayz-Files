modded class LB_Elevator_Mission {

	override void OnGroupChanged() {
		LBLogger.Debug("On Group Changed ...", "AdvancedGroups");
		super.OnGroupChanged();
		LBGroupUI openedMapUI = LBGroupUI.Cast(LBMenuManager.Get().GetPersistentMenu("LBGroupUI"));

		openedMapUI.OnGroupChanged();
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		if (mission && mission.gpsHud) {
			mission.gpsHud.OnGroupChanged();
		}
		if (LBGroupSettingsConfig.Get.enablePlayerList) {
			LBPlayerList.Get().OnGroupChanged();
		}
#ifndef LB_DISABLE_CHAT
		if (mission)
			mission.UpdateChannel();
#endif
		LBLogger.Debug("Finished Group Change Event", "AdvancedGroups");
	}

	override void OnItemInInventoryChanged() {
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		if (mission) {
			LBLogger.Debug("GPS: " + mission.gpsHud + " compass: " + mission.compassHud, "AdvancedGroups");
			if (mission.gpsHud) {
				mission.gpsHud.UpdateGPSItemVisibility();
			}
			if (mission.compassHud) {
				mission.compassHud.UpdateCanEnableCompass();
			}
		}
	}

	override void OnInviteReceived() {
		LBGroupCreatePage page = GetGroupCreatePage();
		if (page)
			page.SetInviteButton(true, lastInvite);
	}

	void AcceptGroupInvite() {
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb)
			return;
		Param1<string> lastInviteParam = new Param1<string>(lastInvite);
		LBLogger.Debug("Accepted Invite for " + lastInvite, "AdvancedGroups");
		g_Game.RPCSingleParam(null, LBGroupRPCs.GROUP_ACCEPT_INVITE, lastInviteParam, true);
		LBGroupCreatePage page = GetGroupCreatePage();
		if (page)
			page.SetInviteButton(false, "");
	}

	LBGroupCreatePage GetGroupCreatePage() {
		LBGroupUI openedMapUI = LBGroupUI.Cast(LBMenuManager.Get().GetPersistentMenu("LBGroupUI"));
		LBGroupPage createPage = openedMapUI.GetPageByType(LBPageSettingsType.GROUP);
		if (!createPage)
			return null;
		LBGroupCreatePage cPage;
		Class.CastTo(cPage, createPage);
		return cPage;
	}
}