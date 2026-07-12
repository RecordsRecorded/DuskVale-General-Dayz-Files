class LBAdminMenuFrame_ESPSettings : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_esp_tools";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/esp_goggles.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 60;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 600;
		height = 200;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.cheat.esp"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_esp_tools";
	}

	WrapSpacerWidget list_filters;
	TextWidget txt_distance;
	SliderWidget slider_distance;
	XComboBoxWidget combo_distance_type, combo_min_health, combo_max_health;
	CheckBoxWidget chckbx_enable, chckbx_classnames, chckbx_skeleton;
	Widget panel_min_color, panel_max_color;

	ref array<ref Param2<CheckBoxWidget, LBESPFilter>> filterParams = new array<ref Param2<CheckBoxWidget, LBESPFilter >> ();

	override bool SetPrio() {
		LBWidgetUtils.DeleteAllChildren(list_filters);
		filterParams.Clear();
		foreach (LBESPFilter filter : LBAdminMenuSettings.Get.espFilters) {
			CheckBoxWidget chckbx = CheckBoxWidget.Cast(LBLayoutManager.Get().CreateLayout("SimpleCheckbox", list_filters));
			TextWidget txt = TextWidget.Cast(chckbx.FindAnyWidget("temp_text"));
			txt.SetText(filter.name);
			chckbx.SetChecked(filter.active);
			filterParams.Insert(new Param2<CheckBoxWidget, LBESPFilter>(chckbx, filter));
		}
		combo_distance_type.SetCurrentItem(LBAdminMenuSettings.Get.lastEspDistanceMode);
		LBESPManager.distanceMode = combo_distance_type.GetCurrentItem();

		int lastDistance = LBAdminMenuSettings.Get.lastEspDistance;
		slider_distance.SetCurrent(lastDistance);
		LBESPManager.SetMaxDistance(slider_distance.GetCurrent());
		SetLastDistanceText();
		chckbx_classnames.SetChecked(LBAdminMenuSettings.Get.espShowClassnames);
		chckbx_enable.SetChecked(LBESPManager.enabled);

		combo_min_health.SetCurrentItem(LBAdminMenuSettings.Get.espMinHealth);
		combo_max_health.SetCurrentItem(LBAdminMenuSettings.Get.espMaxHealth);

		UpdateHealthBoxesColor();

		return super.SetPrio();
	}

	void UpdateHealthBoxesColor() {
		panel_min_color.SetColor(LBAdminMenuFrame_ItemSpawner.GetHealthStateColor(combo_min_health.GetCurrentItem()) | 0xff000000);
		panel_max_color.SetColor(LBAdminMenuFrame_ItemSpawner.GetHealthStateColor(combo_max_health.GetCurrentItem()) | 0xff000000);
		LBAdminMenuSettings.Get.espMinHealth = combo_min_health.GetCurrentItem();
		LBAdminMenuSettings.Get.espMaxHealth = combo_max_health.GetCurrentItem();
		LBAdminMenuSettings.Loader.Save();
	}

	void SetLastDistanceText() {
		if (LBAdminMenuSettings.Get.lastEspDistance < 0)
			txt_distance.SetText("#lb_adm_unlimited");
		else
			txt_distance.SetText("" + LBAdminMenuSettings.Get.lastEspDistance + "m");
	}

	override void UpdateSlow() {
		chckbx_enable.SetChecked(LBESPManager.enabled);
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == chckbx_enable) {
			MissionGameplay mg = MissionGameplay.Cast(g_Game.GetMission());
			mg.LBSetModifierEnabled("#lb_adm_esp", chckbx_enable.IsChecked(), 0);
			LBESPManager.useClassnames = chckbx_classnames.IsChecked();
			LBESPManager.SetSkeletonEnabled(chckbx_skeleton.IsChecked());
			if (chckbx_enable.IsChecked()) {
				LBESPManager.EnableESP();
			} else {
				LBESPManager.DisableESP();
			}
			LBESPManager.SetMaxDistance(slider_distance.GetCurrent());
			LBESPManager.distanceMode = combo_distance_type.GetCurrentItem();
			LBAdminMenuSettings.Get.lastEspDistanceMode = LBESPManager.distanceMode;
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == combo_distance_type) {
			LBESPManager.distanceMode = combo_distance_type.GetCurrentItem();
			LBAdminMenuSettings.Get.lastEspDistanceMode = LBESPManager.distanceMode;
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (CheckFilters(w)) {
			return true;
		} else if (w == chckbx_classnames) {
			LBESPManager.useClassnames = chckbx_classnames.IsChecked();
			LBESPManager.UpdateNames();
			LBAdminMenuSettings.Get.espShowClassnames = chckbx_classnames.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == chckbx_skeleton) {
			LBESPManager.SetSkeletonEnabled(chckbx_skeleton.IsChecked());
			return true;
		} else if (w == combo_min_health || w == combo_max_health) {
			UpdateHealthBoxesColor();
			LBESPManager.UpdateFilter();
			return true;
		}
		return false;
	}

	bool CheckFilters(Widget w) {
		foreach (Param2<CheckBoxWidget, LBESPFilter> param : filterParams) {
			if (param.param1 == w && param.param2) {
				param.param2.active = !param.param2.active;
				LBESPManager.UpdateFilter();
				LBAdminMenuSettings.Loader.Save();
				return true;
			}
		}
		return false;
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == slider_distance) {
			LBAdminMenuSettings.Get.lastEspDistance = slider_distance.GetCurrent();
			LBAdminMenuSettings.Loader.Save();
			LBESPManager.SetMaxDistance(slider_distance.GetCurrent());
			SetLastDistanceText();
		}
		return false;
	}
}