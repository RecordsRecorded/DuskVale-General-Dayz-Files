class LBMarkerListPage : LBGroupPage {

	ref LBMarkerList markerListManger;

	void LBMarkerListPage(LBPageSettingsType type_) {
		LBLayoutConfig_.Event_StreamerModeChanged.Insert(OnStreamerModeChange);
	}

	void ~LBMarkerListPage() {
		if (LBLayoutConfig_.Event_StreamerModeChanged)
			LBLayoutConfig_.Event_StreamerModeChanged.Remove(OnStreamerModeChange);
	}

	void OnStreamerModeChange(bool enabled) {
		markerListManger.UpdateEntries(parent, true);
	}

	override bool InitPage(LBGroupUI parentUI) {
		return super.InitPage(parentUI, 2, 0, LBPagesConfig.Get.GetButtonText(pageType), false, LBPagesConfig.Get.GetPagePriority(pageType));
	}

	int lastMarkerCount = 0;

	override void InitMainWidget() {
		markerListManger = new LBMarkerList();
		Widget marker_list = rootWidget.FindAnyWidget("marker_list");
		markerListManger.listWidget = marker_list;
	}

	override void OnUpdateSlow() {
		UpdateMarkerListManager();
	}

	override bool OnClick(Widget w) {
		LBMarkerListEntry clicked = markerListManger.OnIconPressed(w);
		if (clicked) {
			parent.mapMarkerManager.MoveToPoint(clicked.marker.position, 0.15);
			return true;
		}
		if (markerListManger.OnButtonPressed(w)) {
			markerListManger.UpdateEntries(parent);
			return true;
		}
		return false;
	}

	override void OnMarkerChanged() {
		markerListManger.UpdateEntries(parent, true);
	}

	override void OnShow() {
		super.OnShow();
		UpdateMarkerListManager();
		OnStreamerModeChange(LBLayoutConfig.Get.streamerModeEnabled);
	}

	void UpdateMarkerListManager() {
		int count = GetMarkerCount();
		LBLogger.Debug("GroupUI: UpdateMarkerListManager " + (markerListManger != null) + " " + count, "AdvancedGroups");
		if (markerListManger)
			markerListManger.UpdateEntries(parent, count != lastMarkerCount);
		lastMarkerCount = count;
	}

	int GetMarkerCount() {
		int count = 0;
		count += LBPrivateMarkerManager.Get.privateMarkers.Count();
		count += LBStaticMarkerManagerClient.Get().staticMarkers.Count();
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (pb && pb.GetLBGroup()) {
			count += pb.GetLBGroup().members.Count();
			count += pb.GetLBGroup().markers.Count();
		}
		return count;
	}

}