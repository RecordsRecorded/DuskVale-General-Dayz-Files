class LBGroupUI : LBMenuBase {

	const int TOP_BUTTON_COUNT = 7;

	bool typing = false;

	MapWidget mapWidget;
	Widget mapNotFound;
	ref LBMapMarkerManager mapMarkerManager;
	TextWidget txt_pos_x, txt_pos_y;
	ref Timer positionUpdateTimer = new Timer();

	Widget leftPanel, fullPanel, topPanel, topPanelContent;
	ref array<ref LBGroupPage> pages = new array<ref LBGroupPage>();
	LBGroupPage currentPage;
	Widget groupButton = null;
	CheckBoxWidget chckbx_dragMarkers;
	Widget mapLegend, mapLegendParent;

	TextWidget mapNotFoundText, txt_streamer_mode;
	ImageWidget mapNotFoundImage;

	ref LBAddMarkerPopup addPopup;
	ref LBClanTagColorDialog changeTagDialog;

	void LBGroupUI() {
		LBLayoutConfig_.Event_StreamerModeChanged.Insert(OnStreamerModeChange);
		LBColorManager_.Event_OnColorChange.Insert(OnColorChange);
		LBConfigManager.Get().GetEventOnConfigReceived("LB_NoBuildConfig").Insert(OnBuildZonesChange);
		LBWidgetUtils.Event_OnScreenSizeChanged.Insert(OnSizeChange);
		LBConfigManager.Get().GetEventOnConfigReceived("LBPagesConfig").Insert(OnPagesConfigReceived);
	}
	void ~LBGroupUI() {
		if (LBLayoutConfig_.Event_StreamerModeChanged)
			LBLayoutConfig_.Event_StreamerModeChanged.Remove(OnStreamerModeChange);
		if (LBColorManager_.Event_OnColorChange)
			LBColorManager_.Event_OnColorChange.Remove(OnColorChange);
		if (LBGroupPage.topButtons)
			LBGroupPage.topButtons.Clear();
		LBConfigManager.Get().GetEventOnConfigReceived("LB_NoBuildConfig").Remove(OnBuildZonesChange);
		LBWidgetUtils.Event_OnScreenSizeChanged.Remove(OnSizeChange);
		LBConfigManager.Get().GetEventOnConfigReceived("LBPagesConfig").Remove(OnPagesConfigReceived);
	}

	override bool SavePersistentState() {
		return true;
	}

	override string GetLayoutName() {
		return "GroupUI";
	}

	override TStringArray GetConnectIgnoredFields() {
		return {"groupButton"};
	}

	override TStringArray GetConnectRenameFields() {
		return {"mapWidget", "Map", "mapNotFound", "MapNotFound"};
	}

	override float GetBlur() {
		return 0.7;
	}

	override string GetTooltip(Widget w) {
		if (w == chckbx_dragMarkers)
			return "Enable this to drag markers around after they have been placed without having to delete them and place them again. Markers with a white background can be dragged anywhere";
		if (currentPage)
			return currentPage.GetTooltip(w);
		return "";
	}

	void OnStreamerModeChange(bool enabled) {
		LBLogger.Debug("OnStreamerModeChange LBGroupUI: " + enabled, "AdvancedGroups");
		ImageWidget serverLogo = ImageWidget.Cast(layoutRoot.FindAnyWidget("logo"));
		if (!enabled) {
			LBAppearanceConfig.Get.LoadLogo(serverLogo);
		} else if (serverLogo)
			serverLogo.Show(false);
		if (txt_streamer_mode)
			txt_streamer_mode.Show(enabled);
	}

	void OnColorChange() {
		AddMapMarker();
	}

	void StartUIPositionTimer() {
		if (LBPagesConfig.Get.infoPageSettings.showPlayerPositionCoordinates) {
			txt_pos_x.Show(true);
			txt_pos_y.Show(true);
			positionUpdateTimer.Stop();
			positionUpdateTimer.Run(0.1, this, "UpdateUIPosition", null, true);
		} else {
			txt_pos_x.Show(false);
			txt_pos_y.Show(false);
			positionUpdateTimer.Stop();
		}
	}

	void UpdateUIPosition() {
		Man player = g_Game.GetPlayer();
		if (!player) {
			txt_pos_x.SetText("");
			txt_pos_y.SetText("");
		} else {
			vector pos = player.GetPosition();
			txt_pos_x.SetText("X: " + FloatToString(pos[0], true));
			txt_pos_y.SetText("Z: " + FloatToString(pos[2], true));
		}
	}

	string FloatToString(float f, bool addPoint) {
		string txt = "" + f;
		int index = txt.IndexOf(".");
		if (addPoint) {
			if (index > 0)
				txt = txt.Substring(0, index + 2);
			else
				txt = txt + ".0";
		} else {
			if (index > 0)
				txt = txt.Substring(0, index);
		}
		return txt;
	}

	override void OnInit() {
		LBLogger.Debug("InitPageRest", "AdvancedGroups");
		mapMarkerManager = new LBMapMarkerManager(mapWidget, true);
		SetServerLogo();
		InitPages();
		ChangePageTo(pages.Get(GetDefaultPageNum()));
		LBLogger.Debug("Init Page Rest", "AdvancedGroups");
	}
	
	void OnPagesConfigReceived() {
		if (!layoutRoot)
			return;
		LBLogger.Debug("Recreating all Group Map Pages...", "AdvancedGroups");
		InitPages();
		ChangePageTo(pages.Get(GetDefaultPageNum()));
	}

	void OnBuildZonesChange() {
		AddMapMarker();
	}

	int GetDefaultPageNum() {
		return 0;
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		super.OnRPC(target, rpc_type, ctx);
		if (currentPage)
			currentPage.OnRPC(rpc_type, ctx);
	}

	void SetServerLogo() {
		ImageWidget serverLogo = ImageWidget.Cast(layoutRoot.FindAnyWidget("logo"));
		LBAppearanceConfig.Get.LoadLogo(serverLogo);
	}

	void CreateMapLegend() {
		if (mapLegend == null)
			return;
		LBLogger.Debug("Creating Map Legend ...", "AdvancedGroups");
		LBWidgetUtils.DeleteAllChildren(mapLegend);
		LBLogger.Debug("Show Map Legend ? " + LBGroupMiscConfig.Get.enableMapLegend, "AdvancedGroups");
		if (LBGroupMiscConfig.Get.enableMapLegend) {
			mapLegend.Show(true);
			mapLegendParent.Show(true);

			int i = 0;
			float w, h;
			if (LBGroupMiscConfig.Get.mapLegendTitle != "") {
				TextWidget legendTitle = TextWidget.Cast(LBLayoutManager.Get().CreateLayout("MapLegendTitle", mapLegend));
				legendTitle.SetText(LBGroupMiscConfig.Get.mapLegendTitle);
				legendTitle.GetScreenSize(w, h);
				legendTitle.SetPos(0, 5);
				mapLegend.SetSize(1, 5 + h + 5);
				i++;
			}
			foreach (MapLegendItem item : LBGroupMiscConfig.Get.mapLegend) {
				LBLogger.Verbose("Adding Map Legend Item: " + item.iconPath + " " + item.name, "AdvancedGroups");
				item.CreateLegendWidget(mapLegend, i);
				i++;
			}
			mapLegend.GetScreenSize(w, h);
			mapLegendParent.SetSize(w, h);
			LBLogger.Verbose("W:" + w + " H:" + h, "AdvancedGroups");
		} else {
			mapLegend.Show(false);
			mapLegendParent.Show(false);
		}
	}

	bool HasMapInInventory() {
		return LBPlayerUtils.HasItemsInInventory(LBGroupMiscConfig.Get.mapItems);
	}

	bool HasTranceiverInInventory() {
		return LBPlayerUtils.HasItemsInInventory(LBMarkerSettingsConfig.Get.playerMarkerItems);
	}

	void OpenGroupPage() {
		LBLogger.Verbose("Group Page is being opened. Button: " + groupButton, "AdvancedGroups");
		if (groupButton)
			SetCurrentPage(groupButton);
	}

	void InitPages() {
		LBLogger.Debug("Initializing all Group Map Pages...", "AdvancedGroups");
		LBGroupPage.topButtons.Clear();
		if (pages) {
			foreach (LBGroupPage page : pages) {
				if (page.rootWidget)
					page.rootWidget.Unlink();
				if (page.buttonWidget)
					page.buttonWidget.Unlink();
			}
		}
		LBLogger.Debug("Deleted all previous Page widgets", "AdvancedGroups");
		pages.Clear();
		if (addPopup && addPopup.addPopup) {
			addPopup.addPopup.Unlink();
		}
		if (changeTagDialog && changeTagDialog.rootWidget) {
			changeTagDialog.rootWidget.Unlink();
		}
		addPopup = new LBAddMarkerPopup();
		addPopup.Init(this);//*/
		changeTagDialog = new LBClanTagColorDialog();
		changeTagDialog.Init(this);

		InitPage(new LBInfoPage(LBPageSettingsType.INFO));
		InitPage(new LBGroupCreatePage(LBPageSettingsType.GROUP));
		InitPage(new LBGroupManagePage(LBPageSettingsType.GROUP));
		InitPage(new LBMarkerListPage(LBPageSettingsType.MARKERS));
		InitPage(new LBClientSettingsPage(LBPageSettingsType.SETTINGS));
		foreach (LBCustomPageSettings settings : LBPagesConfig.Get.customPages) {
			InitPage(new LBCustomPanelPage(LBPageSettingsType.CUSTOM, settings));
		}
		CreateCustomPages();
	}

	void InitPage(LBGroupPage page) {
		pages.Insert(page);
		page.InitPage(this);
	}

	LBGroupPage GetPageByType(LBPageSettingsType type) {
		foreach (LBGroupPage page : pages) {
			if (page.pageType == type)
				return page;
		}
		return null;
	}

	void CreateCustomPages() {

	}

	int GetCurrentPage() {
		return pages.Find(currentPage);
	}

	bool SetCurrentPage(int index) {
		LBGroupPage page = pages.Get(index);
		if (!page)
			return false;
		return SetCurrentPage(page.buttonWidget);
	}

	bool SetCurrentPage(Widget buttonClicked) {
		if (!buttonClicked)
			return false;
		foreach (LBGroupPage page : pages) {
			if (page.OnTopButtonClicked(buttonClicked)) {
				ChangePageTo(page);
				return true;
			}
		}
		return false;
	}

	void ReloadCurrentPage() {
		Widget btn = currentPage.buttonWidget;
		SetCurrentPage(btn);
	}

	void ChangePageTo(LBGroupPage page) {
		LBGroupPage oldPage = currentPage;
		currentPage = page;
		if (oldPage)
			oldPage.OnHide();
		page.OnShow();
	}

	void OnMarkerChanged() {
		if (currentPage)
			currentPage.OnMarkerChanged();
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (super.OnMouseEnter(w, x, y))
			return true;
		typing = (w && EditBoxWidget.Cast(w));
		if (currentPage && currentPage.OnMouseEnter(w, x, y))
			return true;
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y) {
		if (super.OnMouseLeave(w, enterW, x, y))
			return true;
		if (currentPage && currentPage.OnMouseLeave(w, x, y))
			return true;
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		LBLogger.Debug("Group On Click: " + w + " " + button, "AdvancedGroups");
		if (super.OnClick(w, x, y, button))
			return true;
		if (SetCurrentPage(w))
			return true;
		if (currentPage && currentPage.OnClick(w))
			return true;
		if (addPopup.OnClick(w))
			return true;
		if (w == chckbx_dragMarkers) {
			mapMarkerManager.SetDragable(chckbx_dragMarkers.IsChecked());
		}
		return false;
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (currentPage && currentPage.OnChange(w))
			return true;
		if (addPopup.OnChange(w))
			return true;
		return false;
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column,	int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (currentPage && currentPage.OnItemSelected(w, row, column))
			return true;
		return false;
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == mapWidget) {
			if (LBMarkerSettingsConfig.Get.disableMarkerPlacement && !LBAdmins.Get().IsActive())
				return true;
			if (button == 0) {
				addPopup.ShowPopup(x, y, false, null);
			} else if (button == 1) {
				vector mousePos = Vector(x + 10, y + 10, 0);
				vector mapPos = mapWidget.ScreenToMap(mousePos);
				LBMarker marker = addPopup.FindMarkerInRadius(mapPos);
				LBLogger.Debug("Edit Marker: " + marker, "AdvancedGroups");
				if (marker)
					addPopup.ShowPopup(x, y, true, marker);
			}
			return true;
		}
		if (currentPage && currentPage.OnDoubleClick(w))
			return true;
		return false;
	}

	void OnGroupChanged() {
		if (currentPage && !currentPage.IsInherited(LBGroupAdminPage)) {
			ReloadCurrentPage();
		}
		LBLogger.Info("UI On Group Changed", "AdvancedGroups");
		foreach (LBGroupPage page : pages)
			page.OnGroupChanged();
		if (addPopup)
			addPopup.OnGroupChanged();
		AddMapMarker();
		UpdateMarkerListLater();
	}

	override void Update(float timeslice) {
		super.Update(timeslice);
		if (currentPage) {
			currentPage.OnUpdateFrame();
		}
		if (addPopup)
			addPopup.OnUpdateFrame();
		if (mapMarkerManager)
			mapMarkerManager.UpdateFrame();
	}

	override void OnShow() {
		LBLogger.Debug("OnShow LBGroupUI", "AdvancedGroups");
		super.OnShow();

		OnStreamerModeChange(LBLayoutConfig.Get.streamerModeEnabled);

		LBMarker.hideAllMarkers = true;
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateAll, 1000, true);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckNewMarkers, 100, true);
		if (currentPage)
			currentPage.OnShow();
		AddMapMarker();
		AddCustomMarkersOnMapOpen(mapWidget, mapMarkerManager);
		AddCustomMarkersOnMapOpen();
		UpdateButtonVisibility();
		CreateMapLegend();
		CenterMapOnPlayer();
		StartUIPositionTimer();
		if (addPopup)
			addPopup.OnShow();
	}

	void UpdateButtonVisibility() {
		foreach (LBGroupPage page : pages) {
			page.UpdateButtonDisplay();
		}
	}

	bool IsMapVisible() {
		if (LBGroupMiscConfig.Get.mapRequireItem) {
			bool hasMap = HasMapInInventory();
			LBLogger.Debug("Map Requires an Item. Has Item ? " + hasMap, "AdvancedGroups");
			return hasMap;
		}
		return true;
	}

	bool IsOtherPlayersVisible() {
		return !LBMarkerSettingsConfig.Get.requireItemToSeeGroupMembers || HasTranceiverInInventory();
	}

	override void OnHide() {
		super.OnHide();
		LBMarker.hideAllMarkers = false;
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(UpdateAll);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckNewMarkers);
		if (currentPage)
			currentPage.OnHide();
		if (mapMarkerManager)
			mapMarkerManager.ClearMarkers();
		if (addPopup)
			addPopup.OnHide();
	}

	void UpdateAll() {
		if (currentPage)
			currentPage.OnUpdateSlow();
	}

	int lastGroupMarkerCount = 0, lastServerMarkerHash = 0, lastPrivateMarkerCount = 0, lastPlayerMarkerCount = 0, lastCustomMarkerCount = 0;
	bool lastNoBuildEnabled = false;

	void AddMapMarker() {
		AddMapMarker(mapWidget, mapMarkerManager, true);
	}
	void AddMapMarker(MapWidget mapWidget_, LBMapMarkerManager mapMarkerMgr, bool addPlayer) {
		if (!mapMarkerMgr)
			return;
		mapMarkerMgr.ClearMarkers();
		AddGroupMarkers(mapMarkerMgr);
		AddServerMarkers(mapMarkerMgr);
		AddPrivateMarkers(mapMarkerMgr);
		if (addPlayer)
			AddPlayerMarker(mapMarkerMgr);
		AddCustomMarkers(mapWidget_, mapMarkerMgr);
		AddCustomMarkers();
		if (LBMarkerVisibilityManager.Get.showNoBuildZones && LB_NoBuildConfig.Get && LB_NoBuildConfig.Get.enabled && LB_NoBuildConfig.Get.displayOnMap)
			AddNoBuildZones(mapMarkerMgr);
		if (currentPage)
			currentPage.AddMarkers(mapMarkerMgr);
		mapMarkerMgr.CutAllCircles();
		if (chckbx_dragMarkers)
			mapMarkerMgr.SetDragable(chckbx_dragMarkers.IsChecked());
		if (addPopup && addPopup.temp_marker)
			mapMarkerManager.AddMarker(addPopup.temp_marker);
	}

	void AddNoBuildZones(LBMapMarkerManager mapMarkerMgr) {
		if (!LB_NoBuildConfig.Get || !LB_NoBuildConfig.Get.enabled || (currentPage && currentPage.IsInherited(LBNoBuildZonesPage)))
			return;
		int color = LB_NoBuildConfig.Get.GetCircleColor();
		foreach (LB_NoBuildEntry entry : LB_NoBuildConfig.Get.zones) {
			mapMarkerMgr.AddCircleNonScaling(Vector(entry.x, 0, entry.y), entry.r, color, 55544, true);
		}
	}

	void CheckNewMarkers() {
		if (NeedMarkerRefresh())
			AddMapMarker();
	}

	bool NeedMarkerRefresh() {
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		bool need = false;
		bool noBuild = false;
		if (LBMarkerVisibilityManager.Get.showNoBuildZones && LB_NoBuildConfig.Get.enabled)
			noBuild = true;
		if (noBuild != lastNoBuildEnabled) {
			lastNoBuildEnabled = noBuild;
			need = true;
		}
		int groupMarkers = 0;
		if (pb && pb.GetLBGroup()) {
			LBGroup grp = pb.GetLBGroup();
			groupMarkers += grp.markers.Count();
			groupMarkers += grp.pings.Count();
			groupMarkers += grp.members.Count();
		}
		if (groupMarkers != lastGroupMarkerCount) {
			lastGroupMarkerCount = groupMarkers;
			need = true;
		}
		LBStaticMarkerManagerClient mgr = LBStaticMarkerManagerClient.Get();
		int staticHash = 0;
		foreach (LBServerMarker markServ : mgr.staticMarkers) {
			staticHash += markServ.CalcHash();
		}
		if (staticHash != lastServerMarkerHash) {
			lastServerMarkerHash = staticHash;
			need = true;
		}
		int clientMarkers = LBPrivateMarkerManager.Get.privateMarkers.Count();
		if (clientMarkers != lastPrivateMarkerCount) {
			lastPrivateMarkerCount = clientMarkers;
			need = true;
		}
		LBMarkerSettingsConfig_ cfg = LBMarkerSettingsConfig.Get;
		if (cfg && cfg.canSeeOwnPlayerOnMap && lastPlayerMarkerCount != 1) {
			lastPlayerMarkerCount = 1;
			need = true;
		} else if (cfg && !cfg.canSeeOwnPlayerOnMap && lastPlayerMarkerCount != 0) {
			lastPlayerMarkerCount = 0;
			need = true;
		}
		return need;
	}

	void UpdateMarkerListLater() {
		LBMarkerListPage listPage = LBMarkerListPage.Cast(GetPageByType(LBPageSettingsType.MARKERS));
		if (listPage) {
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(listPage.markerListManger.UpdateEntries, 500, false, this, true);
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(listPage.markerListManger.UpdateEntries, 1000, false, this, true);
		}
	}

	void AddCustomMarkersOnMapOpen() {
	}
	void AddCustomMarkersOnMapOpen(MapWidget mapWidget_, LBMapMarkerManager mapMarkerMgr) {
		mapWidget_.ClearUserMarks();
	}

	void AddCustomMarkers() {
	}
	void AddCustomMarkers(MapWidget mapWidget_, LBMapMarkerManager mapMarkerMgr) {
	}

	void AddPlayerMarker(LBMapMarkerManager mapMarkerMgr) {
		if (!g_Game.GetPlayer() || !LBMarkerSettingsConfig.Get.canSeeOwnPlayerOnMap)
			return;
		mapMarkerMgr.AddMarkerObject(g_Game.GetPlayer(), "#lb_ag_me", LBColorManager.Get.GetColor("Own Player Map Marker"), LBMarkerVisibilityManager.Get.playerMarkerIcon);
	}

	void CenterMapOnPlayer() {
		if (!g_Game.GetPlayer() || !LBMarkerSettingsConfig.Get.canSeeOwnPlayerOnMap)
			return;
		if (!LBMarkerVisibilityManager.Get.centerMapOnPlayer && !FoWShouldCenterOnPlayer())
			return;
		vector position = g_Game.GetPlayer().GetPosition();
		mapWidget.SetScale(0.2);
		mapWidget.SetMapPos(position);
	}

	bool FoWShouldCenterOnPlayer() {
		if (!LBFOWConfig.Get.centerOnPlayerIfNotOnScreen || !LBFOWConfig.Get.enabled || LBFOWConfig.Get.CanIgnoreFOW())
			return false;
		float mapW, mapH, mapX, mapY;
		mapWidget.GetScreenPos(mapX, mapY);
		mapWidget.GetScreenSize(mapW, mapH);
		vector topLeft = mapWidget.ScreenToMap(Vector(mapX, mapY, 0));
		vector bottomRight = mapWidget.ScreenToMap(Vector(mapX + mapW, mapY + mapH, 0));
		vector playerPos = g_Game.GetPlayer().GetPosition();
		return playerPos[0] < topLeft[0] || playerPos[0] > bottomRight[0] || playerPos[2] < bottomRight[2] || playerPos[2] > topLeft[2];
	}

	void AddGroupMarkers(LBMapMarkerManager mapMarkerMgr) {
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb || !pb.GetLBGroup())
			return;
		LBGroup grp = pb.GetLBGroup();
		if (!grp)
			return;
		foreach (LBMarker marker : grp.markers) {
			mapMarkerMgr.AddMarker(marker);
		}
		foreach (LBMarker marker2 : grp.pings) {
			mapMarkerMgr.AddMarker(marker2);
		}
		if (IsOtherPlayersVisible()) {
			foreach (LBGroupMember member : grp.members) {
				mapMarkerMgr.AddMarker(member);
			}
		}
	}

	bool AddServerMarkers(LBMapMarkerManager mapMarkerMgr) {
		LBStaticMarkerManagerClient mgr = LBStaticMarkerManagerClient.Get();
		foreach (LBServerMarker marker : mgr.staticMarkers) {
			mapMarkerMgr.AddMarker(marker);
		}
		return true;
	}

	bool AddPrivateMarkers(LBMapMarkerManager mapMarkerMgr) {
		foreach (LBMarker marker : LBPrivateMarkerManager.Get.privateMarkers) {
			mapMarkerMgr.AddMarker(marker);
		}
		return true;
	}

	void OnSizeChange() {
		foreach (LBGroupPage page : pages) {
			page.OnSizeChange();
		}
	}

	override bool OnDrag(Widget w, int x, int y) {
		if (mapMarkerManager)
			mapMarkerManager.OnDragStart(w);
		LBLogger.Debug("OnDrag: " + w, "AdvancedGroups");
		return true;
	}

	override bool OnDragging(Widget w, int x, int y, Widget reciever) {
		LBLogger.Debug("OnDragging: " + w, "AdvancedGroups");
		return true;
	}

	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever) {
		LBLogger.Debug("OnDraggingOver: " + w, "AdvancedGroups");
		return true;
	}

	override bool OnDrop(Widget w, int x, int y, Widget reciever) {
		LBLogger.Debug("OnDrop: " + w + " at: " + x + "," + y, "AdvancedGroups");
		vector worldpos = mapWidget.ScreenToMap(Vector(x, y, 0));
		if (LBFOWConfig.Get.CannotInteractWith(worldpos, !LBFOWConfig.Get.canAddMarkers)) {
			return true;
		}
		worldpos[1] = g_Game.SurfaceY(worldpos[0], worldpos[2]);
		if (mapMarkerManager) {
			LBMarker marker = mapMarkerManager.FindMarkerByMainWidget(w);
			if (marker) {
				marker.SetPosition(worldpos);
				marker.SendPositionToServer();
			}
			mapMarkerManager.OnDragStop(w);
			mapMarkerManager.UpdateFrame(true);
		}
		return true;
	}
}