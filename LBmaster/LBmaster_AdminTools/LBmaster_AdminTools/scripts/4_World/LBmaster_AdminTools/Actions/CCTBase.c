modded class CCTBase {

	float originalDistance = -1;

	void LBOverwriteDistance() {
		if (originalDistance == -1) {
			if (EnScript.GetClassVar(this, "m_MaximalActionDistanceSq", 0, originalDistance) || originalDistance != -1) {
				float overwrite = 100000.0 * 100000.0;
				EnScript.SetClassVar(this, "m_MaximalActionDistanceSq", 0, overwrite);
			}
		}
	}

	void LBResetOverwriteDistance() {
		if (originalDistance != -1) {
			EnScript.SetClassVar(this, "m_MaximalActionDistanceSq", 0, originalDistance);
			originalDistance = -1;
		}
	}

}