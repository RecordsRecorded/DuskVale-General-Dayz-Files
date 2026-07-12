#ifdef LBmaster_GroupDLCPlotpole
class ActionClaimFlag : ActionSingleUseBase {

	void ActionClaimFlag() {
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override void CreateConditionComponents() {
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override string GetText() {
		return "#lb_action_claim_flag";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		TerritoryFlag totem = TerritoryFlag.Cast(target.GetObject());
		if (!totem || !LBTerritoryConfig.Get.enablePlotpoleExtension || !player.GetLBGroup())
			return false;
		if (totem.ownerGroupTagHash == 0)
			return true;
		if (totem.GetAnimationPhase("flag_mast") >= 1.0 && LBTerritoryConfig.Get.treatFullyLoweredPlotpolesLikeUnclaimedPlotpoles && totem.ownerGroupTagHash != player.GetLBGroup().GetTagHash())
			return true;
		return false;
	}

	override bool RemoveForceTargetAfterUse() {
		return false;
	}

	override typename GetInputType() {
		return InteractActionInput;
	}

}
#endif