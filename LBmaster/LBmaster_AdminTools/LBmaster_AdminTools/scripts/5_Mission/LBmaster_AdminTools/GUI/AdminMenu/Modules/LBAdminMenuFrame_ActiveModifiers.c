class LBAdminMenuFrame_ActiveModifiers : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_active_modifier";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/checklist.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 200;
		height = 50;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 200;
		height = 300;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.view.active_modifier"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_active_modifier";
	}

	WrapSpacerWidget list;

	int lastActiveForPlayer = 0;

	override void OnShow() {
		super.OnShow();
		outline.Show(true);
		background.Show(true);
		MissionGameplay.lbEnabledModifierChanged = true;
	}

	override void OnHide() {
		super.OnHide();
		outline.Show(false);
		background.Show(false);
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (MissionGameplay.lbEnabledModifierChanged || PlayerModifierChanged())
			UpdateSlow();
	}

	bool PlayerModifierChanged() {
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		return player && player.LBGetActiveModifiers() != lastActiveForPlayer;
	}

	override void UpdateSlow() {
		if (!MissionGameplay.lbEnabledModifierChanged && !PlayerModifierChanged())
			return;
		MissionGameplay.lbEnabledModifierChanged = false;
		LBWidgetUtils.DeleteAllChildren(list);
		foreach (string modifier : MissionGameplay.lbEnabledModifier) {
			TextWidget txt = TextWidget.Cast(LBLayoutManager.Get().CreateLayout("AdminActiveModifierEntry", list));
			txt.SetText(modifier);
		}
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (player) {
			lastActiveForPlayer = player.LBGetActiveModifiers();
			LBLastCheatsConfig.Get.enabledModifiers = lastActiveForPlayer;
			foreach (LBModifierInfo info : LBModifierManager.Get().infos) {
				if (player.LBIsModifierActive(info.id)) {
					txt = TextWidget.Cast(LBLayoutManager.Get().CreateLayout("AdminActiveModifierEntry", list));
					txt.SetText(info.displayName);
				}
			}
			LBLastCheatsConfig.Loader.Save();
		}
	}

}