class LBAdminHandcuffs : RestrainingToolLocked {

	override void SetActions() {
		super.SetActions();

		RemoveAction(ActionUnrestrainSelf);
	}

}