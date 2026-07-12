class LBAdminMenuFrame_PlayerCheats : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_cheats";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/cheat.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 250;
		height = 100;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 450;
		height = 200;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.cheat.fullbright", "admin.cheat.godmode", "admin.cheat.godmode_vehicle", "admin.cheat.pristinehands", "admin.cheat.tempplayer", "admin.cheat.unlimitedammo"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_cheats";
	}

	ref array<ref Param3<CheckBoxWidget, Widget, LBModifierInfo>> infos = new array<ref Param3<CheckBoxWidget, Widget, LBModifierInfo >> ();

	CheckBoxWidget chckbx_full_bright, chckbx_temp_player;
	ButtonWidget btn_setup_temp;
	XComboBoxWidget combo_load_settings;

	override void Init(LBAdminMenuMainButton parent_) {
		super.Init(parent_);
		foreach (LBModifierInfo info : LBModifierManager.Get().infos) {
			if (info.toggleOnCheatsPage && info.permOwn != "" && LBAdmins.Get().HasPermission(info.permOwn)) {
				CheckBoxWidget chckbx = CheckBoxWidget.Cast(LBLayoutManager.Get().CreateLayout("SimpleCheckbox", layoutRoot));
				TextWidget txt = TextWidget.Cast(chckbx.FindAnyWidget("temp_text"));
				txt.SetText(info.displayName);
				infos.Insert(new Param3<CheckBoxWidget, Widget, LBModifierInfo>(chckbx, txt, info));
			}
		}
		UpdateCheckboxes();
	}

	override string GetTooltip(Widget w) {
		if (w == chckbx_full_bright) {
			return "#lb_adm_cheat_full_bright_desc";
		} else if (w == chckbx_temp_player) {
			return "#lb_adm_cheat_temp_player_desc";
		} else if (w == combo_load_settings) {
			return "Here you can change if previous enabled modifiers should be loaded and also when. There are 5 different options to select here:\n- Never: Never restore the information. Always reset to none enabled\n- Relog: Restore it when logging back into the same server. Only saved for the last server, so joining another server will reset the last enabled modifiers\n- Game Crash: Restore when logging back into the same server within 5 minutes of disconnecting. Server restarts or connecting to another server will wipe this information\n- Joined: Every time you join a server. Doesn't matter which server\n- Admin Mode Enabled: Every time you enable admin mode on a server";
		}
		foreach (Param3<CheckBoxWidget, Widget, LBModifierInfo> param : infos) {
			if (param.param1 == w || param.param2 == w)
				return param.param3.description;
		}
		return super.GetTooltip(w);
	}

	override void Show(bool show) {
		super.Show(show);
		UpdateCheckboxes();
	}

	void UpdateCheckboxes() {
		chckbx_full_bright.Show(LBAdmins.Get().HasPermission("admin.cheat.fullbright"));
		chckbx_temp_player.Show(LBAdmins.Get().HasPermission("admin.cheat.tempplayer"));
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
			return;
		foreach (Param3<CheckBoxWidget, Widget, LBModifierInfo> param : infos) {
			param.param1.SetChecked(player.LBIsModifierActive(param.param3.id));
		}
		combo_load_settings.SetCurrentItem(LBLastCheatsConfig.Get.saveLevel);
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		foreach (Param3<CheckBoxWidget, Widget, LBModifierInfo> param : infos) {
			if (w == param.param1 || w == param.param2) {
				LBToggleModifier(param.param3.id);
				return true;
			}
		}
		if (w == combo_load_settings) {
			LBLastCheatsConfig.Get.saveLevel = combo_load_settings.GetCurrentItem();
			LBLastCheatsConfig.Loader.Save();
			return true;
		}
		CheckBoxWidget chckbx = CheckBoxWidget.Cast(w);
		if (chckbx) {
			if (chckbx.GetUserID() == LBAdminCheatList.CHEAT_TEMP_PLAYER) {
				if (chckbx.IsChecked()) {
					LBWarningPopup.Get().Show("Enable", "Are you sure you want to enable Temp Player?", ScriptCaller.Create(ConfirmToggleTemp), ScriptCaller.Create(CancelToggleTemp));
					return true;
				} else {
					parent.CloseMe();
				}
			}
			SendCheatRequest(chckbx.GetUserID(), chckbx.IsChecked());
		}
		return false;
	}

	void CancelToggleTemp() {
		chckbx_temp_player.SetChecked(false);
	}

	void ConfirmToggleTemp() {
		parent.CloseMe();
		SendCheatRequest(LBAdminCheatList.CHEAT_TEMP_PLAYER, true);
	}

	void LBToggleModifier(LBPlayerModifier mod) {
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
			return;
		bool active = player.LBIsModifierActive(mod);
		array<PlayerIdentity> nullList = null;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(nullList);
		rpc.Write(mod);
		bool enable = !active;
		rpc.Write(enable);
		rpc.Send(null, LBAdminToolRPCs.SET_MODIFIER_REMOTE, true);
	}
	
	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBAdminToolRPCs.SYNC_LOCAL_MODIFIERS) {
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(UpdateCheckboxes);
		}
		super.OnRPC(target, rpc_type, ctx);
	}

	static bool SendCheatRequest(int type, bool enabled) {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.TOGGLE_CHEAT, new Param2<int, bool>(type, enabled), true);
		return true;
	}
}