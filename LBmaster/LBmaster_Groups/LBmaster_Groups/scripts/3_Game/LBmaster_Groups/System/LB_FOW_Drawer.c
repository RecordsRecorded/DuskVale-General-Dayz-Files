class LB_FOW_Drawer {

	MapWidget map_widget;
	CanvasWidget drawCanvas;
	vector sub;
	float canvasWidth, canvasHeight;
	static ref array<ref LBFOWSector> blockedSectors = new array<ref LBFOWSector>();
	ref LBMapGridInfo gridInfo;

	void LB_FOW_Drawer(CanvasWidget canvas, MapWidget map_) {
		drawCanvas = canvas;
		map_widget = map_;
		UpdateSizes();
		gridInfo = new LBMapGridInfo();
	}

	void UpdateSizes() {
		float x, y;
		drawCanvas.GetScreenPos(x, y);
		drawCanvas.GetScreenSize(canvasWidth, canvasHeight);
		sub = Vector(x, y, 0);
	}

	void CalculateBlockedSectors() {
		blockedSectors.Clear();
		int currentIndex = 0, currentMask = 1;
		array<int> sectorInfo = LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO.fowSectorInfo;
		LBFOWSector sector;
		int sectorCount = LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO.sectorCount;
		for (int y = 0; y < sectorCount; ++y) {
			int start = 0;
			for (int x = 0; x < sectorCount; ++x) {
				int info = sectorInfo[currentIndex] & currentMask;
				NextIndex(currentIndex, currentMask);

				if (info != 0) { // Is revealed
					if (start != x) { // We had blocked sectors before
						sector = new LBFOWSector(start, y, x - 1, y);
						blockedSectors.Insert(sector);
					}
					start = x + 1;
					continue;
				}
			}
			if (start != sectorCount) {
				sector = new LBFOWSector(start, y, sectorCount - 1, y);
				blockedSectors.Insert(sector);
			}
		}
		int skip = 1;
		for (int i = 0; i < blockedSectors.Count(); ++i) {
			LBFOWSector current = blockedSectors.Get(i);
			if (current.deleted)
				continue;
			LBFOWSector found = null;
			for (int a = i + skip; a < blockedSectors.Count(); ++a) {
				LBFOWSector check = blockedSectors.Get(a);
				if (check.y1 <= current.y2) {
					continue;
				} else if (check.y1 == current.y2 + 1) {
					if (check.x1 == current.x1 && check.x2 == current.x2) {
						found = check;
						break;
					}
				} else {
					break;
				}
			}
			if (found) {
				found.deleted = true;
				++skip;
				current.y2 = found.y2;
				--i;
			} else {
				skip = 1;
			}
		}
		int before = blockedSectors.Count();
		int back = 0;
		for (i = 0; i < blockedSectors.Count(); ++i) {
			current = blockedSectors.Get(i);
			if (current.deleted) {
				++back;
				continue;
			}
			if (back != 0)
				blockedSectors.Set(i - back, current);
		}
		blockedSectors.Resize(before - back);

		LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO.changed = false;
		LBLogger.Debug("Calculated " + before + " blocked Sectors row wise. Merged to a total of " + blockedSectors.Count() + " sectors", "AdvancedGroups");
	}

	static void NextIndex(inout int index, inout int mask) {
		mask = mask << 1;
		if (mask == 0) {
			mask = 1;
			++index;
		}
	}

	void DrawFogOfWar() {
		if (!drawCanvas || !LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO || !LBFOWConfig.Get || !LBFOWConfig.Get.enabled)
			return;
		if (LBAdmins.Get().HasPermissionActive("groups.ignore.fow"))
			return;
		if (LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO.changed)
			CalculateBlockedSectors();
		float sectorSize = LBFOWPlayerInfo_.CLIENT_RECEIVED_INFO.sectorSize;
		int color = LBFOWConfig.Get.fogColor.GetColorARGB();
		foreach (LBFOWSector blocked : blockedSectors) {
			if (!blocked.deleted)
				FOWDrawRectFrom(blocked.GetFrom(sectorSize), blocked.GetTo(sectorSize), color);
		}
		gridInfo.DrawGridLines(map_widget, drawCanvas);
	}

	void FOWDrawRectFrom(vector from, vector to, int color) {
		vector start = map_widget.MapToScreen(from) - sub;
		vector end = map_widget.MapToScreen(to) - sub;
		if (start[0] > canvasWidth || end[1] > canvasHeight || end[0] < 0 || start[1] < 0) {
			return;
		}
		FOWDrawRect(start[0], start[1], end[0] - start[0], end[1] - start[1], color);
	}

	void FOWDrawRect(float x, float y, float w, float h, int color) {
		drawCanvas.DrawLine(x, y + h / 2, x + w, y + h / 2, h, color);
	}

}