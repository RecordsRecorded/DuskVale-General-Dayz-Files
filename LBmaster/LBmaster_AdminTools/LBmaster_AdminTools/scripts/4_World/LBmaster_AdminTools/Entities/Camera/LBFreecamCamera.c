class LBFreecamCamera : DayZPlayerCamera {

	bool disabledMovement = false, spaceWasDown = false;

	vector lastOrientation;
	static vector orientation, position;

	vector posAdd = "0 0 0";
	float smoothPositionVelocityX[1];
	float smoothPositionVelocityY[1];
	float smoothPositionVelocityZ[1];

	vector orientAdd = "0 0 0";
	float smoothOrientationVelocityX[1];
	float smoothOrientationVelocityY[1];
	float smoothOrientationVelocityZ[1];

	Object targetedObject;
	vector lastTargetObjectPosition = vector.Zero;
	vector targetPositionChange = vector.Zero;

	static bool fullMap = false;
	float mapPosUpdate = 0.0;

	static LBFreecamCamera instance;

	static bool leftPressed = false;
	bool frozen = false, skipUnfreeze = false;

	void LBFreecamCamera(DayZPlayer pPlayer, HumanInputController pInput) {
		instance = this;
		SetFrozen(false);
		smoothPositionVelocityX[0] = 0;
		smoothPositionVelocityY[0] = 0;
		smoothPositionVelocityZ[0] = 0;
		smoothOrientationVelocityX[0] = 0;
		smoothOrientationVelocityY[0] = 0;
		smoothOrientationVelocityZ[0] = 0;
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.TOGGLE_CHEAT, new Param2<int, bool>(14, true), true);
	}

	void ~LBFreecamCamera() {
		if (!skipUnfreeze) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TOGGLE_CHEAT, new Param2<int, bool>(14, false), true);
			SetFrozen(true);
		}
	}

	override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult) {
		LBFreecamCamera prev = LBFreecamCamera.Cast(pPrevCamera);
		if (prev) {
			prev.skipUnfreeze = true;
			if (prev.frozen)
				SetFrozen(true);
		}
		orientation[2] = 0;
		if (pPrevCamera)
			lastOrientation = pPrevCamera.GetBaseAngles();
		lastTargetObjectPosition = vector.Zero;
		targetedObject = null;
	}

	override vector GetBaseAngles() {
		return lastOrientation;
	}

	override string GetCameraName() {
		return "LBFreecamCamera";
	}

	void SetFrozen(bool frozen_) {
		this.frozen = frozen_;
		if (!g_Game)
			return;
		DayZPlayer pPlayer = g_Game.GetPlayer();
		if (!pPlayer)
			return;
		HumanInputController pInput = pPlayer.GetInputController();
		if (frozen) {
			pInput.SetDisabled(false);
			pInput.OverrideMovementSpeed(HumanInputControllerOverrideType.DISABLED, 0);
		} else {
			pInput.OverrideMovementSpeed(HumanInputControllerOverrideType.ENABLED, 0);
			pInput.SetDisabled(true);
		}
	}

	static bool IsActive() {
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		return player && player.lbFreecamEnabled;
	}

	static void Enable() {
		fullMap = false;
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
			return;
		if (LBAdminMenuSettings.Get.freecamResetFOVOnEnter && LBAdminMenuSettings.Get.freecamFOV != 55.0) {
			LBAdminMenuSettings.Get.freecamFOV = 55.0;
			LBAdminMenuSettings.Loader.Save();
		}
		orientation = g_Game.GetCurrentCameraDirection().VectorToAngles();
		position = g_Game.GetCurrentCameraPosition();
		if (orientation[1] > 180)
			orientation[1] = orientation[1] - 360;
		LBLogger.Debug("Changing To Freecam Camera. Prev Dir: " + orientation, "AdminTools");
		for (int x = 0; x < 64; x++) {
			DayZPlayerCameras.RegisterTransitionTime(x, DayZPlayerCameras.LB_FREECAM_CAMAERA_ID, 0, true);
		}
		player.GetDayZPlayerType().RegisterCameraCreator(DayZPlayerCameras.LB_FREECAM_CAMAERA_ID, LBFreecamCamera);
		LBLogger.Debug("Camera Registered", "AdminTools");

		player.lbFreecamEnabled = true;
		LBLogger.Debug("Freecam Camera Enabled", "AdminTools");
	}

	static void Disable() {
		if (fullMap) {
			fullMap = false;
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.FREECAM_FULL_MAP, new Param2<bool, vector>(false, g_Game.GetCurrentCameraPosition()), true);
		}
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		player.lbFreecamEnabled = false;
		LBLogger.Debug("Freecam Camera Disabled", "AdminTools");
	}

	static void Toggle() {
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		Set(!player.lbFreecamEnabled);
	}

	static void Set(bool enabled) {
		LBLogger.Debug("Setting Freecam Camera " + enabled, "AdminTools");
		if (enabled)
			Enable();
		else
			Disable();
	}

	void SetTarget(Object target) {
		ResetTarget();
		targetedObject = target;
		if (target) {
			lastTargetObjectPosition = target.GetPosition();
			string name = target.GetDisplayName();
			if (name == "")
				name = target.GetType();
			if (name == "")
				name = Object.GetDebugName(target);
			NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", LBTranslatedString("#lb_adm_freecam_target", {"%name%", name}).Get(), LBIconConfig.Get.info);
		} else {
			NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", "#lb_adm_freecam_no_target", LBIconConfig.Get.info);
		}
	}

	void ResetTarget() {
		lastTargetObjectPosition = vector.Zero;
		targetedObject = null;
	}

	Object GetObjectAtCursor(out vector hitPos) {
		vector camPos = g_Game.GetCurrentCameraPosition();
		vector camDir = g_Game.GetCurrentCameraDirection().Normalized() * 2000.0;
		vector endPos = camPos + camDir;
		Object hitObj;
		vector hitNormal;
		int component;
		set<Object> objects = new set<Object>();
		DayZPhysics.RaycastRV(camPos, endPos, hitPos, hitNormal, component, objects, null, g_Game.GetPlayer(), false, false, ObjIntersectGeom);
		if (objects.Count() > 0)
			return objects.Get(0);
		return null;
	}

	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult) {
		super.OnUpdate(pDt, pOutResult);
		if (!frozen) {
			vector hitPos;
			if (g_Game.LBIsKeyDown(KeyCode.KC_SPACE)) {
				if (!spaceWasDown) {
					spaceWasDown = true;
					Object newTarget = GetObjectAtCursor(hitPos);
					if (newTarget != targetedObject)
						SetTarget(newTarget);
				}
			} else {
				spaceWasDown = false;
			}
			if ((GetMouseState(MouseState.MIDDLE) & MB_PRESSED_MASK) && !g_Game.GetUIManager().GetMenu()) {
				if (!leftPressed) {
					if (LBESPManager.enabled && LBESPManager.highlightedEntity) {
						position = LBESPManager.highlightedEntity.GetPosition();
					} else {
						GetObjectAtCursor(hitPos);
						position = hitPos;
					}
				}
				leftPressed = true;
			} else {
				leftPressed = false;
			}
			float speedChange = GetUApi().GetInputByName("UAPrevAction").LocalValue() - GetUApi().GetInputByName("UANextAction").LocalValue();
			if (g_Game.GetUIManager().GetMenu())
				speedChange = 0;
			if (g_Game.LBIsKeyDown(KeyCode.KC_LCONTROL)) {
				LBAdminMenuSettings.Get.freecamFOV = Math.Clamp(LBAdminMenuSettings.Get.freecamFOV - speedChange, 1.0, 360);
			} else {
				if (LBAdminMenuSettings.Get.freecamResetFOVOnLettingGo && LBAdminMenuSettings.Get.freecamFOV != 55.0) {
					LBAdminMenuSettings.Get.freecamFOV = 55.0;
					LBAdminMenuSettings.Loader.Save();
				}
				LBAdminMenuSettings.Get.freecamSpeed = Math.Clamp(LBAdminMenuSettings.Get.freecamSpeed + speedChange * 0.1, 0.01, 5);
			}

			if (speedChange != 0) {
				LBAdminMenuSettings.Loader.Save();
			}
			DayZPlayer player = g_Game.GetPlayer();
			if (player) {
				disabledMovement = true;
				HumanInputController controller = player.GetInputController();
				controller.SetDisabled(true);
				if (!player.IsAlive()) {
					Disable();
				}
			}
			float forward = GetUApi().GetInputByName("UAMoveForward").LocalValue() - GetUApi().GetInputByName("UAMoveBack").LocalValue();
			float left = GetUApi().GetInputByName("UAMoveLeft").LocalValue() - GetUApi().GetInputByName("UAMoveRight").LocalValue();
			float up = GetUApi().GetInputByName("UAMoveUp").LocalValue() - GetUApi().GetInputByName("UAMoveDown").LocalValue();
			bool turbo = GetUApi().GetInputByName("UATurbo").LocalValue();
			float lookUp = (GetUApi().GetInputByName("UAAimUp").LocalValue() - GetUApi().GetInputByName("UAAimDown").LocalValue());
			float lookLeft = (GetUApi().GetInputByName("UAAimLeft").LocalValue() - GetUApi().GetInputByName("UAAimRight").LocalValue());
			vector orient = Vector(-lookLeft, lookUp, 0) * pDt * LBAdminMenuSettings.Get.freecamMouseSensitivity * 5;
			HandleOrientationAdd(orient, pDt);
			Math3D.YawPitchRollMatrix(orientation, pOutResult.m_CameraTM);
			float speedMult = Math.Pow(LBAdminMenuSettings.Get.freecamSpeed, 5) * pDt;
			if (turbo) {
				speedMult = speedMult * LBAdminMenuSettings.Get.freecamTurbo;
			}
			vector moveDir = Vector(-left, 0, forward).Normalized() * speedMult;
			vector move = moveDir.Multiply3(pOutResult.m_CameraTM) + Vector(0, up * speedMult, 0);
			if (targetedObject) {
				vector targetPos = targetedObject.GetPosition();
				vector targetOff = targetPos - lastTargetObjectPosition;
				move = move + targetOff;
				lastTargetObjectPosition = targetPos;
			}
			HandlePositionAdd(move, pDt);
		} else {
			Math3D.YawPitchRollMatrix(orientation, pOutResult.m_CameraTM);
		}

		pOutResult.m_CameraTM[3] = position;

		pOutResult.m_fDistance = -0.0001;
		pOutResult.m_fUseHeading = 0.0;
		pOutResult.m_fInsideCamera = 0.0;
		pOutResult.m_fShootFromCamera = 0.0;
		pOutResult.m_fPositionModelSpace = 0.0;

		pOutResult.m_iDirectBone = -1;
		pOutResult.m_iDirectBoneMode = 0.0;
		pOutResult.m_fPositionModelSpace = 0.0;
		pOutResult.m_CollisionIgnoreEntity = m_pPlayer;

		pOutResult.m_fIgnoreParentRoll = 1.0;
		pOutResult.m_fIgnoreParentPitch = 1.0;
		pOutResult.m_fIgnoreParentYaw	= 1.0;
		pOutResult.m_bOwnerTMOverride	= 1.0;

		pOutResult.m_fFovAbsolute	= LBAdminMenuSettings.Get.freecamFOV * Math.DEG2RAD;

		mapPosUpdate += pDt;
		if (fullMap && mapPosUpdate > 0.5) {
			g_Game.UpdateSpectatorPosition(position);
			mapPosUpdate = 0.0;
		}
	}

	void HandleOrientationAdd(vector orient, float pDt) {
		float movementSmooth = LBAdminMenuSettings.Get.freecamMouseSmooth / 100.0;
		if (movementSmooth <= 0.0) {
			orientAdd = orient;
			smoothOrientationVelocityX[0] = 0;
			smoothOrientationVelocityY[0] = 0;
			smoothOrientationVelocityZ[0] = 0;
		} else if (orientAdd != vector.Zero || orient != vector.Zero) {
			orientAdd[0] = Math.SmoothCD(orientAdd[0], orient[0], smoothOrientationVelocityX, movementSmooth, 1000, pDt);
			orientAdd[1] = Math.SmoothCD(orientAdd[1], orient[1], smoothOrientationVelocityY, movementSmooth, 1000, pDt);
			orientAdd[2] = Math.SmoothCD(orientAdd[2], orient[2], smoothOrientationVelocityZ, movementSmooth, 1000, pDt);
		}
		orientation = orientation + orientAdd;
		if (orientation[0] < -180)
			orientation[0] = orientation[0] + 360;
		else if (orientation[0] > 180)
			orientation[0] = orientation[0] - 360;
		if (orientation[1] <= -89)
			orientation[1] = -89;
		else if (orientation[1] >= 89)
			orientation[1] = 89;

	}

	void HandlePositionAdd(vector move, float pDt) {
		float movementSmooth = LBAdminMenuSettings.Get.freecamMovementSmooth / 100.0;
		if (movementSmooth <= 0.0) {
			posAdd = move;
			smoothPositionVelocityX[0] = 0;
			smoothPositionVelocityY[0] = 0;
			smoothPositionVelocityZ[0] = 0;
		} else if (posAdd != vector.Zero || move != vector.Zero) {
			posAdd[0] = Math.SmoothCD(posAdd[0], move[0], smoothPositionVelocityX, movementSmooth, 1000, pDt);
			posAdd[1] = Math.SmoothCD(posAdd[1], move[1], smoothPositionVelocityY, movementSmooth, 1000, pDt);
			posAdd[2] = Math.SmoothCD(posAdd[2], move[2], smoothPositionVelocityZ, movementSmooth, 1000, pDt);
		}
		position = position + posAdd;
		if (!LBAdminMenuSettings.Get.freecamAllowUnderground) {
			float height = g_Game.SurfaceY(position[0], position[2]) + 0.1;
			if (position[1] < height)
				position[1] = height;
		}
	}

}