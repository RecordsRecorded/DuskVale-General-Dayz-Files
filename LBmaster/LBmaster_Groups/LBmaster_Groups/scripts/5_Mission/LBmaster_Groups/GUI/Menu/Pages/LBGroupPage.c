class LBGroupPage {

	static ref map<int, ButtonWidget> topButtons = new map<int, ButtonWidget>();

	LBGroupUI parent;
	string buttonname;
	bool fullsized = false;
	int pageID, pageSubID, priority;
	bool over_map = false;
	int last_map_x = -1, last_map_y = -1;

	ButtonWidget buttonWidget;
	Widget rootWidget;
	LBPageSettingsType pageType;

	void LBGroupPage(LBPageSettingsType type_) {
		this.pageType = type_;
	}

	bool InitPage(LBGroupUI parentUI) {
		return false;
	}

	bool CanDisplayButton() {
		LBPageSettings settings = LBPagesConfig.Get.GetSettings(pageType);
		return !settings || settings.enabled;
	}

	bool InitPage(LBGroupUI parentUI, int pageID_, int pageSubID_, string name, bool fullsize, int priority_ = 0) {
		parent = parentUI;
		buttonname = name;
		fullsized = fullsize;
		pageID = pageID_;
		pageSubID = pageSubID_;
		priority = priority_;
		return InitWidgets();
	}

	string GetLayoutName() {
		return "Map Page " + pageID + " " + pageSubID;
	}

	string GetTooltip(Widget w) {
	}

	bool InitWidgets() {
		string layoutName = GetLayoutName();
		Widget parentWidget;
		if (fullsized) {
			parentWidget = parent.fullPanel;
		} else {
			parentWidget = parent.leftPanel;
		}
		LBLogger.Debug("Creating Layout: " + layoutName, "AdvancedGroups");
		rootWidget = LBLayoutManager.Get().CreateLayout(layoutName, parentWidget);
		LBLogger.Debug("Widget: " + rootWidget, "AdvancedGroups");
		rootWidget.Show(false);
		InitMainWidget();

		if (!topButtons.Contains(pageID) || LBCustomPanelPage.Cast(this)) {
			layoutName = "Map Top Button";
			LBLogger.Debug("Creating new Top Button for PageID: " + pageID + " with Layout " + layoutName + " with a priority of " + priority, "AdvancedGroups");
			Widget btnWid = LBLayoutManager.Get().CreateLayout(layoutName, parent.topPanelContent);
			btnWid.SetSort(1000 - priority); // Higher Sort will be put at the end. We need to reverse it here
			buttonWidget = ButtonWidget.Cast(btnWid);
			if (buttonWidget) {
				buttonWidget.SetText(buttonname);
				topButtons.Insert(pageID, buttonWidget);
			}
		} else {
			LBLogger.Debug("Getting Top Button for PageID: " + pageID + " with Layout " + layoutName, "AdvancedGroups");
			buttonWidget = topButtons.Get(pageID);
		}
		UpdateButtonDisplay();
		return true;
	}

	void UpdateButtonDisplay() {
		if (buttonWidget) {
			buttonWidget.Show(CanDisplayButton());
		}
	}

	bool OnTopButtonClicked(Widget w) {
		return (w == buttonWidget) && CanDisplayButton();
	}

	bool OnClick(Widget w) {
		return false;
	}

	bool OnChange(Widget w) {
		return false;
	}

	bool OnItemSelected(Widget w, int row, int column) {
		return false;
	}

	bool OnDoubleClick(Widget w) {
		return false;
	}

	bool OnMouseEnter(Widget w, int x, int y) {
		if (w == parent.mapWidget) {
			over_map = true;
		}
		return false;
	}

	bool OnMouseLeave(Widget w, int x, int y) {
		if (w == parent.mapWidget) {
			over_map = false;
		}
		return false;
	}

	void OnGroupChanged() {

	}

	void InitMainWidget() {

	}

	void OnShow() {
		if (rootWidget)
			rootWidget.Show(true);
		parent.leftPanel.Show(!fullsized);
		bool mapVisible = parent.IsMapVisible();
		parent.mapWidget.Show(!fullsized && mapVisible);
		parent.mapNotFound.Show(!fullsized && !mapVisible);
		parent.mapNotFoundImage.LoadImageFile(0, LBGroupMiscConfig.Get.mapNotFoundImage);
		parent.mapNotFoundText.SetText(LBGroupMiscConfig.Get.mapNotFoundText);
		parent.fullPanel.Show(fullsized);
	}

	void OnHide() {
		if (rootWidget)
			rootWidget.Show(false);
	}

	void OnUpdateFrame() {
		if (over_map && GetMouseState(MouseState.LEFT) < 0 && last_map_x == -1 && last_map_y == -1) {
			GetMousePos(last_map_x, last_map_y);
		} else if (over_map && GetMouseState(MouseState.LEFT) >= 0) {
			int x, y;
			GetMousePos(x, y);
			if (x == last_map_x && y == last_map_y) {
				OnClick(parent.mapWidget);
			}
			last_map_y = -1;
			last_map_x = -1;
		}
	}

	void OnRPC(int type, ParamsReadContext ctx);
	void AddMarkers(LBMapMarkerManager mapMarkerManager) {}
	void OnUpdateSlow() {}
	void OnMarkerChanged() {}
	void OnSizeChange() {}
}
