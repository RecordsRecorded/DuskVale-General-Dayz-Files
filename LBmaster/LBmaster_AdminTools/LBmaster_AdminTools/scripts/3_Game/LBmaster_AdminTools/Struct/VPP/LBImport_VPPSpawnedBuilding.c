class LBImport_VPPSpawnedBuilding {

	string name;
	string itemname;
	vector position;
	vector direction;
	vector orientation;
	bool active;
	[NonSerialized()]
	Object building;
	string networkID;

	LBObjectBuilderObject ToLBFormat() {
		return new LBObjectBuilderObject(itemname, position, orientation, 1.0, active);
	}

}