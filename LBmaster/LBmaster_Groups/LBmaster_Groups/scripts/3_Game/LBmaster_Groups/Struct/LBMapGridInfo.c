class LBMapGridInfo {

	int offsetX = 0, offsetY = 0;
	int worldSize = 0;
	int gridColor;
	ref array<ref Param3<int, int, float>> gridZoomLevels = new array<ref Param3<int, int, float >> (); // stepX, stepY, zoomMax

	void LBMapGridInfo() {
		string world = g_Game.GetWorldName();
		gridColor = -1;
		if (g_Game.ConfigIsExisting("CfgWorlds " + world + " Grid")) {
			offsetX = g_Game.ConfigGetInt("CfgWorlds " + world + " Grid offsetX");
			offsetY = g_Game.ConfigGetInt("CfgWorlds " + world + " Grid offsetY");
			worldSize = g_Game.GetWorld().GetWorldSize();
			int children = g_Game.ConfigGetChildrenCount("CfgWorlds " + world + " Grid");
			for (int i = 0; i < children; ++i) {
				string gridName;
				g_Game.ConfigGetChildName("CfgWorlds " + world + " Grid", i, gridName);
				int stepX = 0, stepY = 0;
				float zoomMax = 1.0;
				if (g_Game.ConfigIsExisting("CfgWorlds " + world + " Grid " + gridName + " stepX"))
					stepX = g_Game.ConfigGetInt("CfgWorlds " + world + " Grid " + gridName + " stepX");
				if (g_Game.ConfigIsExisting("CfgWorlds " + world + " Grid " + gridName + " stepY"))
					stepY = g_Game.ConfigGetInt("CfgWorlds " + world + " Grid " + gridName + " stepY");
				if (g_Game.ConfigIsExisting("CfgWorlds " + world + " Grid " + gridName + " zoomMax"))
					zoomMax = g_Game.ConfigGetFloat("CfgWorlds " + world + " Grid " + gridName + " zoomMax");
				if (stepX != 0 && stepY != 0)
					gridZoomLevels.Insert(new Param3<int, int, float>(stepX, stepY, zoomMax));
			}
		}
		if (LBFOWConfig.Get.overwriteColorFromConfig) {
			gridColor = LBFOWConfig.Get.gridColorOverwrite.GetColorARGB();
		} else if (g_Game.ConfigIsExisting("RscMapControl colorGrid")) {
			TFloatArray gridColorFloats = new TFloatArray();
			g_Game.ConfigGetFloatArray("RscMapControl colorGrid", gridColorFloats);
			if (gridColorFloats.Count() >= 4) {
				gridColor = ARGBF(gridColorFloats.Get(3) * 0.2, gridColorFloats.Get(0), gridColorFloats.Get(1), gridColorFloats.Get(2));
			}
		}
	}

	void DrawGridLines(MapWidget mapWidget, CanvasWidget canvas) {
		if (!LBFOWConfig.Get.drawMapGridOverFog)
			return;
		float currentZoom = mapWidget.GetScale();
		float mapX, mapY, mapW, mapH, canW, canH;
		mapWidget.GetScreenPos(mapX, mapY);
		mapWidget.GetScreenSize(mapW, mapH);
		canvas.GetScreenSize(canW, canH);
		vector worldStart = mapWidget.ScreenToMap(Vector(mapX, mapY, 0));
		vector worldEnd = mapWidget.ScreenToMap(Vector(mapX + mapW, mapY + mapH, 0));
		float worldToCanX = canW / (worldEnd[0] - worldStart[0]);
		float worldToCanY = canH / (worldEnd[2] - worldStart[2]);
		int size = g_Game.GetWorld().GetWorldSize();
		foreach (Param3<int, int, float> gridZoom : gridZoomLevels) {
			if (gridZoom.param3 < currentZoom)
				continue;
			int x = offsetX + GetNextFullNumber(worldStart[0] - offsetX, gridZoom.param1);
			int sizeOffset = gridZoom.param2 - (size % gridZoom.param2);
			int y = offsetY + GetNextFullNumber(worldEnd[2] - offsetY, gridZoom.param2) - sizeOffset;
			float xEnd = worldEnd[0];
			float yEnd = worldStart[2];
			for (; x < xEnd; x = x + gridZoom.param1) {
				float cX = (x - worldStart[0]) * worldToCanX;
				canvas.DrawLine(cX, 0, cX, canH, 1, gridColor);
			}
			for (; y < yEnd; y += gridZoom.param2) {
				float cY = (y - worldStart[2]) * worldToCanY;
				canvas.DrawLine(0, cY, canW, cY, 1, gridColor);
			}
		}
	}

	static int GetNextFullNumber(int got, int step) {
		return got + step - (got % step);
	}

}