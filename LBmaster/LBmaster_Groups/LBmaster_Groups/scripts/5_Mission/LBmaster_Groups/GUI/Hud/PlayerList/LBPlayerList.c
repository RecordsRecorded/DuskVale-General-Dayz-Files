class LBPlayerList {

	Widget mainWidget;
	Widget listWidget;
	int lastMemberHash = 0;

	float lastUpdateEntries = 0.0, lastUpdateDistances = 0.0;

	ref array<ref LBPlayerListEntry> entries = new array<ref LBPlayerListEntry>();

	static ref LBPlayerList g_LBPlayerList;

	static void Delete() {
		g_LBPlayerList = null;
	}

	static LBPlayerList Get() {
		if (!g_LBPlayerList) {
			g_LBPlayerList = new LBPlayerList();
			g_LBPlayerList.InitWidgets();
		}
		return g_LBPlayerList;
	}

	void LBPlayerList() {
		LBColorManager_.Event_OnColorChange.Insert(OnColorChange);
		LBPositionManager_.Event_OnPositionChange.Insert(OnPositionChange);
		LBLayoutConfig_.Event_OnLayoutChanged.Insert(OnLayoutChange);
	}

	void ~LBPlayerList() {
		if (LBColorManager_.Event_OnColorChange)
			LBColorManager_.Event_OnColorChange.Remove(OnColorChange);
		if (LBPositionManager_.Event_OnPositionChange)
			LBPositionManager_.Event_OnPositionChange.Remove(OnPositionChange);
		if (LBLayoutConfig_.Event_OnLayoutChanged)
			LBLayoutConfig_.Event_OnLayoutChanged.Remove(OnLayoutChange);
		if (mainWidget)
			mainWidget.Unlink();
	}

	void OnLayoutChange() {
		UpdateEntries(true);
	}

	void OnColorChange() {
		UpdateEntries(true);
	}

	void OnPositionChange() {
		UpdatePosition();
	}

	void UpdatePosition() {
		LBLogger.Debug("Updating Position of Playerlist", "AdvancedGroups");
		vector screenLocation = LBPositionManager.Get.GetPosition("PlayerList");
		int index = LBPositionManager.Get.GetIndex("PlayerList");
		LBWidgetUtils.SetWidgetAlignmentIndex(listWidget, index);
		LBWidgetUtils.SetWidgetPositionIndex(listWidget, screenLocation, index);
		LBLogger.Debug("Updated Position to: " + screenLocation, "AdvancedGroups");
	}

	void UpdateVisibility(float timeslice) {
		lastUpdateEntries += timeslice;
		if (lastUpdateEntries >= 1.0) {
			UpdateEntries();
			lastUpdateEntries = 0.0;
		}
		lastUpdateDistances += timeslice;
		if (lastUpdateDistances >= 0.2) {
			UpdateDistances();
			lastUpdateDistances = 0.0;
		}
		if (!g_Game || !g_Game.GetMission() || !mainWidget)
			return;
		IngameHud hud = IngameHud.Cast(g_Game.GetMission().GetHud());
		if (!hud || g_Game.GetUIManager().GetMenu()) {
			mainWidget.Show(false);
			return;
		}
		mainWidget.Show(hud.LBIsHudVisible() && LBMarkerVisibilityManager.Get.playerlistEnabled);
	}

	void InitWidgets() {
		if (mainWidget || g_Game.IsServer())
			return;
		mainWidget = LBLayoutManager.Get().CreateLayout("PlayerListParent");
		if (mainWidget) {
			listWidget = mainWidget.FindAnyWidget("playerlist");
			LBLogger.Debug("Created Parent Widget for Player List: " + listWidget, "AdvancedGroups");
		}
		LBLogger.Debug("Initialized PlayerList Widget", "AdvancedGroups");
		OnGroupChanged();
	}

	void OnGroupChanged() {
		OnColorChange();
		OnPositionChange();
		OnLayoutChange();
	}

	void CreateEntries() {
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		LBGroup grp = pb.GetLBGroup();
		int mySubGroup = pb.GetMyGroupMarker().currentSubgroup;
		array<ref LBGroupMember> members = grp.GetSubgroupMembers(mySubGroup);
		foreach (LBGroupMember member : members) {
			LBPlayerListEntry entry = new LBPlayerListEntry();
			entry.Init(this, member);
			entries.Insert(entry);
			entry.UpdateWidget();
		}
		UpdateDistances();
	}

	int ShouldUpdateList() {
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return 0;
		LBGroup grp = pb.GetLBGroup();
		int mySubGroup = pb.GetMyGroupMarker().currentSubgroup;
		array<ref LBGroupMember> members = grp.GetSubgroupMembers(mySubGroup);
		LBLogger.Spam("Group Members: " + members.Count() + " Entries: " + entries.Count(), "AdvancedGroups");
		if (members.Count() != entries.Count())
			return 1;
		int memberHash = 0;
		foreach (LBGroupMember member : grp.members) {
			memberHash += member.CalcHash(false);
		}
		if (memberHash != lastMemberHash) {
			lastMemberHash = memberHash;
			return 1;
		}
		bool changedHealth = false;
		foreach (LBPlayerListEntry entry : entries) {
			if (!entry || !entry.member || members.Find(entry.member) == -1)
				return 1;
			if (entry.lastHealth != entry.member.health)
				changedHealth = true;
		}
		if (changedHealth)
			return 2;
		return 0;
	}

	void ClearAndDeleteEntries() {
		entries.Clear();
	}

	void UpdateEntries(bool force = false) {
		LBLogger.Spam("UpdateEntries. Force ? " + force + " Count: " + entries.Count(), "AdvancedGroups");
		if (!mainWidget)
			return;
		int update = ShouldUpdateList();
		LBLogger.Spam("Update: " + update, "AdvancedGroups");
		if (update == 0 && !force)
			return;
		if (update == 1 || force) {
			ClearAndDeleteEntries();
			CreateEntries();
		}
		LBLogger.Verbose("Updating PlayerList Widgets. Entry Count: " + entries.Count(), "AdvancedGroups");
		foreach (LBPlayerListEntry entry : entries) {
			entry.UpdateWidget();
		}
		LBLogger.Verbose("Updated all PlayerList Widgets", "AdvancedGroups");
	}

	void UpdateDistances() {
		foreach (LBPlayerListEntry entry : entries) {
			entry.UpdateDistance();
		}
	}

}
