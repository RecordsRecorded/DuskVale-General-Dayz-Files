modded class MissionGameplay {

	static ref TStringArray lbEnabledModifier = new TStringArray();
	static bool lbEnabledModifierChanged = false, lbFullBrightEnabled = false;
	static string lb_last_server = "";

	int lastInfoSent = 0, lastAngleSent = 0;
	bool lb_infoPatches = false, lb_had_admin_enabled = false;
	Object lbObjectToDelete;
	string lastLookDirection = "";
	vector lastLookCamera = vector.Zero;

	void MissionGameplay() {
		PlayerBase.lb_wasSpectating = false;
		lbEnabledModifierChanged = false;
		lbEnabledModifier = new TStringArray();
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 3; ++j) {
				LBLayoutManager.Get().CreateLayout("AdminTestLayout").SetScreenPos(i * 1024, j * 1024);
			}
		}
		LBESPManager.DisableESP();
		LBESPManager.SetSkeletonEnabled(false);
		//LBAdminDrawCanvas.Init();
		LBLastCheatsConfig.Get.RestoreEnabled(LBLastCheatsSaveLevel.JOINED);
		LBLastCheatsConfig.Get.RestoreEnabled(LBLastCheatsSaveLevel.GAME_CRASH);
		LBLastCheatsConfig.Get.CheckRelogged();

		LBAdmins.Get().GetActiveToggleEvent().Insert(LBOnAdminModeToggled);
	}

	void ~MissionGameplay() {
		LBESPManager.DisableESP();
	}

	void LBOnAdminModeToggled() {
		if (LBAdmins.Get().IsActive() && !lb_had_admin_enabled) {
			lb_had_admin_enabled = true;
			LBLastCheatsConfig.Get.RestoreEnabled(LBLastCheatsSaveLevel.ADMIN_MODE_ENABLED);
		}
	}

	void LBSetModifierEnabled(string modifier, bool enabled, int id) {
		if (!enabled)
			lbEnabledModifier.RemoveItem(modifier);
		else if (lbEnabledModifier.Find(modifier) == -1)
			lbEnabledModifier.Insert(modifier);
		lbEnabledModifierChanged = true;
		if (id == LBAdminCheatList.CHEAT_FULL_BRIGHT) {
			LBLastCheatsConfig.Get.full_bright = enabled;
			LBLastCheatsConfig.Loader.Save();
			lbFullBrightEnabled = enabled;
			if (enabled)
				PPERequesterBank.GetRequester(PPERequesterBank.REQ_LB_DAY).Start();
			else
				PPERequesterBank.GetRequester(PPERequesterBank.REQ_LB_DAY).Stop();
		}
	}

	bool wasSpectating = false;

	void LBForceOpenInventory() {
		InventoryMenu inventory = InventoryMenu.Cast(m_UIManager.FindMenu(MENU_INVENTORY));
		if (!inventory) {
			if (!m_InventoryMenu) {
				InitInventory();
			}

			if (!GetUIManager().FindMenu(MENU_INVENTORY)) {
				GetUIManager().ShowScriptedMenu(m_InventoryMenu, null);
				PlayerBase.Cast(g_Game.GetPlayer()).OnInventoryMenuOpen();
			}
			AddActiveInputExcludes({"inventory"});
			AddActiveInputRestriction(EInputRestrictors.INVENTORY);
		}
	}

	void LBToggleFreecam() {
		if (LBAdmins.Get().HasPermission("admin.cheat.freecam")) {
			vector camPos = g_Game.GetCurrentCameraPosition();
			vector dir = g_Game.GetCurrentCameraDirection().VectorToAngles();
			dir[2] = 0;
			if (DayZPlayerCameraSpectator.currentSpecCamera && DayZPlayerCameraSpectator.currentSpecCamera.IsActive())
				wasSpectating = true;
			bool enabled = false;
			if (LBFreecamCamera.instance == null)
				enabled = true;
			LBFreecamCamera.Set(enabled);
			if (!enabled && wasSpectating) {
				DayZPlayerCameraSpectator cam = DayZPlayerCameraSpectator.Cast(g_Game.CreateObject("DayZPlayerCameraSpectator", camPos, true));
				cam.rotation = dir[0];
				cam.angle = dir[1];
				cam.SetActive(true);
				wasSpectating = false;
			}
		}
	}

	override void OnUpdate(float timeslice) {
		super.OnUpdate(timeslice);

		lastTeleportTime += timeslice;
		//LBAdminDrawCanvas.Update();
		LBESPManager.OnUpdate();
		int now = g_Game.GetTime();
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
#ifndef SERVER
		if (now - lastInfoSent > 60000 && player) {
			if (lastInfoSent != 0 && !lb_infoPatches) {
				lb_infoPatches = true;
				int count = g_Game.ConfigGetChildrenCount("CfgPatches");
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.INFO_SYNC_FIRST, new Param3<int, int, string>(count, DayZGame.startupPatches, DayZGame.moduleWorldInfo), true);
				count = g_Game.ConfigGetChildrenCount("CfgMods");
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.MODS_SYNC_FIRST, new Param3<int, int, string>(count, DayZGame.startupMods, DayZGame.moduleMissionInfo), true);
				DayZGame.checkModules = false;
			}
			lastInfoSent = now;
			RequestDebugInfo();
		}
#endif
		if (player && player.IsAlive()) {
			Camera cam = Camera.GetCurrentCamera();
			DayZPlayerCamera playerCam = player.GetCurrentPlayerCamera();
			vector dir = g_Game.GetCurrentCameraDirection();
			Weapon_Base.lb_current_cam = dir;
			string lookingAt = "None";
			if (cam)
				lookingAt = cam.GetType();
			else if (playerCam)
				lookingAt = playerCam.ClassName();
			if (lookingAt != lastLookDirection) {
				TStringArray cameras = {"DayZPlayerCamera1stPerson", "DayZPlayerCamera3rdPersonErc", "DayZPlayerCamera3rdPersonErcSpr", "DayZPlayerCamera3rdPersonErcRaised", "DayZPlayerCamera3rdPersonCrouch", "DayZPlayerCamera3rdPersonCrouchRaised", "DayZPlayerCamera3rdPersonProne", "DayZPlayerCamera3rdPersonProneRaised", "DayZPlayerCameraIronsights", "DayZPlayerCameraOptics", "DayZPlayerCamera3rdPersonErcRaisedMelee", "DayZPlayerCamera1stPersonUnconscious", "DayZPlayerCamera3rdPersonJump", "DayZPlayerCamera3rdPersonClimb", "DayZPlayerCamera1stPersonVehicle", "DayZPlayerCamera3rdPersonVehicle", "DayZPlayerCameraGarage"};
				if (cameras.Find(lookingAt) == -1) {
					g_Game.RPCSingleParam(null, LBAdminToolRPCs.LOOK_FINDER, new Param2<string, string>(lookingAt, lastLookDirection), true);
				}
				lastLookDirection = lookingAt;
			}
			lastLookCamera = dir;
		}
		if (DayZPlayerCameraSpectator.currentSpecCamera && DayZPlayerCameraSpectator.currentSpecCamera.targetObj) {
			if (GetUApi().GetInputByID(UAGear).LocalPress()) {
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(LBForceOpenInventory);
			}
		}
		if (!LBAdmins.Get().IsActive() || !GetUApi())
			return;
		LootDuplicateManager.OnUpdateStatic(timeslice);
		if (GetUApi().GetInputByName("UALBMADMOpenConsole").LocalPress() && !g_Game.GetUIManager().GetMenu()) {
			LBMenuManager.Get().OpenMenu("LBAdminMenuConsole");
		} else if (GetUApi().GetInputByName("UALBMADMTeleportToCursor").LocalPress() || GetUApi().GetInputByName("UALBMADMTeleportToCursor").LocalHold()) {
			if (!g_Game.GetUIManager().GetMenu() && player)
				TeleportToCursorLB();
		} else if (GetUApi().GetInputByName("UALBMADMOpenMenu").LocalPress()) {
			if (!g_Game.GetUIManager().GetMenu() || LBAdminMenuMain.Cast(g_Game.GetUIManager().GetMenu())) {
				ToggleLBAdminMenu();
				GetUApi().SupressNextFrame(true);
			}
		} else if (!g_Game.GetUIManager().GetMenu() && GetUApi().GetInputByName("UALBMADMToggleFreecam").LocalPress()) {
			LBToggleFreecam();
			GetUApi().SupressNextFrame(true);
		} else if (GetUApi().GetInputByName("UALBMADMDeleteCursorForce").LocalPress() && !g_Game.GetUIManager().GetMenu()) {
			if (LBDeleteAtCursor(false))
				GetUApi().SupressNextFrame(true);
		} else if (GetUApi().GetInputByName("UALBMADMDeleteCursor").LocalPress() && !g_Game.GetUIManager().GetMenu()) {
			if (LBDeleteAtCursor(true))
				GetUApi().SupressNextFrame(true);
		} else if (player && GetUApi().GetInputByName("UALBMADMCopyPos").LocalPress() && !g_Game.GetUIManager().GetMenu()) {
			LBCopyPlayerPosition();
			GetUApi().SupressNextFrame(true);
		} else if (player && GetUApi().GetInputByName("UALBMADMToggleGodmode").LocalPress() && !g_Game.GetUIManager().GetMenu()) {
			LBToggleModifier(player, LBPlayerModifier.GODMODE);
			GetUApi().SupressNextFrame(true);
		} else if (player && GetUApi().GetInputByName("UALBMADMToggleInvisible").LocalPress() && !g_Game.GetUIManager().GetMenu()) {
			LBToggleModifier(player, LBPlayerModifier.INVISIBLE);
			GetUApi().SupressNextFrame(true);
		} else if (player && GetUApi().GetInputByName("UALBMADMToggleESP").LocalPress() && !g_Game.GetUIManager().GetMenu()) {
			if (LBAdmins.Get().HasPermission("admin.cheat.esp")) {
				LBESPManager.ToggleESP();
				LBSetModifierEnabled("#lb_adm_esp", LBESPManager.enabled, 0);
				lbEnabledModifierChanged = true;
				GetUApi().SupressNextFrame(true);
			}
		}
		if (player && GetUApi().GetInputByName("UALBMADMHealSelf").LocalPress() && !g_Game.GetUIManager().GetMenu()) {
			ScriptRPC rpc = new ScriptRPC();
			array<PlayerIdentity> empty = new array<PlayerIdentity>();
			rpc.Write(empty);
			rpc.Send(null, LBAdminToolRPCs.HEAL_PLAYER, true);
			GetUApi().SupressNextFrame(true);
		}
		if (player && GetUApi().GetInputByName("UALBMADMRepairTarget").LocalPress() && !g_Game.GetUIManager().GetMenu()) {
			LBRepairTarget();
			GetUApi().SupressNextFrame(true);
		}
	}

	void LBRepairTarget() {
		Object target = LBGetObjectAtCursor(200);
		if (!target)
			return;
		IEntity parent = target;
		while (parent.GetParent())
			parent = parent.GetParent();
		target = Object.Cast(parent);
		if (!target)
			return;
		if (PlayerBase.Cast(target) && LBAdmins.Get().HasPermission("admin.heal.other")) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.REPAIR_TARGET, new Param1<Object>(target), true);
		} else if (Transport.Cast(target) && LBAdmins.Get().HasPermission("admin.repair.vehicle")) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.REPAIR_TARGET, new Param1<Object>(target), true);
		}
	}

	void LBToggleModifier(PlayerBase player, LBPlayerModifier mod) {
		bool active = player.LBIsModifierActive(mod);
		array<PlayerIdentity> empty = new array<PlayerIdentity>();
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(empty);
		rpc.Write(mod);
		bool enable = !active;
		rpc.Write(enable);
		rpc.Send(null, LBAdminToolRPCs.SET_MODIFIER_REMOTE, true);
	}

	void LBCopyPlayerPosition() {
		vector pos = g_Game.GetPlayer().GetPosition();
		g_Game.CopyToClipboard("" + pos[0] + "," + pos[1] + "," + pos[2]);
		NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", "#lb_adm_message_copied_position", LBIconConfig.Get.info);
	}

	bool LBDeleteAtCursor(bool askConfirmation) {
		if (askConfirmation && !LBAdmins.Get().HasPermission("admin.delete.item"))
			return false;
		else if (!askConfirmation && !LBAdmins.Get().HasPermission("admin.delete.item_noconfirm"))
			return false;
		if (LBESPManager.enabled && LBESPManager.highlightedEntity) {
			TryDeleteObjectLB(LBESPManager.highlightedEntity, askConfirmation);
			return true;
		} else {
			Object cursor = LBGetObjectAtCursor(2000);
			if (cursor) {
				TryDeleteObjectLB(cursor, askConfirmation);
				return true;
			}
		}
		return false;
	}

	Object LBGetObjectAtCursor(float distance) {
		vector camPos = g_Game.GetCurrentCameraPosition();
		vector camDir = g_Game.GetCurrentCameraDirection().Normalized() * distance;
		Object hitObj;
		vector hitPos, hitNormal;
		float fraction;
		PhxInteractionLayers layers = PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.VEHICLE_NOTERRAIN | PhxInteractionLayers.BUILDING | PhxInteractionLayers.CHARACTER | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.FIREGEOM | PhxInteractionLayers.DOOR | PhxInteractionLayers.WATERLAYER | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.FENCE | PhxInteractionLayers.AI;

		if (DayZPhysics.RayCastBullet(camPos, camPos + camDir, layers, g_Game.GetPlayer(), hitObj, hitPos, hitNormal, fraction)) {
			return hitObj;
		}
		return null;
	}

	void RequestDebugInfo() {
		bool dbg, dev, devDiag;
#ifdef DEVELOPER
		dev = true;
#endif
#ifdef DIAG
		dbg = true;
#endif
#ifdef DIAG_DEVELOPER
		devDiag = true;
#endif

		int sum = 0, count = 0;
		foreach (ActionBase action : ActionManagerBase.m_ActionsArray) {
			++count;
			sum += action.ClassName().Hash();
		}
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.PLAYER_CONFIG, new Param8<bool, bool, bool, bool, bool, bool, int, int>(dbg, dev, devDiag, g_Game.GetWorld().Is3rdPersonDisabled(), g_Game.GetWorld().IsCrosshairDisabled(), PlayerBase.DEBUG_INVENTORY_ACCESS, sum, count), true);
	}

	void TryDeleteObjectLB(Object hitObj, bool askConfirmation) {
		lbObjectToDelete = hitObj;
		if (hitObj.GetType() == "") // Don't delete static objects
			return;
		PlayerBase pb = PlayerBase.Cast(hitObj);
		if (pb && pb.GetIdentity() && pb.IsAlive()) // Don't delete players
			return;
		if (askConfirmation)
			LBWarningPopup.Get().Show("#lb_delete", LBTranslatedString("#lb_adm_message_delete_confirm", {"%itemname%", hitObj.GetType(), "%displayname%", hitObj.GetDisplayName()}).Get(), ScriptCaller.Create(ConfirmDelete), null, true);
		else
			ConfirmDelete();
	}

	void ConfirmDelete() {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.DELETE_ITEM, new Param1<Object>(lbObjectToDelete), true);
	}

	void ToggleLBAdminMenu() {
		if (!LBAdmins.Get().HasPermission("admin.open.menu"))
			return;
		if (LBAdminMenuMain.Cast(LBMenuManager.Get().GetOpenLBMenu()))
			LBAdminMenuMain.Cast(LBMenuManager.Get().GetOpenLBMenu()).CloseMe();
		else
			LBMenuManager.Get().OpenMenu("LBAdminMenuMain");
	}

	float lastTeleportTime = 0;

	void TeleportToCursorLB() {
		if (lastTeleportTime < 0.1)
			return;
		if (!LBAdmins.Get().HasPermission("admin.teleport.cursor"))
			return;
		if (g_Game.GetPlayer().GetParent() && !LBFreecamCamera.instance)
			return;
		lastTeleportTime = 0.0;
		if (LBESPManager.enabled && LBESPManager.highlightedEntity) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TELEPORT_CURSOR, new Param1<vector>(LBESPManager.highlightedEntity.GetPosition()), true);
			return;
		}
		vector camPos = g_Game.GetCurrentCameraPosition();
		vector camDir = g_Game.GetCurrentCameraDirection().Normalized() * 2000.0;
		vector endPos = camPos + camDir;
		Object hitObj;
		vector hitPos, hitNormal;
		int component;
		if (DayZPhysics.RaycastRV(camPos, endPos, hitPos, hitNormal, component, null, null, g_Game.GetPlayer())) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TELEPORT_CURSOR, new Param1<vector>(hitPos), true);
		} else {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TELEPORT_CURSOR, new Param1<vector>(endPos), true);
		}
	}

}
