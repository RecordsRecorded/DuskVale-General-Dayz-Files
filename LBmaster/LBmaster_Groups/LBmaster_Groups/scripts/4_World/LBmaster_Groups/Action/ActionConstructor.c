modded class ActionConstructor {

	override void RegisterActions(TTypenameArray actions) {
		super.RegisterActions(actions);
		actions.Insert(ActionInvitePlayerToGroup);
#ifdef LBmaster_GroupDLCPlotpole
		actions.Insert(ActionClaimFlag);
		actions.Insert(ActionLBCheckFlagOwner);
#endif
	}

}
