class LBImport_VPPBuildingSet {

	string name;
	bool active;
	ref array<ref LBImport_VPPSpawnedBuilding> buildings;

	LBObjectBuilderSet ToLBFormat() {
		LBObjectBuilderSet set_ = new LBObjectBuilderSet(name, active);
		if (buildings) {
			foreach (LBImport_VPPSpawnedBuilding building : buildings) {
				if (building)
					set_.objects.Insert(building.ToLBFormat());
			}
		}
		return set_;
	}

}