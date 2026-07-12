class ActionBuildConstructionAdmin : ActionInteractBase {

	override void CreateConditionComponents() {
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	string presetName = "";

	override string GetText() {
		return LBTranslatedString("#lb_adm_build_action", {"%preset%", presetName}).Get();
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		if (!target || !target.GetObject())
			return false;
		if (!LBAdmins.Get().HasPermissionActive("admin.use.constructioneditor", player))
			return false;
#ifndef SERVER
		if (g_Game.GetUIManager() && g_Game.GetUIManager().GetMenu())
			return false;
		BaseBuildingBase targetItem = BaseBuildingBase.Cast(target.GetObject());
		if (!targetItem) {
			return false;
		}
		presetName = LBAdminMenuSettings.Get.GetFavouriteConstructionConfig(targetItem.GetType());
		return LBAdminMenuSettings.Get.IsContructionPresetExisting(targetItem.GetType(), presetName);
#endif
		return true;
	}

	override void OnExecuteClient(ActionData action_data) {
		BaseBuildingBase original = BaseBuildingBase.Cast(action_data.m_Target.GetObject());
		BaseBuildingBase itemCopy = BaseBuildingBase.Cast(g_Game.CreateObject(original.GetType(), vector.Zero, true));
		if (!itemCopy)
			return;
		TStringSet presetParts = LBAdminMenuSettings.Get.GetConstructedParts(itemCopy.GetType(), presetName);
		Construction constr = itemCopy.GetConstruction();
		map<string, ref ConstructionPart> parts = constr.GetConstructionParts();
		foreach (string part_name, ConstructionPart part2 : parts) {
			bool built = false;
			if (presetParts.Find(part_name) != -1) {
				built = true;
			}
			SetPartBuilt(itemCopy, part_name, built);
		}

		Param5<BaseBuildingBase, int, int, int, bool> param = new Param5<BaseBuildingBase, int, int, int, bool>(original, itemCopy.m_SyncParts01, itemCopy.m_SyncParts02, itemCopy.m_SyncParts03, itemCopy.m_HasBase);
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.BASEBUILDING_SET, param, true);
	}

	void SetPartBuilt(BaseBuildingBase itemCopy, string part_name, bool built) {
		if (!itemCopy)
			return;
		Construction constr = itemCopy.GetConstruction();
		ConstructionPart part = constr.GetConstructionPart(part_name);
		if (built) {
			itemCopy.RegisterPartForSync(part.GetId());
			constr.AddToConstructedParts(part_name);
			if (part.IsBase())
				itemCopy.m_HasBase = true;
		} else {
			itemCopy.UnregisterPartForSync(part.GetId());
			constr.RemoveFromConstructedParts(part_name);
			if (part.IsBase())
				itemCopy.m_HasBase = false;
		}
		itemCopy.UpdateVisuals();
	}

}