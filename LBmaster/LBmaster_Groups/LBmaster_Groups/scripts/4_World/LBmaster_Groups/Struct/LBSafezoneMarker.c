class LBSafezoneMarker : LBGroupMember {

	[NonSerialized()]
	TextWidget title_name;
	[NonSerialized()]
	LBSafezoneMarkerEntry configEntry;

	override void FindPlayerBase() {}
	override void InitCompassWidget() {}
	override bool ShowMarkerMapOrGPS(bool isMap) {
		return false;
	}
	override bool IsGroupMarker() {
		return false;
	}
	override bool ShowDistance3D() {
		return false;
	}
	override bool ShowPlayerMarker() {
		return true;
	}
	override string GetIcon() {
		return LBMarkerVisibilityManager.Get.playerMarkerIcon;
	}
	override bool ShowMarker3D() {
		if (clientPBFound.IsInMyLBGroup(true) || !LBPlayerUtils.IsPlayerAlive(clientPBFound) || IsInvisible())
			return false;
		Man myPlayer = g_Game.GetPlayer();
		if (!LBPlayerUtils.IsPlayerAlive(myPlayer) || !LBSafezoneMarkers.Get.IsInRadius(LBPlayerUtils.GetHeadPosition(myPlayer), LBPlayerUtils.GetHeadPosition(myPlayer)))
			return false;
		return LBSafezoneMarkers.Get.IsInRadius(LBPlayerUtils.GetHeadPosition(clientPBFound), LBPlayerUtils.GetHeadPosition(myPlayer));
	}

	override void InitMarker() {
		super.InitMarker();
		if (iconExtraWidget) {
			iconExtraWidget.LoadImageFile(0, "set:dayz_gui image:mic");
			iconExtraWidget.SetColor(ARGB(255, 255, 0, 0));
		}
		title_name = TextWidget.Cast(mainWidget.FindAnyWidget("title_name"));
	}

	void UpdateTitle(string title, int titleColor) {
		if (title_name) {
			if (title == "NONE") {
				title_name.Show(false);
			} else {
				title_name.Show(true);
				title_name.SetText("    " + title);
				title_name.SetColor(titleColor);
			}
		}
	}

	bool IsInvisible() {
		return !clientPBFound.IsFlagSet(EntityFlags.VISIBLE);
	}

	override bool UpdateMarkerClient() {
		if (clientPBFound && clientPBFound.lb_is_speaking) {
			iconExtraWidget.Show(true);
		} else {
			iconExtraWidget.Show(false);
		}
		return super.UpdateMarkerClient();
	}

	override int Get3DColorARGB() {
		if (!configEntry)
			return -1;
		return configEntry.color.GetColorARGB();
	}

	static LBSafezoneMarker CreateSafezoneMarker(PlayerBase pb) {
		if (!LBSafezoneMarkers.Get || !pb.GetIdentity())
			return null;
		if (pb == g_Game.GetPlayer() && !LBSafezoneMarkers.Get.showOwnPlayerTag)
			return null;
		if (pb.safezoneMarkerIndex < 0 || pb.safezoneMarkerIndex >= LBSafezoneMarkers.Get.safezoneMarkers.Count())
			return null;

		PlayerIdentity identity = pb.GetIdentity();
		LBSafezoneMarker safezoneMarker = new LBSafezoneMarker();

		safezoneMarker.configEntry = LBSafezoneMarkers.Get.safezoneMarkers.Get(pb.safezoneMarkerIndex);
		safezoneMarker.CreateMember(pb);
		safezoneMarker.clientPBFound = pb;
		if (LBSafezoneMarkers.Get.obfuscatePlayerNames && !safezoneMarker.configEntry.ignoreNameObfuscationSetting) {
			safezoneMarker.name = LBPlayerNameObfuscation.GetObfuscatedName(pb);
		} else {
			safezoneMarker.name = safezoneMarker.configEntry.prefix + safezoneMarker.name;
		}
		safezoneMarker.InitMarker();
		safezoneMarker.UpdateTitle(pb.lbSafezoneTitle, pb.lbSafezoneTitleColor);
		return safezoneMarker;
	}

}