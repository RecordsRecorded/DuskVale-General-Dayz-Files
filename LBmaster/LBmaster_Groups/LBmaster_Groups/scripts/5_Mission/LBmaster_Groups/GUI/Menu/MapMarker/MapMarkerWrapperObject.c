class MapMarkerWrapperObject : MapMarkerWrapper {

	Object obj;

	ImageWidget icon;
	TextWidget name;

	void Init(Object obj2, int color, string theicon, string thename, Widget iconPane) {
		if (!iconPane)
			return;
		obj = obj2;
		widget = LBLayoutManager.Get().CreateLayout("MapMarker", iconPane);
		if (widget) {
			widget.Show(true);
			name = TextWidget.Cast(widget.FindAnyWidget("name"));
			icon = ImageWidget.Cast(widget.FindAnyWidget("icon"));
			SetIcon(theicon);
			SetColor(color);
			SetName("  " + thename);
			SetPosition(obj.GetPosition());
			widget.Update();
			widget.GetScreenSize(widgetWidth, widgetHeight);
		}
	}

	override void Update(float mapScale, vector mapPos, MapWidget mapWidget, float xOffset, float yOffset) {
		if (isDragged)
			return;
		vector screenPos = mapWidget.MapToScreen(GetPosition());

		widget.SetPos(screenPos[0] - widgetHeight / 2.0 - xOffset, screenPos[1] - widgetHeight / 2.0 - yOffset, true);
		if (name)
			name.SetTextExactSize(Math.Clamp(20.0, 10.0, 50.0));
		if (icon) {
			icon.SetSize(widgetHeight, widgetHeight);
		}
	}

	override void UpdateVisibility(bool isMap) {
		bool visible = IsMarkerVisible();
		widget.Show(visible);
	}

	bool IsMarkerVisible() {
		if (obj && PlayerBase.Cast(obj)) {
			if (LBMarkerSettingsConfig.Get && LBMarkerSettingsConfig.Get.canSeeOwnPlayerOnMap) {
				if (LBMarkerSettingsConfig.Get.requireItemToSeeOwnPlayerOnMap && !LBPlayerUtils.HasItemsInInventory(LBMarkerSettingsConfig.Get.ownPlayerMarkerItems))
					return false;
				return true;
			}
		}
		return false;
	}

	vector GetPosition() {
		if (obj)
			return obj.GetPosition();
		return position;
	}

	override bool UpdateOptional(bool change = true) {
		if (!change)
			return false;
		if (SetPosition(GetPosition())) {
			return true;
		}
		return false;
	}
	override bool SetColor(int color) {
		if (lastColor != color) {
			name.SetColor(color);
			icon.SetColor(color);
		}
		return super.SetColor(color);
	}

	bool SetIcon(string iconpath) {
		if (lasticon != iconpath && icon) {
			icon.LoadImageFile(0, iconpath);
			lasticon = iconpath;
			icon.Show(iconpath.Length() > 0 && FileExist(iconpath));
			return true;
		}
		return false;
	}

	bool SetName(string thename) {
		if (lastname != thename) {
			name.SetText(thename);
			lastname = thename;
			return true;
		}
		return false;
	}
}
