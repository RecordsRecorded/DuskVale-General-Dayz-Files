#ifdef DZ_Expansion_Quests
modded class ExpansionQuestModule {

	override bool HasQuestMarker(int questID, int objectiveIndex) {
		return LBStaticMarkerManagerClient.Get().FindMarkerByUID(GetQuestMarkerIdLB(questID, objectiveIndex)) != null;
	}

	override protected void CreateMarkerClient(vector pos, string text, int questID, bool is3D, int objectiveIndex, int visibility) {
		LBServerMarker marker = new LBServerMarker();
		ExpansionIcon icon = ExpansionIcons.Get("Questionmark");
		string markerIcon = "Questionmark";
		if (icon)
			markerIcon = icon.IconPath;
		marker.SetupMarker(LBMarkerType.SERVER_DYNAMIC, text, markerIcon, pos + "0 2 0");
		marker.uid = GetQuestMarkerIdLB(questID, objectiveIndex);
		marker.display3d = is3D;
		marker.SetColorInt(ARGB(255, 241, 196, 15));
		//if ((visibility & EXPANSION_MARKER_VIS_MAP) == 0)
		//	marker.displayMap = false;
		LBStaticMarkerManagerClient.Get().AddNewMarker(marker);
	}

	override protected void RemoveMarkersClient(int questID, int objectiveIndex) {
		if (objectiveIndex == -2) {
			for (int i = -1; i < 20; ++i)
				LBStaticMarkerManagerClient.Get().DeleteMarker(GetQuestMarkerIdLB(questID, i));
		} else {
			LBStaticMarkerManagerClient.Get().DeleteMarker(GetQuestMarkerIdLB(questID, objectiveIndex));
		}
	}

	int GetQuestMarkerIdLB(int questId, int objectiveIndex) {
		return 700000 + questId * 100 + objectiveIndex;
	}
}
#endif