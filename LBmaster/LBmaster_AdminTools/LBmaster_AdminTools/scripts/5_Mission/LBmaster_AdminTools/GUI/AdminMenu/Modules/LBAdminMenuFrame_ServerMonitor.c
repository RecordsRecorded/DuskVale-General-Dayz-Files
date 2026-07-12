class LBAdminMenuFrame_ServerMonitor : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_server_info";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/server_monitor.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 20;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 500;
		height = 400;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.read.serverinfo"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_server_info";
	}

	CanvasWidget canvas_fps;
	TextWidget txt_player_count, txt_animal_count, txt_zombie_count, txt_vehicle_count, txt_item_count, txt_fps, txt_uptime;
	ButtonWidget btn_restart, btn_lock;

	bool locked = false;

	override void UpdateSlow() {
		if (IsVisible())
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.SERVER_INFO, new Param1<bool>(true), true);
	}

	override void Show(bool show) {
		super.Show(show);
		btn_restart.Show(LBAdmins.Get().HasPermission("admin.restart.server"));
		btn_lock.Show(LBAdmins.Get().HasPermission("admin.lock.server"));
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBAdminToolRPCs.SERVER_INFO) {
			int animalCount, zombieCount, playerCount, vehicleCount, itemCount, fps, startupTime;
			if (!ctx.Read(animalCount) || !ctx.Read(zombieCount) || !ctx.Read(playerCount) || !ctx.Read(vehicleCount) || !ctx.Read(itemCount) || !ctx.Read(fps) || !ctx.Read(startupTime) || !ctx.Read(locked))
				return;
			if (locked)
				btn_lock.SetText("#lb_adm_unlock");
			else
				btn_lock.SetText("#lb_adm_lock_server");
			txt_animal_count.SetText("" + animalCount);
			txt_zombie_count.SetText("" + zombieCount);
			txt_player_count.SetText("" + LBPlayerUtils.GetOnlinePlayerCount() + " (" + playerCount + " bodies)");
			txt_vehicle_count.SetText("" + vehicleCount);
			txt_item_count.SetText("" + itemCount);
			txt_fps.SetText("" + fps);
			int now = LBDate.Init(true).GetTimestamp();
			txt_uptime.SetText(LBDate.Init(now - startupTime).ToDiffString());
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_restart) {
			LBSimpleInputPopup.Get().Show("#lb_adm_restart", "#lb_adm_restart_server_in__seconds", "10", ScriptCaller.Create(ConfirmRestart));
			return true;
		} else if (w == btn_lock) {
			if (!locked)
				LBSimpleInputPopup.Get().Show("#lb_adm_lock", "#lb_adm_enter_a_lock_message", "Server is under maintenance", ScriptCaller.Create(ConfirmLock));
			else
				ConfirmUnlock();
		}
		return false;
	}
	void ConfirmLock(string message) {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.SERVER_LOCK, new Param2<bool, string>(true, message), true);
	}

	void ConfirmUnlock() {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.SERVER_LOCK, new Param2<bool, string>(false, ""), true);
	}

	void ConfirmRestart(string text) {
		int time = text.ToInt();
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.SERVER_RESTART, new Param1<int>(time), true);
	}

}