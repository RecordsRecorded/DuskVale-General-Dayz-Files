class LBAdminDrawCanvasObjectLine : LBAdminDrawCanvasObject {

	vector start, end;

	void LBAdminDrawCanvasObjectLine(vector start_, vector end_) {
		this.start = start_;
		this.end = end_;
	}

	override void Draw(CanvasWidget canvas) {
		vector screenStart = g_Game.GetScreenPos(start);
		vector screenEnd = g_Game.GetScreenPos(end);
		bool onStart = IsOnScreen(screenStart);
		bool onEnd = IsOnScreen(screenEnd);
		if (!onStart) {
			screenStart = FindOnLine(start, end);
		}
		if (!onEnd) {
			screenEnd = FindOnLine(end, start);
		}
		canvas.DrawLine(screenStart[0], screenStart[1], screenEnd[0], screenEnd[1], 1.0, ARGB(255, 255, 255, 255));
	}

	bool IsOnScreen(vector screenPos) {
		return screenPos[2] > 0 && screenPos[0] > 0 && screenPos[0] < LBWidgetUtils.screenWidth && screenPos[1] > 0 && screenPos[1] < LBWidgetUtils.screenHeight;
	}

	vector FindOnLine(vector p1, vector p2) {
		vector screenMid;
		for (int i = 0; i < 20; ++i) {
			vector mid = (p1 + p2) * 0.5;
			screenMid = g_Game.GetScreenPos(mid);
			if (IsOnScreen(screenMid)) {
				p2 = mid;
			} else {
				p1 = mid;
			}
		}
		screenMid = g_Game.GetScreenPos(p2);
		return screenMid;
	}

}