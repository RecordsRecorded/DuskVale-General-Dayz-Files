class MapMarkerWrapperLBMarker : MapMarkerWrapper {

	LBMarker marker;
	ImageWidget icon;
	TextWidget name;
	bool dragable = false, canDrag = false;
	Widget iconPane_;
	ref MapMarkerWrapperCircle circle = null;

	void Init(LBMarker marker2, Widget iconPane) {
		iconPane_ = iconPane;
		if (!iconPane || !marker2)
			return;
		marker = marker2;
		widget = CreateLayout(iconPane);
		if (widget) {
			widget.Show(true);
			name = TextWidget.Cast(widget.FindAnyWidget("name"));
			icon = ImageWidget.Cast(widget.FindAnyWidget("icon"));
			SetIcon(marker.GetIcon());
			SetColor(marker.GetColorARGB());
			SetName(" " + marker.name);
			SetPosition(marker.position);
			widget.Update();
			widget.GetScreenSize(widgetWidth, widgetHeight);
		}
	}

	override int GetLayer() {
		if (marker)
			return marker.circleLayer;
		return super.GetLayer();
	}

	void ReInit() {
		if (widget)
			widget.Unlink();
		lastPosition = vector.Zero;
		lastname = "";
		lasticon = "";
		lastColor = 0;
		Init(marker, iconPane_);
	}

	Widget CreateLayout(Widget iconPane) {
		if (dragable && marker && (marker.type == LBMarkerType.GROUP_MARKER || marker.type == LBMarkerType.PRIVATE_MARKER)) {
			canDrag = true;
			return LBLayoutManager.Get().CreateLayout("MapMarker_Dragable", iconPane);
		}
		return LBLayoutManager.Get().CreateLayout("MapMarker", iconPane);
	}

	void SetDragable(bool drag_) {
		if (drag_ != dragable) {
			dragable = drag_;
			ReInit();
		}
	}

	override void Update(float mapScale, vector mapPos, MapWidget mapWidget, float xOffset, float yOffset) {
		if (!marker)
			return;
		if (isDragged)
			return;
		vector screenPos = mapWidget.MapToScreen(GetPosition());

		widget.SetPos(screenPos[0] - widgetHeight / 2.0 - xOffset, screenPos[1] - widgetHeight / 2.0 - yOffset, true);
		if (name)
			name.SetTextExactSize(Math.Clamp(20.0, 10.0, 50.0));
		if (icon) {
			icon.SetSize(widgetHeight, widgetHeight);
		}
		if (circle)
			circle.Update(mapScale, mapPos, mapWidget, xOffset, yOffset);
	}

	override void UpdateVisibility(bool isMap) {
		if (!marker)
			return;
		if (marker && (marker.type == LBMarkerType.SERVER_STATIC || marker.type == LBMarkerType.SERVER_DYNAMIC) && LBLayoutConfig.Get.streamerModeEnabled) {
			widget.Show(false);
			return;
		}
		bool visible = IsMarkerVisible(isMap);
		widget.Show(visible);
		if (circle)
			circle.UpdateVisibility(isMap);
	}

	bool IsMarkerVisible(bool isMap) {
		LBGroupMember memberMarker;
		if (!marker)
			return false;
		if (marker.type == LBMarkerType.GROUP_PLAYER_MARKER && Class.CastTo(memberMarker, marker)) {
			string mysteamid = LBAdmins.Get().GetMySteamid();
			if (memberMarker.steamid == mysteamid)
				return false;
		}
		if (marker.type == -1)
			return true;
		return marker.ShowMarkerMapOrGPS(isMap);
	}

	vector GetPosition() {
		if (marker)
			return marker.position;
		return position;
	}

	override bool SetColor(int color) {
		if (dragable && canDrag)
			color = ARGB(255, 0, 0, 0);
		if (lastColor != color) {
			name.SetColor(color);
			icon.SetColor(color);
		}
		return super.SetColor(color);
	}
	override bool UpdateOptional(bool change = true) {
		if (!change)
			return false;
		if (marker) {
			if (SetPosition(marker.position) | SetColor(marker.GetColorARGB()) | SetIcon(marker.GetIcon()) | SetName(marker.name) | SetRadius(marker.position, marker.circleRadius, marker.GetCircleColor(), marker.circleStriked)) {
				return true;
			}
		}
		if (circle && circle.UpdateOptional(change))
			return true;
		return false;
	}

	bool SetRadius(vector pos, float circleRadius, int circleColor, bool striked) {
		if (circleRadius <= 0) {
			circle = null;
			return false;
		}
		if (circle == null) {
			circle = new MapMarkerWrapperCircle();
			circle.SetDrawCanvas(drawCanvas);
			circle.Init(pos, circleRadius, circleColor, striked);
			if (marker.circleLayer < 0)
				circle.SetLayer(marker.uid);
			else
				circle.SetLayer(marker.circleLayer);
			return true;
		}
		return circle.SetPosition(pos) | circle.SetRadius(circleRadius) | circle.SetColor(circleColor) | circle.SetStriked(striked);
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
