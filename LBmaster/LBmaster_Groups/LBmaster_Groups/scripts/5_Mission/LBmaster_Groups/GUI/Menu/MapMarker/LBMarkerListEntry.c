class LBMarkerListEntry {

	LBGroupUI groupUI;
	LBMarker marker;

	Widget mainWidget, icon_btn;
	TextWidget name;
	ButtonWidget btn_0, btn_1;
	ImageWidget icon;

	string spacername;
	bool spacer = false;
	bool expanded = true;
	LBMarkerType type;

	void Delete() {
		if (mainWidget)
			mainWidget.Unlink();
	}

	void InitSpacer(Widget parent, string namee, LBMarkerType type_) {
		this.type = type_;
		spacer = true;
		spacername = namee;
		mainWidget = LBLayoutManager.Get().CreateLayout("Map Marker List Entry", parent);
		ConnectClassWidgetVariables(this, mainWidget, {"mainWidget"});
		mainWidget.FindAnyWidget("spacer").Show(false);
		icon.Show(false);
		btn_0.SetText("3D");
		btn_1.SetText("v");
		name.SetText(" " + namee);
		name.SetBold(true);
		Show(false);
	}

	void InitMarker(Widget parent, LBMarker mmarker, LBGroupUI groupui) {
		this.groupUI = groupui;
		mainWidget = LBLayoutManager.Get().CreateLayout("Map Marker List Entry", parent);
		ConnectClassWidgetVariables(this, mainWidget, {"mainWidget"});
		btn_0.SetText("3D");
		btn_1.SetText("E");
		btn_1.Enable(mmarker.type != LBMarkerType.GROUP_PLAYER_MARKER);
		name.SetText(" " + mmarker.name);
		string iconPath = mmarker.GetIcon();

		if (iconPath.Length() > 0) {
			icon.LoadImageFile(0, iconPath);
		} else {
			icon.Show(false);
		}
		icon.SetColor(mmarker.GetColorARGB());
		Show(false);
		InitMarker(mmarker);
	}

	void InitMarker(LBMarker mmarker) {
		marker = mmarker;
	}

	void UpdateWidget() {
		int state = 0;
		if (spacer) {
			state = LBMarkerVisibilityManager.Get.GetGlobalVisibilityOrAdd(type).displaystate;
		} else {
			if (marker) {
				state = LBMarkerVisibilityManager.Get.GetVisibilityOrAdd(marker.uid).displaystate;
				bool hideFow = false;
				switch (marker.type) {
					case LBMarkerType.SERVER_STATIC:
					case LBMarkerType.SERVER_DYNAMIC:
						if (LBFOWConfig.Get.CannotInteractWith(marker.position, LBFOWConfig.Get.hideServerMarkersInList))
							hideFow = true;
						break;
					case LBMarkerType.GROUP_MARKER:
					case LBMarkerType.GROUP_PING:
						if (LBFOWConfig.Get.CannotInteractWith(marker.position, LBFOWConfig.Get.hideGroupMarkersInList))
							hideFow = true;
						break;
					case LBMarkerType.PRIVATE_MARKER:
						if (LBFOWConfig.Get.CannotInteractWith(marker.position, LBFOWConfig.Get.hidePrivateMarkersInList))
							hideFow = true;
						break;
				}
				if (hideFow) {
					Show(false);
					return;
				}
			}
		}
		if (state == 0)
			btn_0.SetText("3D");
		else if (state == 1)
			btn_0.SetText("2D");
		else
			btn_0.SetText("O");
		Show(true);
	}

	bool OnButtonPressed(Widget w) {
		if (w == btn_0) {
			if (spacer) {
				LBMarkerVisibilityManager.Get.GetGlobalVisibilityOrAdd(type).GetNextState();
			} else {
				LBMarkerVisibilityManager.Get.GetVisibilityOrAdd(marker.uid).GetNextState();
			}
			return true;
		} else if (w == btn_1) {
			if (spacer) {
				if (expanded) {
					Collapse();
				} else {
					Expand();
				}
			} else {
				if (groupUI)
					groupUI.addPopup.ShowPopup(0, 0, true, marker);
			}
			return true;
		}
		return false;
	}

	void Show(bool b) {
		if (mainWidget)
			mainWidget.Show(b);
	}

	void Expand() {
		expanded = true;
		if (btn_1)
			btn_1.SetText("v");
	}

	void Collapse() {
		expanded = false;
		if (btn_1)
			btn_1.SetText("^");
	}

}