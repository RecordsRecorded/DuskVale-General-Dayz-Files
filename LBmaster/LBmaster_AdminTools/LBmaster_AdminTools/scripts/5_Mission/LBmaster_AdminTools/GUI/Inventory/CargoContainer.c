modded class CargoContainer {
	override bool IsDisplayable() {
		return LBFreecamCamera.instance != null || super.IsDisplayable();
	}
	override bool IsVisible() {
		return LBFreecamCamera.instance != null || super.IsVisible();
	}
}