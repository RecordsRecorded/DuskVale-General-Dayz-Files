modded class ClosableContainer {
	override bool IsDisplayable() {
		return LBFreecamCamera.instance != null || super.IsDisplayable();
	}

	override void Open() {
		if (LBFreecamCamera.instance != null)
			m_LockCargo = false;
		super.Open();
	}
}