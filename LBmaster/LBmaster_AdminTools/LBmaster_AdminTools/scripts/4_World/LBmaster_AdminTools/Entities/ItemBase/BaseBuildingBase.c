modded class BaseBuildingBase {

	void OnSynchronizedClientLB() {
		OnSynchronizedClient();
	}

	override void SetActions() {
		super.SetActions();
		AddAction(ActionBuildConstructionAdmin);
	}

}