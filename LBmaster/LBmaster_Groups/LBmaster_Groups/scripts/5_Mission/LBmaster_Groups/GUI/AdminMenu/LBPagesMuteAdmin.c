#ifndef LB_DISABLE_CHAT
[DayZGame.RegisterLBAdminPage("LBPagesMuteAdmin", "LBmaster_Groups/logo/logo.paa", "Chat mute", "groups.mute.player")]
class LBPagesMuteAdmin : LBAdmin_Menu_Page {

	TextListboxWidget list_muted, list_online;
	Widget panel_mute;
	ButtonWidget btn_unmute_selected, btn_mute_steamid, btn_mute_selected;
	EditBoxWidget input_mute_duration, input_mute_steamid, input_search_online;

	ref array<ref LBOnlinePlayerEntry> onlinePlayerList = new array<ref LBOnlinePlayerEntry>();

	void LBPagesMuteAdmin() {
		LBConfigManager.Get().GetEventOnConfigReceived("LBMuteConfig").Insert(OnConfigUpdated);
	}

	override void RegisterAllLinkedVars() {
		linked.RegisterLinkedList("GetMutes().mutedPlayers", list_muted).SetColumnCount(3).SetListManager(new LBListManager<LBMutedPlayerEntry>());
		linked.RegisterLinkedList("onlinePlayerList", list_online).SetColumnCount(2).SetSearchBar(input_search_online).SetListManager(new LBListManagerSearch<LBOnlinePlayerEntry>());
	}

	override void OnShow() {
		FillOnlinePlayers();
		linked.LoadLinkedVars();
	}

	void OnConfigUpdated() {
		FillOnlinePlayers();
		linked.LoadLinkedVars();
	}

	LBMuteConfig_ GetMutes() {
		return LBMuteConfig.Get;
	}

	override array<string> GetEditedConfigs() {}

	override bool OnClick(Widget w, int x, int y, int button) {
		super.OnClick(w, x, y, button);
		if (w == btn_mute_selected) {
			MutePlayer();
			return true;
		} else if (w == btn_mute_steamid) {
			MutePlayer(input_mute_steamid.GetText());
			return true;
		} else if (w == btn_unmute_selected) {
			UnmuteSelectedPlayer();
			return true;
		}
		return false;
	}

	void FillOnlinePlayers() {
		if (!ClientData.m_PlayerList || !ClientData.m_PlayerList.m_PlayerList)
			return;
		array<ref SyncPlayer> list = ClientData.m_PlayerList.m_PlayerList;
		onlinePlayerList.Clear();
		foreach (SyncPlayer player : list) {
			onlinePlayerList.Insert(new LBOnlinePlayerEntry(player.m_PlayerName, player.m_UID));
		}
	}

	void MutePlayer(string steamid = "") {
		if (steamid == "") {
			int row = linked.SearchedIndexToListIndex(list_online, list_online.GetSelectedRow());
			if (row < 0 || row >= onlinePlayerList.Count())
				return;
			steamid = onlinePlayerList.Get(row).param2;
		} else {
			input_mute_steamid.SetText("");
		}
		int duration = input_mute_duration.GetText().ToInt();
		if (duration <= 0)
			return;
		g_Game.RPCSingleParam(null, LBGroupRPCs.ADMIN_MUTE_STEAMID, new Param2<string, int>(steamid, duration), true);
		input_mute_duration.SetText("");
	}

	void UnmuteSelectedPlayer() {
		int row = list_muted.GetSelectedRow();
		if (row < 0 || row >= GetMutes().mutedPlayers.Count())
			return;
		string steamid = GetMutes().mutedPlayers.Get(row).param1;
		g_Game.RPCSingleParam(null, LBGroupRPCs.ADMIN_UNMUTE_PLAYER, new Param1<string>(steamid), true);
	}

}

#endif