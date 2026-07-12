class LBPlayerListEntry {

	LBPlayerList parentList;
	LBGroupMember member;

	Widget mainWidget;
	Widget healthbar;
	TextWidget playername, distance;
	Widget borderWidget;

	float lastHealth;

	void ~LBPlayerListEntry() {
		if (mainWidget)
			mainWidget.Unlink();
	}

	void Init(LBPlayerList parent, LBGroupMember groupMember) {
		parentList = parent;
		string name = LBLayoutConfig.Get.GetPlayerListLayoutName();
		LBLogger.Debug("Creating Player List Entry with " + name + " and Parent: " + parent, "AdvancedGroups");
		mainWidget = LBLayoutManager.Get().CreateLayout(name, parent.listWidget);
		ConnectClassWidgetVariables(this, mainWidget, {"mainWidget"});
		Show(false);
		InitMember(groupMember);
	}

	void InitMember(LBGroupMember groupMember) {
		member = groupMember;
	}

	void UpdateWidget() {
		if (!member || !member.online) {
			Show(false);
			return;
		}
		LBLogger.Debug("Update Playerlists Widget " + member.name, "AdvancedGroups");
		Show(true);
		ProgressBarWidget healthProgress = ProgressBarWidget.Cast(healthbar);
		if (healthProgress) {
			healthProgress.SetCurrent(member.health);
		} else {
			ImageWidget healthImage = ImageWidget.Cast(healthbar);
			if (healthImage) {
				int index = 4 - ((int)((member.health + 10) / 25));
				healthImage.LoadImageFile(0, "set:dayz_gui image:iconHealth" + index);
			}
		}
		healthbar.SetColor(GetColor(member.health));
		playername.SetText(member.name);
		lastHealth = member.health;
		if (borderWidget) {
			int borderColor = LBColorManager.Get.GetColor("Playerlist entry border");
			borderWidget.SetColor(borderColor);
		}
	}

	void UpdateDistance() {
		if (distance) {
			if (LBGroupSettingsConfig.Get.enablePlayerListDistance && member && member.steamid != LBAdmins.Get().GetMySteamid()) {
				distance.Show(true);
				vector pos = g_Game.GetCurrentCameraPosition();
				float dist = vector.Distance(member.position, pos);
				if (dist < 1000) {
					distance.SetText(" " + ((int) dist) + "m");
				} else {
					float km = ((float)((int)(dist / 100))) / 10;
					distance.SetText(" " + km + "km");
				}
			} else {
				distance.Show(false);
			}
		}
	}

	int GetColor(float health) {
		int colorFull = GetHealthFullColor();
		int colorZero = GetHealthEmptyColor();
		float colorPartFull = health / 100.0;
		return LBConverter.MixColors(colorFull, colorZero, colorPartFull);
	}

	int GetHealthEmptyColor() {
		return LBColorManager.Get.GetColor("Playerlist entry zero health");
	}

	int GetHealthFullColor() {
		return LBColorManager.Get.GetColor("Playerlist entry full health");
	}

	void Show(bool b) {
		mainWidget.Show(b);
	}

}