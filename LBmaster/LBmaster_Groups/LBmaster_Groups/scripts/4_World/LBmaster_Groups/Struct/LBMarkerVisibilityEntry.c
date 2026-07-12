class LBMarkerVisibilityEntry {

	int uid;
	int displaystate; // 0 3D+2D // 1 2D // 2 None

	int GetNextState() {
		displaystate++;
		displaystate = displaystate % 3;
		LBMarkerVisibilityManager.Loader.Save();
		return displaystate;
	}

}