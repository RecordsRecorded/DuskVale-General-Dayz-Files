class LBAdminSpawnRequest {

	int type;
	Object target;
	string itemname;
	vector position;
	float quantity;
	string lastSpawn;
	int foodstage;
	int liquidType;
	float health;
	bool spawnDebug;
	bool spawnCE;
	int temperature;
	ref array<PlayerIdentity> selectedPlayers;

	void Load(int type_, Object target_, string itemname_, vector position_, float quantity_, string lastSpawn_, int foodstage_, int liquidType_, float health_, bool spawnDebug_, bool spawnCE_, bool temp_, array<PlayerIdentity> selectedPlayers_) {
		this.type = type_;
		this.target = target_;
		this.itemname = itemname_;
		this.position = position_;
		this.quantity = quantity_;
		this.lastSpawn = lastSpawn_;
		this.foodstage = foodstage_;
		this.liquidType = liquidType_;
		this.health = health_;
		this.spawnDebug = spawnDebug_;
		this.spawnCE = spawnCE_;
		this.temperature = temp_;
		this.selectedPlayers = selectedPlayers_;
	}

}