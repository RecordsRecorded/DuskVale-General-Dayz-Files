class LBGlobalVisibilityEntry {

	LBMarkerType type;
	int displaystate; // 0 3D+2D // 1 2D // 2 None

	int GetNextState() {
		displaystate++;
		displaystate = displaystate % 3;
		return displaystate;
	}

}