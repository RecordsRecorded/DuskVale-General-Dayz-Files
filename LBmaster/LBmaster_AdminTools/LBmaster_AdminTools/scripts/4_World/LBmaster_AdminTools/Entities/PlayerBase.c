modded class PlayerBase {

	int lb_scale = 1000;
	int local_lb_scale = 1000;
	static bool lb_wasSpectating = false;
	bool lbIsAdmin = false;
	bool lastOpenedMenu = false, wasFreecam = false;
	int lastBleedingCountSpectating = 0, lb_lastPostFrame;
	static float lbOffsets;
	private LBPlayerModifier activeModifiers = 0;
	private LBPlayerModifier activeModifiers_Old_Synced = 0;
	private LBPlayerModifier activeModifiersLocal = 0;
	static PlayerBase lb_tempPlayer = null;
	static Object tempPlayerParent = null;
	bool invisibleLocal = false;
	bool lbFreecamEnabled = false;

	override void Init() {
		super.Init();
		RegisterNetSyncVariableInt("activeModifiers");
		RegisterNetSyncVariableBool("lbIsAdmin");
		RegisterNetSyncVariableInt("lb_scale", 0, 100000);
	}

	void LBOnUnlimitedAmmo() {
		CheckWeaponJammed();
		if (!LBIsModifierActive(LBPlayerModifier.UNLIMITED_AMMO))
			return;

		Weapon_Base hands = Weapon_Base.Cast(GetHumanInventory().GetEntityInHands());
		if (hands) {
			Magazine mag = hands.GetMagazine(hands.GetCurrentMuzzle());
			if (mag) {
#ifdef SERVER
				mag.ServerSetAmmoMax();
#else
				mag.LocalSetAmmoMax();
#endif
			} else {
#ifdef SERVER
				hands.FillInnerMagazine();
#endif
			}
		}
	}
	
	void LBOnModifierChanged(LBPlayerModifier mod, bool enabled) {} // If you registered custom modifiers, here is the point where you do something once it's enabled / disabled

	bool LBIsModifierActive(LBPlayerModifier mod) {
		return (activeModifiers & mod) != 0 || (activeModifiersLocal & mod) != 0;
	}
	
	LBPlayerModifier LBGetActiveModifiers() {
		return activeModifiers | activeModifiersLocal;
	}

	override float GetDeltaT() {
		if (LBIsModifierActive(LBPlayerModifier.ACTION_SPEEDHACK))
			return super.GetDeltaT() * 100;
		return super.GetDeltaT();
	}

#ifndef SERVER
	void PlayerBase() {
		if (LBESPManager.enabled)
			LBESPManager.AddItem(this);
		LBESPManager.allPlayers.Insert(this);
		SetEventMask(EntityEvent.POSTFRAME);
		if (Math.RandomInt(0, 100) == 0) {
			SetInvisible(true);
		}
	}

	override bool IsRestrained() {
		if (LBFreecamCamera.instance && !IsControlledPlayer())
			return true;
		return super.IsRestrained();
	}

	override int CameraHandler(int pCameraMode) {
		if (lbFreecamEnabled)
			return DayZPlayerCameras.LB_FREECAM_CAMAERA_ID;
		return super.CameraHandler(pCameraMode);
	}

	override void OnSelectPlayer() {
		if (DayZPlayerCameraSpectator.currentSpecCamera)
			g_Game.ObjectDelete(DayZPlayerCameraSpectator.currentSpecCamera);
		super.OnSelectPlayer();
		RPCSingleParam(LBAdminToolRPCs.SYNC_LOCAL_MODIFIERS, new Param1<bool>(true), true);
	}

	void ~PlayerBase() {
		LBESPManager.RemovePlayer(this);
	}

	override void EOnPostFrame(IEntity other, int extra) {
		super.EOnPostFrame(other, extra);
		SetInvisible(invisibleLocal);
		if (lb_scale != 1000) {
			SetScale(lb_scale / 1000.0);
		}
		lb_lastPostFrame = g_Game.GetTime();
	}

	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions) {
		if (!IsControlledPlayer() && (LBIsModifierActive(LBPlayerModifier.INVISIBLE) || LBIsModifierActive(LBPlayerModifier.SILENT)))
			return;
		super.GetActions(action_input_type, actions);
	}

	override void ProcessSoundEvent(string pEventType, string pUserString, int pUserInt) {
		if (!LBIsModifierActive(LBPlayerModifier.INVISIBLE) && !LBIsModifierActive(LBPlayerModifier.SILENT))
			super.ProcessSoundEvent(pEventType, pUserString, pUserInt);
	}

	override void ProcessAttachmentEvent(string pEventType, string pUserString, int pUserInt) {
		if (!LBIsModifierActive(LBPlayerModifier.INVISIBLE) && !LBIsModifierActive(LBPlayerModifier.SILENT))
			super.ProcessAttachmentEvent(pEventType, pUserString, pUserInt);
	}

	override void ProcessWeaponEvent(string pEventType, string pUserString, int pUserInt) {
		if (!LBIsModifierActive(LBPlayerModifier.INVISIBLE) && !LBIsModifierActive(LBPlayerModifier.SILENT))
			super.ProcessWeaponEvent(pEventType, pUserString, pUserInt);
	}
	override void SpawnBreathVaporEffect() {
		if (!LBIsModifierActive(LBPlayerModifier.INVISIBLE) && !LBIsModifierActive(LBPlayerModifier.SILENT))
			super.SpawnBreathVaporEffect();
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
		if (lb_tempPlayer && DayZPlayerCameraSpectator.currentSpecCamera && DayZPlayerCameraSpectator.currentSpecCamera.targetObj == this) {
			lb_tempPlayer.OnRPC(sender, rpc_type, ctx);
			return;
		}
		if (rpc_type == LBAdminToolRPCs.SYNC_LOCAL_MODIFIERS) {
			Param1<int> param;
			if (ctx.Read(param)) {
				int before = activeModifiersLocal;
				activeModifiersLocal = param.param1;
				ReconstructChangedModifier(before, activeModifiersLocal);
			}
		}
		super.OnRPC(sender, rpc_type, ctx);
	}
	
	void ReconstructChangedModifier(int before, int now) {
		int diff = before ^ now;
		// Reconstruct which modifiers changed
		for (int i = 0; i < 32; ++i) {
			int check = 1 << i;
			if (diff & check) {
				LBOnModifierChanged(check, now & check);
			}
		}
	}

	void UpdateInvisibility() {
		if (LBAdminMenuSettings.Get && LBAdminMenuSettings.Get.showPlayerInInvisible && IsControlledPlayer() || !LBIsModifierActive(LBPlayerModifier.INVISIBLE))
			invisibleLocal = false;
		else
			invisibleLocal = true;
	}

	override void OnVariablesSynchronized() {
		super.OnVariablesSynchronized();
		if (IsControlledPlayer()) {
			HumanInputController input = GetInputController();
			input.SetDisabled(LBIsModifierActive(LBPlayerModifier.FREEZE));
			m_StaminaHandler.LBSetDisabled(LBIsModifierActive(LBPlayerModifier.UNLIMITED_STAMINA));
		}
		UpdateInvisibility();
		SetInvisible(invisibleLocal);
		m_PlayerSoundManagerClient.SetLBDisabled(LBIsModifierActive(LBPlayerModifier.INVISIBLE) || LBIsModifierActive(LBPlayerModifier.SILENT));
		if (LBIsModifierActive(LBPlayerModifier.INVISIBLE))
			dBodySetSolid(this, false);
		else
			dBodySetSolid(this, true);
		SetScale(lb_scale / 1000.0);
		if (local_lb_scale != lb_scale) {
			LBLogger.Debug("Setting scale of " + this + " to " + lb_scale, "AdminTools");
			if (lb_scale == 1000) {
				dBodyEnableGravity(this, true);
			} else {
				dBodyEnableGravity(this, false);
			}
		}
		local_lb_scale = lb_scale;
		if (activeModifiers != activeModifiers_Old_Synced) {
			ReconstructChangedModifier(activeModifiers, activeModifiers_Old_Synced);
			activeModifiers_Old_Synced = activeModifiers;
		}
	}

	override void OnStepEvent(string pEventType, string pUserString, int pUserInt) {
		if (!LBIsModifierActive(LBPlayerModifier.INVISIBLE) && !LBIsModifierActive(LBPlayerModifier.SILENT))
			super.OnStepEvent(pEventType, pUserString, pUserInt);
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone) {
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);
		if (LBESPManager.enabled)
			LBESPManager.UpdateHealthLevel(this);
	}

	override void HandleView() {
		bool enabled = IsInThirdPerson();
		super.HandleView();
		if (LBAdmins.Get().HasPermissionActive("admin.use.3rdperson")) {
			if (GetUApi().GetInputByID(UAPersonView).LocalPress())
				enabled = !enabled;
			SetIsInThirdPerson(enabled);
		}
	}

#endif
	void CheckWeaponJammed() {
		bool enabled = false, allowed = false;
		float weight;
		Class clazz = GetAimingModel();
		EnScript.GetClassVar(clazz, "m_ProceduralRecoilEnabled", 0, enabled);
		EnScript.GetClassVar(clazz, "m_AimNoiseAllowed", 0, allowed);
		EnScript.GetClassVar(clazz, "m_SwayWeight", 0, weight);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(CheckWeaponJammedNext, enabled, allowed, weight);
	}

	void CheckWeaponJammedNext(bool enabled, bool allowed, float weight) {
		if (!IsAlive())
			return;
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.JAMMED_INFO, new Param4<bool, bool, float, float>(enabled, allowed, weight, lbOffsets), true);
	}
}