class LBAdminMenuFrame_Freecam : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_freecam";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/camera.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 20;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 700;
		height = 140;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.cheat.freecam"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_freecam";
	}

	TextWidget txt_speed;
	SliderWidget slider_freecam_turbo, slider_freecam_speed, slider_freecam_move_smooth, slider_freecam_mouse_smooth, slider_freecam_mouse_acc, slider_freecam_fov;
	EditBoxWidget edit_freecam_turbo, edit_freecam_speed, edit_freecam_move_smooth, edit_freecam_mouse_smooth, edit_freecam_mouse_acc, edit_freecam_fov;
	CheckBoxWidget chckbx_enabled, chckbx_freecam_underground, chckbx_reset_fov_enter, chckbx_reset_fov_end, chckbx_freeze, chckbx_full_map;
	ButtonWidget btn_reset_freecam_turbo, btn_reset_freecam_speed, btn_reset_move_smooth, btn_reset_mouse_smooth, btn_reset_mouse_acc, btn_reset_freecam_fov;

	string ending = "";

	override void Show(bool show) {
		super.Show(show);
		if (show) {
			LoadConfig();
		}
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (IsVisible() && !parent.IsVisible())
			LoadConfig();
		float speed = Math.Pow(slider_freecam_speed.GetCurrent(), 5);
		txt_speed.SetText("" + speed + " m/s");
	}

	void LoadConfig() {
		slider_freecam_turbo.SetCurrent(LBAdminMenuSettings.Get.freecamTurbo);
		edit_freecam_turbo.SetText("" + LBAdminMenuSettings.Get.freecamTurbo + "x");
		slider_freecam_move_smooth.SetCurrent(LBAdminMenuSettings.Get.freecamMovementSmooth);
		edit_freecam_move_smooth.SetText("" + LBAdminMenuSettings.Get.freecamMovementSmooth + "%");
		slider_freecam_mouse_smooth.SetCurrent(LBAdminMenuSettings.Get.freecamMouseSmooth);
		edit_freecam_mouse_smooth.SetText("" + LBAdminMenuSettings.Get.freecamMouseSmooth + "%");
		slider_freecam_mouse_acc.SetCurrent(LBAdminMenuSettings.Get.freecamMouseSensitivity);
		edit_freecam_mouse_acc.SetText("" + LBAdminMenuSettings.Get.freecamMouseSensitivity + "%");
		slider_freecam_fov.SetCurrent(LBAdminMenuSettings.Get.freecamFOV);
		edit_freecam_fov.SetText("" + LBAdminMenuSettings.Get.freecamFOV + "°");
		slider_freecam_speed.SetCurrent(LBAdminMenuSettings.Get.freecamSpeed);
		edit_freecam_speed.SetText("" + LBAdminMenuSettings.Get.freecamSpeed + " ");

		chckbx_freecam_underground.SetChecked(LBAdminMenuSettings.Get.freecamAllowUnderground);
		chckbx_reset_fov_enter.SetChecked(LBAdminMenuSettings.Get.freecamResetFOVOnEnter);
		chckbx_reset_fov_end.SetChecked(LBAdminMenuSettings.Get.freecamResetFOVOnLettingGo);
		chckbx_enabled.SetChecked(LBFreecamCamera.instance != null);
		chckbx_full_map.Show(LBAdmins.Get().HasPermission("admin.cheat.freecam_full_map"));
		chckbx_full_map.Enable(chckbx_enabled.IsChecked());
		if (LBFreecamCamera.instance) {
			chckbx_freeze.SetChecked(LBFreecamCamera.instance.frozen);
			chckbx_full_map.SetChecked(LBFreecamCamera.instance.fullMap);
		} else {
			chckbx_freeze.SetChecked(false);
			chckbx_full_map.SetChecked(false);
		}
		SetFocus(null);
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		bool save = true;
		int index, index2;
		if (w == chckbx_freecam_underground) {
			LBAdminMenuSettings.Get.freecamAllowUnderground = chckbx_freecam_underground.IsChecked();
		} else if (w == chckbx_reset_fov_enter) {
			LBAdminMenuSettings.Get.freecamResetFOVOnEnter = chckbx_reset_fov_enter.IsChecked();
		} else if (w == chckbx_reset_fov_end) {
			LBAdminMenuSettings.Get.freecamResetFOVOnLettingGo = chckbx_reset_fov_end.IsChecked();
		} else if (w == btn_reset_mouse_acc) {
			slider_freecam_mouse_acc.SetCurrent(15.0);
			OnChange(slider_freecam_mouse_acc, 0, 0, false);
		} else if (w == btn_reset_move_smooth) {
			slider_freecam_move_smooth.SetCurrent(0.0);
			OnChange(slider_freecam_move_smooth, 0, 0, false);
		} else if (w == btn_reset_freecam_fov) {
			slider_freecam_fov.SetCurrent(55.0);
			OnChange(slider_freecam_fov, 0, 0, false);
		} else if (w == btn_reset_mouse_smooth) {
			slider_freecam_mouse_smooth.SetCurrent(0.0);
			OnChange(slider_freecam_mouse_smooth, 0, 0, false);
		} else if (w == btn_reset_freecam_turbo) {
			slider_freecam_turbo.SetCurrent(4.0);
			OnChange(slider_freecam_turbo, 0, 0, false);
		} else if (w == btn_reset_freecam_speed) {
			slider_freecam_speed.SetCurrent(3.0);
			OnChange(slider_freecam_speed, 0, 0, false);
		} else if (w == chckbx_enabled) {
			MissionGameplay mg = MissionGameplay.Cast(g_Game.GetMission());
			mg.LBToggleFreecam();
			chckbx_full_map.Enable(chckbx_enabled.IsChecked());
		} else if (w == chckbx_freeze) {
			if (LBFreecamCamera.IsActive() && LBFreecamCamera.instance != null) {
				LBFreecamCamera.instance.SetFrozen(chckbx_freeze.IsChecked());
			} else {
				chckbx_freeze.SetChecked(false);
			}
		} else if (w == chckbx_full_map) {
			if (chckbx_full_map.IsChecked()) {
				chckbx_full_map.SetChecked(false);
				LBWarningPopup.Get().Show("Enable", "Full Map Freecam will delete your character and you can only play with a temporary character after disabling it and you might have to relog!", ScriptCaller.Create(EnableFullMapFreecam));
			} else {
				mg = MissionGameplay.Cast(g_Game.GetMission());
				mg.LBToggleFreecam();
			}
		} else {
			save = false;
		}
		if (save)
			LBAdminMenuSettings.Loader.Save();
		return save;
	}

	void EnableFullMapFreecam() {
		MissionGameplay mg = MissionGameplay.Cast(g_Game.GetMission());
		mg.LBToggleFreecam();

		DayZPlayerCameraSpectator.nextFullMap = true;
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.FREECAM_FULL_MAP, new Param2<bool, vector>(true, g_Game.GetCurrentCameraPosition()), true);
		chckbx_full_map.SetChecked(true);
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == slider_freecam_turbo) {
			LBAdminMenuSettings.Get.freecamTurbo = slider_freecam_turbo.GetCurrent();
			edit_freecam_turbo.SetText("" + LBAdminMenuSettings.Get.freecamTurbo + "x");
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == slider_freecam_move_smooth) {
			LBAdminMenuSettings.Get.freecamMovementSmooth = slider_freecam_move_smooth.GetCurrent();
			edit_freecam_move_smooth.SetText("" + LBAdminMenuSettings.Get.freecamMovementSmooth + "%");
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == slider_freecam_mouse_smooth) {
			LBAdminMenuSettings.Get.freecamMouseSmooth = slider_freecam_mouse_smooth.GetCurrent();
			edit_freecam_mouse_smooth.SetText("" + LBAdminMenuSettings.Get.freecamMouseSmooth + "%");
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == slider_freecam_mouse_acc) {
			LBAdminMenuSettings.Get.freecamMouseSensitivity = slider_freecam_mouse_acc.GetCurrent();
			edit_freecam_mouse_acc.SetText("" + LBAdminMenuSettings.Get.freecamMouseSensitivity + "%");
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == slider_freecam_fov) {
			LBAdminMenuSettings.Get.freecamFOV = slider_freecam_fov.GetCurrent();
			edit_freecam_fov.SetText("" + LBAdminMenuSettings.Get.freecamFOV + "°");
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == slider_freecam_speed) {
			LBAdminMenuSettings.Get.freecamSpeed = slider_freecam_speed.GetCurrent();
			edit_freecam_speed.SetText("" + LBAdminMenuSettings.Get.freecamSpeed + " ");
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == edit_freecam_fov) {
			float value = edit_freecam_fov.GetText().ToFloat();
			slider_freecam_fov.SetCurrent(Math.Clamp(value, slider_freecam_fov.GetMin(), slider_freecam_fov.GetMax()));
			LBAdminMenuSettings.Get.freecamFOV = slider_freecam_fov.GetCurrent();
			LBAdminMenuSettings.Loader.Save();
		} else if (w == edit_freecam_turbo) {
			value = edit_freecam_turbo.GetText().ToFloat();
			slider_freecam_turbo.SetCurrent(Math.Clamp(value, slider_freecam_turbo.GetMin(), slider_freecam_turbo.GetMax()));
			LBAdminMenuSettings.Get.freecamTurbo = slider_freecam_turbo.GetCurrent();
			LBAdminMenuSettings.Loader.Save();
		} else if (w == edit_freecam_mouse_acc) {
			value = edit_freecam_mouse_acc.GetText().ToFloat();
			slider_freecam_mouse_acc.SetCurrent(Math.Clamp(value, slider_freecam_mouse_acc.GetMin(), slider_freecam_mouse_acc.GetMax()));
			LBAdminMenuSettings.Get.freecamMouseSensitivity = slider_freecam_mouse_acc.GetCurrent();
			LBAdminMenuSettings.Loader.Save();
		} else if (w == edit_freecam_move_smooth) {
			value = edit_freecam_move_smooth.GetText().ToFloat();
			slider_freecam_move_smooth.SetCurrent(Math.Clamp(value, slider_freecam_move_smooth.GetMin(), slider_freecam_move_smooth.GetMax()));
			LBAdminMenuSettings.Get.freecamMovementSmooth = slider_freecam_move_smooth.GetCurrent();
			LBAdminMenuSettings.Loader.Save();
		} else if (w == edit_freecam_mouse_smooth) {
			value = edit_freecam_mouse_smooth.GetText().ToFloat();
			slider_freecam_mouse_smooth.SetCurrent(Math.Clamp(value, slider_freecam_mouse_smooth.GetMin(), slider_freecam_mouse_smooth.GetMax()));
			LBAdminMenuSettings.Get.freecamMouseSmooth = slider_freecam_mouse_smooth.GetCurrent();
			LBAdminMenuSettings.Loader.Save();
		} else if (w == edit_freecam_speed) {
			value = edit_freecam_speed.GetText().ToFloat();
			slider_freecam_speed.SetCurrent(Math.Clamp(value, slider_freecam_speed.GetMin(), slider_freecam_speed.GetMax()));
			LBAdminMenuSettings.Get.freecamMouseSmooth = slider_freecam_speed.GetCurrent();
			LBAdminMenuSettings.Loader.Save();
		}
		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (super.OnMouseEnter(w, x, y))
			return true;
		if (w == edit_freecam_speed || w == edit_freecam_fov || w == edit_freecam_mouse_acc || w == edit_freecam_mouse_smooth || w == edit_freecam_move_smooth || w == edit_freecam_turbo) {
			EditBoxWidget edit = EditBoxWidget.Cast(w);
			string text = edit.GetText();
			if (text.Length() > 0) {
				ending = text.SubstringUtf8(text.LengthUtf8() - 1, 1);
				text = text.SubstringUtf8(0, text.LengthUtf8() - 1);
				edit.SetText(text);
			}
			return true;
		}
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y) {
		if (super.OnMouseLeave(w, enterW, x, y))
			return true;
		if (w == edit_freecam_speed || w == edit_freecam_fov || w == edit_freecam_mouse_acc || w == edit_freecam_mouse_smooth || w == edit_freecam_move_smooth || w == edit_freecam_turbo) {
			EditBoxWidget edit = EditBoxWidget.Cast(w);
			edit.SetText(edit.GetText() + ending);
			return true;
		}
		return false;
	}

}