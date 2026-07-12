modded class PlayerSoundManagerClient {

	bool lbDisabled = false;

	override void Update() {
		if (!lbDisabled)
			super.Update();
	}

	void SetLBDisabled(bool dis) {
		lbDisabled = dis;
	}

}