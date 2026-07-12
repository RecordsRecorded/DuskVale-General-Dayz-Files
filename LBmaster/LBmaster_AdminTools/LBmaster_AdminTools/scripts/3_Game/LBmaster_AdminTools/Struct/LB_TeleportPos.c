class LB_TeleportPos {

	string name; // Name of the position
	vector pos; // x,y,z of the position
	vector dir; // Direction the player will look to when teleported
	string addedBy; // Steamid of the admin, who added the position

	static LB_TeleportPos Init(string name_, vector pos_, vector dir_) {
		LB_TeleportPos n = new LB_TeleportPos();
		n.pos = pos_;
		n.dir = dir_;
		n.name = name_;
		return n;
	}

	LB_TeleportPos Copy() {
		return LB_TeleportPos.Init(this.name, this.pos, this.dir);
	}

}