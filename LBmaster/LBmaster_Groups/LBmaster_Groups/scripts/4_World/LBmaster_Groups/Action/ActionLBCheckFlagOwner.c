#ifdef LBmaster_GroupDLCPlotpole
class ActionLBCheckFlagOwner : ActionSingleUseBase {

	void ActionLBCheckFlagOwner() {
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override void CreateConditionComponents() {
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override string GetText() {
		return "Check Owner";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		TerritoryFlag totem = TerritoryFlag.Cast(target.GetObject());
		if (!totem || !LBTerritoryConfig.Get.enablePlotpoleExtension)
			return false;
		return LBAdmins.Get().HasPermissionActive("groups.view.territoryflags", player);
	}

	override bool RemoveForceTargetAfterUse() {
		return false;
	}

	override typename GetInputType() {
		return InteractActionInput;
	}

}
#endif