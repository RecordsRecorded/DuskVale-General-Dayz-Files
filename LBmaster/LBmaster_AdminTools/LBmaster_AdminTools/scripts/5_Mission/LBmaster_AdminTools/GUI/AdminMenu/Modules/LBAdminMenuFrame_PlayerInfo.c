class LBAdminMenuFrame_PlayerInfo : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_player_info";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/playerinfo.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 60;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 750;
		height = 700;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.view.playerinfo"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_player_info";
	}

	ref LBPlayerInfoEntries infoEntries = new LBPlayerInfoEntries();
	PlayerIdentity selected;
	int nextRPC = 0;
	string hands;
	int activeAgents, activeModifiers;
	bool restrained = false, multiSelectActive = false;
	vector playerPos;
	string names, steamids, guids, gamelabsId;
	ref array<PlayerIdentity> tempList = new array<PlayerIdentity>();
	bool bloodyHands = false;

	WrapSpacerWidget panel_player_stats;
	TextWidget txt_name, txt_steamid, txt_guid, txt_cf_id, txt_hands, txt_pos, txt_state, txt_sickness, txt_group;
	ButtonWidget btn_copy_name, btn_copy_steamid, btn_browser_steamid, btn_copy_guid, btn_copy_cf_id, btn_browser_cf_id, btn_copy_hands, btn_copy_pos, btn_spectate;
	ButtonWidget btn_kill, btn_heal, btn_tp_to, btn_bring, btn_bring_back, btn_find_on_map, btn_strip, btn_clear, btn_toggle_restrain, btn_freeze, btn_invis;
	ButtonWidget btn_god, btn_god_veh, btn_ammo, btn_prist_hands, btn_stamina, btn_action_speed, btn_ignore_ai, btn_silent, btn_no_jam, btn_bloody, btn_vomit;
	ButtonWidget btn_scale, btn_kick, btn_ban, btn_message, btn_apply_all, btn_copy_group;
	TextListboxWidget list_sickness;
	Widget playerinfo, panel_cf_id, panel_group;

	override void OnInit() {
		LBAdminMenuFrame_PlayerList list = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList"));
		list.Event_Player_Selected.Insert(RequestPlayerInfoSelected);
		list.Event_Players_Selected.Insert(OnMultiSelect);
		typename tn = eAgents;
		int count = tn.GetVariableCount();
		list_sickness.ClearItems();
		for (int i = 0; i < count; ++i) {
			string name = tn.GetVariableName(i);
			name = LBStringTools.PrettyPrintEnumString(name);
			list_sickness.AddItem(name, null, 0);
		}
		CheckButtonPermissions();
	}

	void OnMultiSelect(array<PlayerIdentity> identities) {
		LBAdminMenuFrame_PlayerList list = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList"));
		if (identities.Count() == 0) {
			multiSelectActive = false;
			RequestPlayerInfoSelected(list.selectedPlayer);
		} else if (identities.Count() == 1) {
			multiSelectActive = false;
			if (list.selectedPlayer)
				RequestPlayerInfoSelected(list.selectedPlayer);
			else
				RequestPlayerInfoSelected(identities.Get(0));
		} else {
			multiSelectActive = true;
			activeModifiers = 0;
			names = identities.Get(0).GetName();
			steamids = identities.Get(0).GetPlainId();
			guids = identities.Get(0).GetId();
			for (int i = 1; i < identities.Count(); ++i) {
				PlayerIdentity player = identities.Get(i);
				names += ", " + player.GetName();
				steamids += ", " + player.GetPlainId();
				guids += ", " + player.GetId();
			}
			txt_name.SetText(names);
			txt_steamid.SetText(steamids);
			txt_guid.SetText(guids);
			txt_hands.SetText("");
			txt_pos.SetText("");
			txt_cf_id.SetText("");
			panel_group.Show(false);
			panel_player_stats.Show(false);
			txt_state.SetText("");
			CheckButtonsMultiPermissions();
			UpdateModifierButtons();
		}
	}

	void CheckButtonPermissions() {
		if (!selected || selected.GetPlainId() != LBAdmins.Get().GetMySteamid()) {
			CheckButtonPermissionsOther();
		} else {
			CheckButtonPermissionsSelf();
		}
	}

	void CheckButtonsMultiPermissions() {
		LBAdmins adm = LBAdmins.Get();
		btn_spectate.Show(false);
		btn_kill.Show(adm.HasPermission("admin.kill.players"));
		btn_heal.Show(adm.HasPermission("admin.heal.others"));
		btn_tp_to.Show(false);
		btn_bring.Show(adm.HasPermission("admin.bring.players"));
		btn_bring_back.Show(adm.HasPermission("admin.bring.players"));
		btn_kick.Show(adm.HasPermission("admin.kick.players"));
		btn_ban.Show(false);
		btn_find_on_map.Show(false);
		btn_strip.Show(adm.HasPermission("admin.strip.players"));
		btn_clear.Show(adm.HasPermission("admin.clear.players"));
		btn_toggle_restrain.Show(adm.HasPermission("admin.restrain.players"));
		btn_freeze.Show(adm.HasPermission("admin.set.freeze_multi"));
		btn_invis.Show(adm.HasPermission("admin.set.invisible_multi"));
		btn_god.Show(adm.HasPermission("admin.set.godmode_multi"));
		btn_god_veh.Show(adm.HasPermission("admin.set.vehicle_godmode_multi"));
		btn_ammo.Show(adm.HasPermission("admin.set.unlimited_ammo_multi"));
		btn_prist_hands.Show(adm.HasPermission("admin.set.pristine_hands_multi"));
		btn_stamina.Show(adm.HasPermission("admin.set.unlimited_stamina_multi"));
		btn_vomit.Show(adm.HasPermission("admin.vomit.players"));
		btn_action_speed.Show(adm.HasPermission("admin.set.action_speedhack_multi"));
		list_sickness.Show(false);
		txt_sickness.Show(false);
		btn_scale.Show(adm.HasPermission("admin.set.player_scales"));
		btn_ignore_ai.Show(adm.HasPermission("admin.set.ignore_ai_multi"));
		btn_bloody.Show(false);
		btn_message.Show(adm.HasPermission("admin.send.message_multi"));
	}

	void CheckButtonPermissionsOther() {
		LBAdmins adm = LBAdmins.Get();
		btn_spectate.Show(adm.HasPermission("admin.spectate.player"));
		btn_kill.Show(adm.HasPermission("admin.kill.player"));
		btn_heal.Show(adm.HasPermission("admin.heal.other"));
		btn_tp_to.Show(adm.HasPermission("admin.teleport.player"));
		btn_bring.Show(adm.HasPermission("admin.bring.player"));
		btn_bring_back.Show(adm.HasPermission("admin.bring.player"));
		btn_kick.Show(adm.HasPermission("admin.kick.player"));
		btn_ban.Show(adm.HasPermission("admin.ban.player"));
		btn_find_on_map.Show(adm.HasPermission("admin.view.playerpositions"));
		btn_strip.Show(adm.HasPermission("admin.strip.player"));
		btn_clear.Show(adm.HasPermission("admin.clear.player"));
		btn_toggle_restrain.Show(adm.HasPermission("admin.restrain.player"));
		btn_freeze.Show(adm.HasPermission("admin.set.freeze"));
		btn_invis.Show(adm.HasPermission("admin.set.invisible"));
		btn_god.Show(adm.HasPermission("admin.set.godmode"));
		btn_god_veh.Show(adm.HasPermission("admin.set.vehicle_godmode"));
		btn_ammo.Show(adm.HasPermission("admin.set.unlimited_ammo"));
		btn_prist_hands.Show(adm.HasPermission("admin.set.pristine_hands"));
		btn_stamina.Show(adm.HasPermission("admin.set.unlimited_stamina"));
		btn_vomit.Show(adm.HasPermission("admin.vomit.player"));
		btn_action_speed.Show(adm.HasPermission("admin.set.action_speedhack"));
		list_sickness.Show(adm.HasPermission("admin.view.sickness"));
		txt_sickness.Show(adm.HasPermission("admin.view.sickness"));
		btn_scale.Show(adm.HasPermission("admin.set.player_scale"));
		btn_ignore_ai.Show(adm.HasPermission("admin.set.ignore_ai"));
		btn_bloody.Show(adm.HasPermission("admin.change.bloodyhands"));
		btn_message.Show(adm.HasPermission("admin.send.message"));
	}

	void CheckButtonPermissionsSelf() {
		LBAdmins adm = LBAdmins.Get();
		btn_spectate.Show(adm.HasPermission("admin.spectate.player"));
		btn_kill.Show(adm.HasPermission("admin.kill.player"));
		btn_heal.Show(adm.HasPermission("admin.heal.self"));
		btn_tp_to.Show(false);
		btn_bring.Show(false);
		btn_bring_back.Show(adm.HasPermission("admin.teleport.back"));
		btn_kick.Show(false);
		btn_ban.Show(false);
		btn_find_on_map.Show(adm.HasPermission("admin.view.playerpositions"));
		btn_strip.Show(adm.HasPermission("admin.strip.player"));
		btn_clear.Show(adm.HasPermission("admin.clear.player"));
		btn_toggle_restrain.Show(adm.HasPermission("admin.restrain.player"));
		btn_freeze.Show(adm.HasPermission("admin.set.freeze"));
		btn_invis.Show(adm.HasPermission("admin.cheat.invisible"));
		btn_god.Show(adm.HasPermission("admin.cheat.godmode"));
		btn_god_veh.Show(adm.HasPermission("admin.cheat.godmode_vehicle"));
		btn_ammo.Show(adm.HasPermission("admin.cheat.unlimitedammo"));
		btn_prist_hands.Show(adm.HasPermission("admin.cheat.pristinehands"));
		btn_stamina.Show(adm.HasPermission("admin.cheat.unlimited_stamina"));
		btn_vomit.Show(adm.HasPermission("admin.vomit.player"));
		list_sickness.Show(adm.HasPermission("admin.view.sickness"));
		txt_sickness.Show(adm.HasPermission("admin.view.sickness"));
		btn_action_speed.Show(adm.HasPermission("admin.cheat.action_speedhack"));
		btn_scale.Show(adm.HasPermission("admin.set.player_scale"));
		btn_bloody.Show(adm.HasPermission("admin.change.bloodyhands"));
		btn_ignore_ai.Show(adm.HasPermission("admin.cheat.ignore_ai"));
		btn_message.Show(true);
	}

	override void UpdateSlow() {
		if (!multiSelectActive)
			RequestPlayerInfo(selected);
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBAdminToolRPCs.REQUEST_INFO) {
			int state = 0; // DEAD, UNCON, ALIVE
			int aliveSec;
			PlayerIdentity targetIdent;
			if (!ctx.Read(state) || !ctx.Read(aliveSec) || !ctx.Read(targetIdent) || !ctx.Read(hands)) {
				panel_player_stats.Show(false);
				txt_state.SetText("#lb_adm_dead");
				UpdateModifierButtons();
				return;
			}
			if (state == 0)
				txt_state.SetText("#lb_adm_dead");
			else if (state == 1)
				txt_state.SetText("#lb_adm_uncon");
			else if (state == 2) {
				string aliveTime = LBDate.Init(aliveSec).ToDiffString();
				txt_state.SetText(LBTranslatedString("#lb_adm_alive_for", {"%time%", aliveTime}).Get());
			}

			infoEntries.ReadPlayer(ctx, panel_player_stats, btn_apply_all, targetIdent);
			panel_player_stats.Show(true);
			if (hands == "")
				txt_hands.SetText("#lb_adm_empty");
			else
				txt_hands.SetText(hands + " (" + LBItemHelper.GetDisplayName(hands) + ")");
			ctx.Read(activeAgents);
			ctx.Read(activeModifiers);
			ctx.Read(restrained);
			ctx.Read(playerPos);
			if (LBAdmins.Get().HasPermission("admin.view.playerpositions")) {
				txt_pos.SetText(playerPos.ToString());
				txt_pos.Show(true);
				btn_copy_pos.Show(true);
			} else {
				txt_pos.Show(false);
				btn_copy_pos.Show(false);
			}
			ctx.Read(bloodyHands);
			ctx.Read(gamelabsId);
			if (gamelabsId != "") {
				panel_cf_id.Show(true);
				txt_cf_id.SetText(gamelabsId);
			} else {
				panel_cf_id.Show(false);
			}
#ifdef LBmaster_Groups
			string groupname, groupshortname;
			if (LBGroupInfoShare.GetPlayerGroup(targetIdent.GetPlainId(), groupname, groupshortname)) {
				txt_group.SetText("[" + groupshortname + "] " + groupname);
				panel_group.Show(true);
			} else {
				panel_group.Show(false);
			}
#endif
			UpdateSicknesses();
			UpdateModifierButtons();
		}
	}

	void UpdateSicknesses() {
		for (int i = 0; i < list_sickness.GetNumItems(); ++i) {
			int agentId = 1 << i;
			if (activeAgents & agentId) {
				list_sickness.SetItemColor(i, 0, ARGB(255, 255, 0, 0));
			} else {
				list_sickness.SetItemColor(i, 0, ARGB(255, 0, 255, 0));
			}
		}
	}

	void UpdateModifierButtons() {
		foreach (LBModifierInfo info : LBModifierManager.Get().infos) {
			SetModifierText(info.id, ButtonWidget.Cast(layoutRoot.FindAnyWidget(info.layout_button_name)), info.txt_enabled, info.txt_disabled);
		}
		SetButtonText(IsSelectedRestrained(), btn_toggle_restrain, "#lb_adm_unrestrain", "#lb_adm_restrain");
		SetButtonText(bloodyHands, btn_bloody, "#lb_adm_clean_hands", "#lb_adm_bloody_hands");
		SetButtonText(selected && selected.GetPlainId() == LBAdmins.Get().GetMySteamid(), btn_spectate, "#lb_adm_spectate_stop", "#lb_adm_spectate");
	}

	void SetModifierText(LBPlayerModifier mod, ButtonWidget btn, string enabled, string disabled) {
		if (!btn)
			return;
		SetButtonText(activeModifiers & mod, btn, enabled, disabled);
	}

	void SetButtonText(bool enabled, ButtonWidget btn, string enabledTxt, string disabledTxt) {
		if (enabled) {
			btn.SetText(enabledTxt);
			if (btn.GetChildren())
				btn.GetChildren().SetColor(ARGB(255, 0, 255, 0));
		} else {
			btn.SetText(disabledTxt);
			if (btn.GetChildren())
				btn.GetChildren().SetColor(ARGB(255, 255, 0, 0));
		}
	}

	void RequestPlayerInfoSelected(PlayerIdentity target) {
		infoEntries.ResetApply();
		RequestPlayerInfo(target);
	}

	void RequestPlayerInfo(PlayerIdentity target) {
		selected = target;
		CheckButtonPermissions();
		if (!target) {
			txt_name.SetText("");
			txt_steamid.SetText("");
			txt_guid.SetText("");
			activeModifiers = 0;
			UpdateModifierButtons();
			activeAgents = 0;
			UpdateSicknesses();
			panel_group.Show(false);
			txt_pos.Show(false);
			btn_copy_pos.Show(false);
			return;
		}
		txt_name.SetText(target.GetName());
		txt_steamid.SetText(target.GetPlainId());
		txt_guid.SetText(target.GetId());

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(target);
		rpc.Send(null, LBAdminToolRPCs.REQUEST_INFO, true);
	}

	bool IsModifierActive(LBPlayerModifier mod) {
		return (activeModifiers & mod) != 0;
	}

	bool ToggleModifier(LBPlayerModifier mod) {
		if (IsModifierActive(mod)) {
			activeModifiers &= ~mod;
			UpdateModifierButtons();
			return true;
		} else {
			activeModifiers |= mod;
			UpdateModifierButtons();
			return false;
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_spectate) {
			RequestSpectate();
			return true;
		} else if (w == btn_find_on_map) {
			parent.EnsureVisible("LBAdminMenuFrame_PlayerMap");
			LBAdminMenuFrame_PlayerList list = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList"));
			list.InvokePlayerSelected(selected);
			return true;
		} else if (w == btn_kill) {
			if (selected && !multiSelectActive)
				StartActionConfirm(LBAdminToolRPCs.KILL_PLAYER, "#lb_adm_kill", LBTranslatedString("#lb_adm_kill_confirm_text", {"%name%", selected.GetName()}).Get());
			else if (names != "" && multiSelectActive)
				StartActionConfirm(LBAdminToolRPCs.KILL_PLAYER, "#lb_adm_kill", LBTranslatedString("#lb_adm_kill_confirm_text", {"%name%", names}).Get());
			return true;
		} else if (w == btn_heal) {
			SendPlayerListRPC(LBAdminToolRPCs.HEAL_PLAYER);
			return true;
		} else if (w == btn_tp_to) {
			if (selected) {
				if (LBFreecamCamera.instance && LBFreecamCamera.instance.fullMap) {
					LBFreecamCamera.instance.position = playerPos + "0 2 0";
				} else {
					g_Game.RPCSingleParam(null, LBAdminToolRPCs.TP_TO, new Param1<PlayerIdentity>(selected), true);
				}
			}
		} else if (w == btn_bring) {
			vector pos;
			if (LBFreecamCamera.instance && LBFreecamCamera.instance.fullMap) {
				pos = g_Game.GetCurrentCameraPosition();
			} else if (g_Game.GetPlayer()) {
				pos = g_Game.GetPlayer().GetPosition();
			} else {
				return true;
			}
			SendPlayerListTeleportRPC(LBAdminToolRPCs.BRING, pos);
		} else if (w == btn_toggle_restrain) {
			ScriptRPC rpc = SetupRPC(false);
			bool sendRestrain = !restrained;
			rpc.Write(sendRestrain);
			SendRPC(rpc, LBAdminToolRPCs.TOGGLE_RESTRAIN);
			if (multiSelectActive) {
				restrained = !restrained;
			}
		} else if (w == btn_strip) {
			if (selected && !multiSelectActive)
				StartActionConfirm(LBAdminToolRPCs.STRIP, "#lb_adm_drop", LBTranslatedString("#lb_adm_drop_confirm_text", {"%name%", selected.GetName()}).Get());
			else if (names != "" && multiSelectActive)
				StartActionConfirm(LBAdminToolRPCs.STRIP, "#lb_adm_drop", LBTranslatedString("#lb_adm_drop_confirm_text", {"%name%", names}).Get());
		} else if (w == btn_clear) {
			if (selected && !multiSelectActive)
				StartActionConfirm(LBAdminToolRPCs.CLEAR_INV, "#lb_delete", LBTranslatedString("#lb_adm_delete_confirm_text", {"%name%", selected.GetName()}).Get());
			else if (names != "" && multiSelectActive)
				StartActionConfirm(LBAdminToolRPCs.CLEAR_INV, "#lb_delete", LBTranslatedString("#lb_adm_delete_confirm_text", {"%name%", names}).Get());
			return true;
		} else if (w == btn_apply_all) {
			infoEntries.ApplyAll();
			return true;
		} else if (w == btn_copy_name) {
			if (selected && !multiSelectActive) {
				g_Game.CopyToClipboard(selected.GetName());
				LBCopiedPopup.ShowAtWidget(btn_copy_name);
			} else if (multiSelectActive) {
				g_Game.CopyToClipboard(names);
				LBCopiedPopup.ShowAtWidget(btn_copy_name);
			}
			return true;
		} else if (w == btn_copy_steamid) {
			if (selected && !multiSelectActive) {
				g_Game.CopyToClipboard(selected.GetPlainId());
				LBCopiedPopup.ShowAtWidget(btn_copy_steamid);
			} else if (multiSelectActive) {
				g_Game.CopyToClipboard(steamids);
				LBCopiedPopup.ShowAtWidget(btn_copy_steamid);
			}
			return true;
		} else if (w == btn_copy_guid) {
			if (selected && !multiSelectActive) {
				g_Game.CopyToClipboard(selected.GetId());
				LBCopiedPopup.ShowAtWidget(btn_copy_guid);
			} else if (multiSelectActive) {
				g_Game.CopyToClipboard(guids);
				LBCopiedPopup.ShowAtWidget(btn_copy_guid);
			}
			return true;
		} else if (w == btn_copy_hands) {
			if (selected && !multiSelectActive) {
				g_Game.CopyToClipboard(hands);
				LBCopiedPopup.ShowAtWidget(btn_copy_hands);
			}
			return true;
		} else if (w == btn_vomit) {
			SendStartPlayerAction(0);
			return true;
		} else if (w == btn_copy_pos && !multiSelectActive) {
			if (LBAdmins.Get().HasPermission("admin.view.playerpositions") && selected && !multiSelectActive) {
				g_Game.CopyToClipboard(playerPos.ToString());
				LBCopiedPopup.ShowAtWidget(btn_copy_pos);
			}
		} else if (w == btn_kick) {
			LBSimpleInputPopup.Get().Show("#lb_adm_kick", "#lb_adm_enter_a_kick_reason", "", ScriptCaller.Create(KickPlayer));
		} else if (w == btn_ban) {
			if (selected && parent.EnsureVisible("LBAdminMenuFrame_Banmanager")) {
				LBAdminMenuFrame_Banmanager banMgr = LBAdminMenuFrame_Banmanager.Cast(parent.GetFrame("LBAdminMenuFrame_Banmanager"));
				banMgr.CreateNewBan(selected.GetPlainId(), selected.GetName());
			}
		} else if (w == btn_scale) {
			LBSimpleInputPopup.Get().Show("#apply", "#lb_adm_enter_a_number_between_0001_and_100", "1.0", ScriptCaller.Create(OnSetScale));
		} else if (w == btn_bloody) {
			if (selected && !multiSelectActive) {
				bloodyHands = !bloodyHands;
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.TOGGLE_BLOODY_HANDS, new Param2<PlayerIdentity, bool>(selected, bloodyHands), true);
				UpdateModifierButtons();
			}
			return true;
		} else if (w == btn_message) {
			LBSimpleInputPopup.Get().Show("#lb_adm_send", "#lb_adm_enter_your_message", "", ScriptCaller.Create(SendPlayerMessage));
		} else if (w == btn_copy_cf_id && !multiSelectActive) {
			g_Game.CopyToClipboard(gamelabsId);
			LBCopiedPopup.ShowAtWidget(btn_copy_cf_id);
		} else if (w == btn_browser_cf_id && !multiSelectActive) {
			g_Game.OpenURL("https://app.cftools.cloud/profile/" + gamelabsId);
		} else if (w == btn_browser_steamid && !multiSelectActive && selected) {
			g_Game.OpenURL("https://steamcommunity.com/profiles/" + selected.GetPlainId());
		} else {
			string layoutName = w.GetName();
			foreach (LBModifierInfo info : LBModifierManager.Get().infos) {
				if (layoutName == info.layout_button_name) {
					SendModifierChange(info.id);
				}
			}
		}
		return false;
	}

	void SendPlayerMessage(string message) {
		if (message == "")
			return;
		ScriptRPC rpc = SetupRPC(true);
		if (!rpc)
			return;
		rpc.Write(message);
		SendRPC(rpc, LBAdminToolRPCs.MESSAGE_PLAYER);
	}

	void OnSetScale(string input) {
		float f = input.ToFloat();
		if (f <= 0)
			return;
		f = Math.Clamp(f, 0.001, 100);
		ScriptRPC rpc = SetupRPC(false);
		rpc.Write(f);
		SendRPC(rpc, LBAdminToolRPCs.SET_SCALE);
	}

	void SendStartPlayerAction(int action) {
		ScriptRPC rpc = SetupRPC(false);
		rpc.Write(action);
		SendRPC(rpc, LBAdminToolRPCs.START_PLAYER_ACTION);
	}

	void SendPlayerListTeleportRPC(int type, vector pos) {
		ScriptRPC rpc = SetupRPC(false);
		rpc.Write(pos);
		SendRPC(rpc, type);
	}

	void SendPlayerListRPC(int type) {
		ScriptRPC rpc = SetupRPC(false);
		SendRPC(rpc, type);
	}

	void SendModifierChange(int type) {
		bool enabled = !ToggleModifier(type);
		ScriptRPC rpc = SetupRPC(false);
		rpc.Write(type);
		rpc.Write(enabled);
		SendRPC(rpc, LBAdminToolRPCs.SET_MODIFIER_REMOTE);
	}

	void SendRPC(ScriptRPC rpc, int type) {
		rpc.Send(null, type, true);
	}

	ScriptRPC SetupRPC(bool requirePlayers) {
		array<PlayerIdentity> players = GetSelectedPlayers();
		if (requirePlayers && players.Count() == 0)
			return null;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(players);
		return rpc;
	}

	array<PlayerIdentity> GetSelectedPlayers() {
		if (multiSelectActive) {
			tempList = LBAdminMenuFrame_PlayerList.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerList")).selectedPlayers;
		} else {
			tempList = new array<PlayerIdentity>();
			if (selected)
				tempList.Insert(selected);
		}
		return tempList;
	}

	void KickPlayer(string reason) {
		ScriptRPC rpc = SetupRPC(false);
		rpc.Write(reason);
		rpc.Send(null, LBAdminToolRPCs.KICK_PLAYER, true);
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == list_sickness && LBAdmins.Get().HasPermission("admin.toggle.sickness")) {
			int index;
			if (!selected || !LBWidgetUtils.CheckSelectedInRange(list_sickness, index))
				return true;
			int agent = 1 << index;
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TOGGLE_AGENT, new Param2<PlayerIdentity, int>(selected, agent), true);
		}
		return false;
	}

	void StartActionConfirm(int rpc, string btnText, string text) {
		nextRPC = rpc;
		LBWarningPopup.Get().Show(btnText, text, ScriptCaller.Create(ConfirmAction));
	}

	void ConfirmAction() {
		SendPlayerListRPC(nextRPC);
	}

	override string GetTooltip(Widget w) {
		if (w == btn_spectate) {
			return "#lb_adm_desc_spectate";
		} else if (w == btn_find_on_map) {
			return "#lb_adm_desc_find_on_map";
		} else if (w == btn_kill) {
			return "#lb_adm_desc_kill";
		} else if (w == btn_heal) {
			return "#lb_adm_desc_heal";
		} else if (w == btn_tp_to) {
			return "#lb_adm_desc_tp_to";
		} else if (w == btn_bring) {
			return "#lb_adm_desc_bring";
		} else if (w == btn_toggle_restrain) {
			if (IsSelectedRestrained()) {
				return "#lb_adm_desc_disable_restrain";
			} else {
				return "#lb_adm_desc_enable_restrain";
			}
		} else if (w == btn_strip) {
			return "#lb_adm_desc_strip";
		} else if (w == btn_clear) {
			return "#lb_adm_desc_clear";
		} else if (w == btn_apply_all) {
			return "#lb_adm_desc_apply_all";
		} else if (w == btn_bring_back) {
			return "#lb_adm_desc_bring_back";
		} else if (w == btn_copy_pos || w == btn_copy_guid || w == btn_copy_name || w == btn_copy_hands || w == btn_copy_cf_id || w == btn_copy_steamid) {
			return "Copy";
		} else if (w == btn_browser_cf_id || w == btn_browser_steamid) {
			return "Open in Browser";
		}
		string name = w.GetName();
		foreach (LBModifierInfo info : LBModifierManager.Get().infos) {
			if (name == info.layout_button_name) {
				return info.descrption_player_info;
			}
		}
		return super.GetTooltip(w);
	}

	bool IsSelectedRestrained() {
		return restrained;
	}

	void RequestSpectate() {
		if (!selected)
			return;
		string steamid = selected.GetPlainId();
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(steamid);
		rpc.Send(null, LBAdminToolRPCs.SPECTATE, true);
		CloseMenu();

	}

}