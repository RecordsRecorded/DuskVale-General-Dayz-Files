class LBCompassHud {

	Widget mainWidget;
	Widget compassimage;
	Widget compassLineWidget;
	Widget topMarkerWidget;
	bool canEnable = false;

	void LBCompassHud() {
		LBColorManager_.Event_OnColorChange.Insert(OnColorChanged);
	}

	void ~LBCompassHud() {
		if (LBColorManager_.Event_OnColorChange)
			LBColorManager_.Event_OnColorChange.Remove(OnColorChanged);
		if (mainWidget) {
			mainWidget.Unlink();
			mainWidget = null;
		}
		LBMarker.compassInit = false;
	}

	void InitWidgets() {
		if (mainWidget || g_Game.IsServer())
			return;
		mainWidget = LBLayoutManager.Get().CreateLayout("Compass");
		ConnectClassWidgetVariables(this, mainWidget, {"mainWidget"});
		LBMarker.compassWidgetGlobal = topMarkerWidget;
		LBLogger.Debug("Initialized Compass Hud Widget", "AdvancedGroups");
		LBMarker.InitCompassWidgets();
		OnColorChanged();
	}

	void Show(bool b) {
		if (mainWidget)
			mainWidget.Show(b && canEnable);
	}

	void UpdateHud() {
		IngameHud hud = IngameHud.Cast(g_Game.GetMission().GetHud());
		if (hud) {
			bool visible = false;
			if (hud.LBIsHudVisible() && LBMarkerVisibilityManager.Get.compassEnabled && LBPlayerUtils.IsClientPlayerAlive() && !g_Game.GetUIManager().GetMenu())
				visible = true;
			Show(visible);
			if (visible) {
				float angle = GetCurrentAngle();
				compassimage.SetPos((-angle / 180.0) - 1.0, 0);
				LBMarker.currentCameraAngle = angle;
			}
		} else {
			Show(false);
		}
	}

	void UpdateCanEnableCompass() {
		canEnable = CanEnableCompass();
		LBLogger.Debug("Can Enable Compass Check: " + canEnable, "AdvancedGroups");
	}

	bool CanEnableCompass() {
		if (!LBGroupMiscConfig.Get.compassRequireItem)
			return true;
		LBLogger.Debug("Require Compass", "AdvancedGroups");
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
			return false;
		LBLogger.Debug("Player found", "AdvancedGroups");
		return HasCompassInInventory(player);
	}

	bool HasCompassInInventory(PlayerBase player) {
		array<EntityAI> items = new array<EntityAI>();
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item : items) {
			if (!item)
				continue;
			string type = item.GetType();
			LBLogger.Debug("Item In Inventory: " + type, "AdvancedGroups");
			if (LBGroupMiscConfig.Get.compassItems.Find(type) != -1)
				return true;
		}
		return false;
	}

	void OnColorChanged() {
		compassimage.SetColor(LBColorManager.Get.GetColor("Compass"));
		compassLineWidget.SetColor(LBColorManager.Get.GetColor("Compass Line"));
	}

	float GetCurrentAngle() {
		vector dir = g_Game.GetCurrentCameraDirection();
		vector angles = dir.VectorToAngles();
		if (angles[0] > 180)
			return angles[0] - 360;
		return angles[0];
	}

}
