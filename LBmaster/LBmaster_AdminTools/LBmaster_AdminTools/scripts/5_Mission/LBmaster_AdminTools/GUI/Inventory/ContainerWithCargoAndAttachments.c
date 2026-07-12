modded class ContainerWithCargoAndAttachments {
	override bool IsDisplayable() {
		return LBFreecamCamera.instance != null || super.IsDisplayable();
	}

	override void Open() {
		if (LBFreecamCamera.instance != null)
			m_LockCargo = false;
		super.Open();
	}

	override void HideCargo() {
		if (LBFreecamCamera.instance)
			ShowCargo();
		else
			super.HideCargo();
	}
}
