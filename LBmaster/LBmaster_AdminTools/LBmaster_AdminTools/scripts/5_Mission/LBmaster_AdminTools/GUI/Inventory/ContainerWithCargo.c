modded class ContainerWithCargo {
	override bool IsDisplayable() {
		return LBFreecamCamera.instance != null || super.IsDisplayable();
	}

	override void Open() {
		if (LBFreecamCamera.instance != null)
			m_LockCargo = false;
		super.Open();
	}

	override void LockCargo(bool value) {
		if (LBFreecamCamera.instance) {
			super.LockCargo(false);
			return;
		}
		super.LockCargo(value);
	}

	override bool IsVisible() {
		if (LBFreecamCamera.instance) {
			OnShow();
		}
		return super.IsVisible();
	}
}