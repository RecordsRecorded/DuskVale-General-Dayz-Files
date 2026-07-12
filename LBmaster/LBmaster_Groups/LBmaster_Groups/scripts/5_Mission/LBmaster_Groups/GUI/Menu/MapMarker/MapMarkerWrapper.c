class MapMarkerWrapper {

	float widgetWidth, widgetHeight;
	Widget widget;
	bool isDragged = false;

	private int layer = 0;

	vector position = vector.Zero;
	vector lastPosition = vector.Zero;
	string lastname = "";
	string lasticon = "";
	int lastColor = 0;

	bool changed = false;
	ref CanvasWidget drawCanvas;

	void ~MapMarkerWrapper() {
		if (widget) {
			widget.Unlink();
		}
	}

	void SetDrawCanvas(CanvasWidget canvas) {
		this.drawCanvas = canvas;
	}

	void SetLayer(int layer_) {
		layer = layer_;
	}

	int GetLayer() {
		return layer;
	}

	void Update(float mapScale, vector mapPos, MapWidget mapWidget, float xOffset, float yOffset) {
	}
	bool UpdateOptional(bool change = true) {
		if (changed) {
			if (change)
				changed = false;
			return true;
		}
		return false;
	}
	void UpdateVisibility(bool isMap) {
	}

	bool IsVisible() {
		if (!widget)
			return false;
		return widget.IsVisible();
	}

	bool SetPosition(vector pos) {
		if (vector.Distance(lastPosition, pos) > 1) {
			this.position = pos;
			this.lastPosition = pos;
			changed = true;
			return true;
		}
		return false;
	}

	bool SetColor(int color) {
		if (lastColor != color) {
			LBLogger.Debug("Set Color from " + lastColor + " to " + color, "AdvancedGroups");
			lastColor = color;
			changed = true;
			return true;
		}
		return false;
	}
}
