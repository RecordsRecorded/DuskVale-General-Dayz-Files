class LBMapMarkerManager {
	private MapWidget map_widget;
	private CanvasWidget drawCanvas;
	private Widget iconPane;

	private ref LB_FOW_Drawer fow_drawer;
	private ref array<ref MapMarkerWrapper> markers = new array<ref MapMarkerWrapper>();
	vector lastMapPos = vector.Zero;
	float lastMapScale = 0.0;
	float xOffset = 0, yOffset = 0;
	bool isMap = true;

	void LBMapMarkerManager(MapWidget mapWidget, bool enableFogOfWarDraw) {
		if (!mapWidget) {
			LBLogger.Fatal("Map Marker Manager did not get Map Widget! Cannot properly display Markers!", "AdvancedGroups");
			return;
		}
		map_widget = mapWidget;
		iconPane = map_widget.FindAnyWidget("iconPane");
		drawCanvas = CanvasWidget.Cast(map_widget.FindAnyWidget("drawCanvas"));
		if (enableFogOfWarDraw)
			fow_drawer = new LB_FOW_Drawer(drawCanvas, map_widget);
		g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateSlowVisibility, 100, true);
		LBLayoutConfig_.Event_StreamerModeChanged.Insert(OnStreamerModeChange);
		UpdateOffsets();
	}

	void UpdateFOWSizes() {
		if (fow_drawer)
			fow_drawer.UpdateSizes();
	}

	void ~LBMapMarkerManager() {
		ClearMarkers();
		if (g_Game && g_Game.GetCallQueue(CALL_CATEGORY_GUI))
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).Remove(UpdateSlowVisibility);
		if (LBLayoutConfig_.Event_StreamerModeChanged)
			LBLayoutConfig_.Event_StreamerModeChanged.Remove(OnStreamerModeChange);
	}

	void UpdateOffsets() {
		iconPane.GetScreenPos(xOffset, yOffset);
	}

	void SetDragable(bool drag) {
		float scale = map_widget.GetScale();
		vector pos = map_widget.GetMapPos();
		foreach (MapMarkerWrapper wrap : markers) {
			MapMarkerWrapperLBMarker mark;
			if (Class.CastTo(mark, wrap)) {
				mark.SetDragable(drag);
				mark.Update(scale, pos, map_widget, xOffset, yOffset);
				mark.UpdateVisibility(isMap);
			}
		}
	}

	void UpdateFrame(bool force = false) {
		float scale = map_widget.GetScale();
		vector pos = map_widget.GetMapPos();
		UpdateOffsets();
		bool updateCircles = false;
		UpdateWrapperChanged(scale, pos, force);
		UpdateCircles(updateCircles);
		if (updateCircles)
			drawCanvas.Clear();
		UpdateWrapperOptional(scale, pos, updateCircles);
		if (force || scale != lastMapScale || vector.Distance(pos, lastMapPos) > 0.01) {
			DrawFogOfWar();
		}
		lastMapScale = scale;
		lastMapPos = pos;
	}

	void DrawFogOfWar() {
		if (fow_drawer)
			fow_drawer.DrawFogOfWar();
	}

	void UpdateWrapperChanged(float scale, vector pos, bool force) {
		if (force || scale != lastMapScale || vector.Distance(pos, lastMapPos) > 0.01) {
			drawCanvas.Clear();
			foreach (MapMarkerWrapper wrapper : markers) {
				if (wrapper.IsVisible())
					wrapper.Update(scale, pos, map_widget, xOffset, yOffset);
			}
		}
	}

	void UpdateCircles(out bool updateCircles) {
		foreach (MapMarkerWrapper wrapper3 : markers) {
			MapMarkerWrapperCircle circle;
			if (!Class.CastTo(circle, wrapper3)) {
				MapMarkerWrapperLBMarker markerWrapper;
				if (Class.CastTo(markerWrapper, wrapper3)) {
					circle = markerWrapper.circle;
				}
			}
			if (circle != null && circle.UpdateOptional(false)) {
				updateCircles = true;
				break;
			}
		}
	}

	void UpdateWrapperOptional(float scale, vector pos, bool updateCircles) {
		foreach (MapMarkerWrapper wrapper2 : markers) {
			if (wrapper2.IsVisible() && (wrapper2.UpdateOptional() || updateCircles)) {
				wrapper2.Update(scale, pos, map_widget, xOffset, yOffset);
			}
		}
	}

	void OnStreamerModeChange(bool enabled) {
		UpdateSlowVisibility();
	}

	void UpdateSlowVisibility() {
		//LBLogger.Verbose("UpdateSlowVisibility " + markers.Count(), "AdvancedGroups");
		foreach (MapMarkerWrapper wrapper : markers) {
			wrapper.UpdateVisibility(isMap);
		}
	}

	void ClearMarkers() {
		if (markers)
			markers.Clear();
		if (drawCanvas)
			drawCanvas.Clear();
	}

	private void AddMarker(MapMarkerWrapper wrapper) {
		markers.Insert(wrapper);
		if (wrapper.IsVisible()) {
			float scale = map_widget.GetScale();
			vector pos = map_widget.GetMapPos();
			wrapper.Update(scale, pos, map_widget, xOffset, yOffset);
			wrapper.UpdateVisibility(isMap);
			UpdateFrame();
		}
	}

	vector GetMousePosWld() {
		int x, y;
		GetMousePos(x, y);
		vector mouse = Vector(x, y, 0);
		vector mapPos = map_widget.ScreenToMap(mouse);
		return mapPos;
	}

	MapMarkerWrapperLBMarker AddMarker(LBMarker marker) {
		MapMarkerWrapperLBMarker wrapper = new MapMarkerWrapperLBMarker();
		wrapper.SetDrawCanvas(drawCanvas);
		wrapper.Init(marker, iconPane);
		wrapper.SetLayer(marker.circleLayer);
		AddMarker(wrapper);
		return wrapper;
	}

	MapMarkerWrapperObject AddMarkerObject(Object obj, string name = "", int color = 0xFFFFFFFF, string icon = "", int layer = 0) {
		MapMarkerWrapperObject wrapper = new MapMarkerWrapperObject();
		wrapper.SetDrawCanvas(drawCanvas);
		wrapper.Init(obj, color, icon, name, iconPane);
		wrapper.SetLayer(layer);
		AddMarker(wrapper);
		return wrapper;
	}

	MapMarkerWrapperCircle AddCircleNonScaling(vector pos, float radius, int color, int layer = 0, bool striked = false) {
		MapMarkerWrapperCircle wrapper = new MapMarkerWrapperCircle();
		wrapper.SetDrawCanvas(drawCanvas);
		wrapper.Init(pos, radius, color, striked);
		wrapper.SetLayer(layer);
		AddMarker(wrapper);
		return wrapper;
	}

	void RemoveLayer(int layer) {
		for (int i = 0; i < markers.Count(); i++) {
			MapMarkerWrapper wrapper = markers.Get(i);
			if (wrapper && wrapper.GetLayer() == layer) {
				markers.Remove(i);
				i--;
			}
		}
		UpdateFrame(true);
	}

	void CutAllCircles() {
		map<int, ref array<MapMarkerWrapperCircle>> circles = new map<int, ref array<MapMarkerWrapperCircle >> ();
		for (int i = 0; i < markers.Count(); i++) {
			MapMarkerWrapper wrapper = markers.Get(i);
			MapMarkerWrapperCircle circle;
			MapMarkerWrapperLBMarker marker;
			if (Class.CastTo(circle, wrapper) || Class.CastTo(marker, wrapper) && marker && Class.CastTo(circle, marker.circle)) {
				array<MapMarkerWrapperCircle> arr;
				if (circles.Contains(circle.GetLayer()))
					arr = circles.Get(circle.GetLayer());
				else {
					arr = new array<MapMarkerWrapperCircle>();
					circles.Insert(circle.GetLayer(), arr);
				}
				arr.Insert(circle);
			}
		}
		LBLogger.Debug("Circle Layers: " + circles.Count(), "AdvancedGroups");
		foreach (int layer, array<MapMarkerWrapperCircle> circ : circles) {
			LBLogger.Verbose("Layer: " + layer + " Circles: " + circ.Count(), "AdvancedGroups");
			foreach (MapMarkerWrapperCircle cir : circ) {
				cir.SetOtherCircles(circ);
			}
		}
		UpdateFrame(true);
	}

	void RemoveMarker(LBMarker marker) {
		for (int i = 0; i < markers.Count(); i++) {
			MapMarkerWrapper wrapper = markers.Get(i);
			MapMarkerWrapperLBMarker wrapCast;
			if (!Class.CastTo(wrapCast, wrapper))
				continue;
			if (wrapCast && wrapCast.marker == marker) {
				markers.Remove(i);
				i--;
				UpdateFrame(true);
			}
		}
	}

	MapMarkerWrapper FindByMainWidget(Widget w) {
		for (int i = 0; i < markers.Count(); i++) {
			MapMarkerWrapper wrapper = markers.Get(i);
			if (wrapper.widget == w)
				return wrapper;
		}
		return null;
	}

	LBMarker FindMarkerByMainWidget(Widget w) {
		MapMarkerWrapperLBMarker wrap = MapMarkerWrapperLBMarker.Cast(FindByMainWidget(w));
		if (!wrap)
			return null;
		return wrap.marker;
	}

	void OnDragStart(Widget w) {
		MapMarkerWrapper wrap = FindByMainWidget(w);
		if (!wrap)
			return;
		wrap.isDragged = true;
	}

	void OnDragStop(Widget w) {
		MapMarkerWrapper wrap = FindByMainWidget(w);
		if (!wrap)
			return;
		wrap.isDragged = false;
	}

	void MoveToPoint(vector pos, float scale) {
		map_widget.SetScale(scale);
		map_widget.SetMapPos(pos);
		UpdateFrame();
	}

}
