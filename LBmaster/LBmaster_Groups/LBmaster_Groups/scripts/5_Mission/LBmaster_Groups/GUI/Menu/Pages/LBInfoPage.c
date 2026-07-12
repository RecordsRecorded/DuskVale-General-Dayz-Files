class LBInfoPage : LBGroupPage {
	const int INFO_BUTTON_COUNT = 6;

	ref array<ButtonWidget> leftButtons = new array<ButtonWidget>();
	ref array<TextWidget> leftButtonsText = new array<TextWidget>();
	ref TStringArray buttonLinks = new TStringArray();

	ButtonWidget btnCpyPlayerCoords;

	TextWidget txt_0, txt_1, txt_2, txt_3_0, txt_3_1;
	TextWidget desc_txt0, desc_txt1, desc_txt2, desc_txt3;
	Widget mousePosPanel;

	void LBInfoPage(LBPageSettingsType type_) {
		LBLayoutConfig_.Event_StreamerModeChanged.Insert(OnStreamerModeChange);
	}
	void ~LBInfoPage() {
		if (LBLayoutConfig_.Event_StreamerModeChanged)
			LBLayoutConfig_.Event_StreamerModeChanged.Remove(OnStreamerModeChange);
	}

	override bool InitPage(LBGroupUI parentUI) {
		return super.InitPage(parentUI, 0, 0, LBPagesConfig.Get.GetButtonText(pageType), false, LBPagesConfig.Get.GetPagePriority(pageType));
	}

	override void InitMainWidget() {
		for (int i = 0; i < INFO_BUTTON_COUNT; i++) {
			leftButtons.Insert(ButtonWidget.Cast(rootWidget.FindAnyWidget("button" + i)));
			leftButtonsText.Insert(TextWidget.Cast(rootWidget.FindAnyWidget("buttonsubtext" + i)));
		}
		btnCpyPlayerCoords = ButtonWidget.Cast(rootWidget.FindAnyWidget("btnCpyPlayerCoords"));
		ConnectClassWidgetVariables(this, rootWidget, {"rootWidget", "buttonWidget"});

		if (!LBPagesConfig.Get.infoPageSettings.enableModCreatorMention) {
			rootWidget.FindAnyWidget("modcreator").Show(false);
		}
		LBLogger.Debug("Initialized Buttons: " + leftButtons.Count(), "AdvancedGroups");
		SetButtonContent();
		UpdateInfoCounter();
		if (btnCpyPlayerCoords && !LBPagesConfig.Get.infoPageSettings.showCursorCoordinates)
			btnCpyPlayerCoords.Show(false);
		OnStreamerModeChange(LBLayoutConfig.Get.streamerModeEnabled);
	}

	override string GetTooltip(Widget w) {
		if (w == btnCpyPlayerCoords)
			return "Copy your current x,y,z position and orientation to clipboard";
		for (int i = 0; i < leftButtons.Count(); i++) {
			ButtonWidget btn = leftButtons.Get(i);
			if (w == btn) {
				LBButtonConfig cfg = LBPagesConfig.Get.infoPageSettings.buttonConfig.Get(i);
				return cfg.hoverTooltip;
			}
		}
		return "";

	}

	override bool OnClick(Widget w) {
		for (int i = 0; i < leftButtons.Count(); i++) {
			ButtonWidget btn = leftButtons.Get(i);
			if (w == btn) {
				string lnk = buttonLinks.Get(i);
				if (lnk && lnk.Length() > 0)
					g_Game.OpenURL(lnk);
				return true;
			}
		}
		if (w == btnCpyPlayerCoords) {
			vector pos = g_Game.GetPlayer().GetPosition();
			string posStr = "<" + pos[0] + " " + pos[1] + " " + pos[2] + "> " + GetCurrentAngle() + " Degree";
			g_Game.CopyToClipboard(posStr);
			LBCopiedPopup.ShowAtWidget(btnCpyPlayerCoords);
		}
		return false;
	}

	float GetCurrentAngle() {
		vector dir = g_Game.GetCurrentCameraDirection();
		vector angles = dir.VectorToAngles();
		if (angles[0] < 0)
			return angles[0] + 360;
		if (angles[0] > 360)
			return angles[0] - 360;
		return angles[0];
	}

	override void OnUpdateSlow() {
		UpdateInfoCounter();
	}

	override void OnUpdateFrame() {
		SetCoordinatesField();
	}

	void SetCoordinatesField() {
		if (!LBPagesConfig.Get.infoPageSettings.showCursorCoordinates) {
			txt_3_0.Show(false);
			txt_3_1.Show(false);
			desc_txt3.Show(false);
			mousePosPanel.Show(false);
			return;
		}
		int x, y;
		GetMousePos(x, y);
		vector mousePos = Vector(x, y, 0);
		vector worldPos = parent.mapWidget.ScreenToMap(mousePos);
		float x1 = worldPos[0];
		float x2 = worldPos[2];
		x = x1;
		int z = x2;
		if (LBFOWConfig.Get.CannotInteractWith(Vector(x, 0, z), LBFOWConfig.Get.hideCursorCoordinates)) {
			txt_3_0.SetText("X: ??");
			txt_3_1.SetText("Z: ??");
		} else {
			txt_3_0.SetText("X:" + x);
			txt_3_1.SetText("Z:" + z);
		}
	}

	void SetButtonContent() {
		if (leftButtons.Count() == 0)
			return;
		array<ref LBButtonConfig> btns = LBPagesConfig.Get.infoPageSettings.buttonConfig;
		for (int i = 0; i < INFO_BUTTON_COUNT; i++) {
			if (btns.Count() <= i) {
				leftButtons.Get(i).Show(false);
				continue;
			}
			LBButtonConfig cfg = btns.Get(i);
			if (!cfg || !cfg.IsValid()) {
				leftButtons.Get(i).Show(false);
				continue;
			}
			leftButtons.Get(i).SetText(cfg.buttonName);
			leftButtonsText.Get(i).SetText(cfg.subtext);
			buttonLinks.Insert(cfg.link);
		}
	}
	void UpdateSurvivorCount() {
		if (txt_0 && desc_txt0) {
			if (!LBPagesConfig.Get.infoPageSettings.showSurvivorCount) {
				txt_0.Show(false);
				desc_txt0.Show(false);
				return;
			}
			int cnt = LBPlayerUtils.GetOnlinePlayerCount();
			if (cnt == 1)
				txt_0.SetText("" + cnt);
			else
				txt_0.SetText("" + cnt);
		}
	}

	void UpdateInfoCounter() {
		UpdateIngameTime();
		UpdateServerTime();
		UpdateSurvivorCount();
	}

	void OnStreamerModeChange(bool enabled) {
		bool show = !enabled;
		LBLogger.Debug("OnStreamerModeChange InfoPage: " + enabled, "AdvancedGroups");
		foreach (ButtonWidget btn : leftButtons) {
			btn.Show(show);
		}
		foreach (TextWidget txt : leftButtonsText) {
			txt.Show(show);
		}
		SetButtonContent();
		if (txt_0)
			txt_0.Show(show);
		if (txt_1)
			txt_1.Show(show);
		if (txt_2)
			txt_2.Show(show);
		if (txt_3_0)
			txt_3_0.Show(show);
		if (txt_3_1)
			txt_3_1.Show(show);
		if (desc_txt0)
			desc_txt0.Show(show);
		if (desc_txt1)
			desc_txt1.Show(show);
		if (desc_txt2)
			desc_txt2.Show(show);
		if (desc_txt3)
			desc_txt3.Show(show);
		if (mousePosPanel)
			mousePosPanel.Show(show);
		UpdateInfoCounter();
		SetCoordinatesField();
	}

	void UpdateIngameTime() {
		if (txt_1 && desc_txt1) {
			if (!LBPagesConfig.Get.infoPageSettings.showIngameTime) {
				txt_1.Show(false);
				desc_txt1.Show(false);
				return;
			}
			if (!g_Game || !g_Game.GetWorld())
				return;
			int year, month, day, hour, minute;
			g_Game.GetWorld().GetDate(year, month, day, hour, minute);
			string hourStr = "" + hour;
			if (hour < 10)
				hourStr = "0" + hour;
			string minuteStr = "" + minute;
			if (minute < 10)
				minuteStr = "0" + minute;
			string timeStr = "" + hourStr + ":" + minuteStr + " (x" + MissionGameplay.acceleration + ")";
			txt_1.SetText(timeStr);
		}
	}

	void UpdateServerTime() {
		if (txt_2 && desc_txt2) {
			if (!LBPagesConfig.Get.infoPageSettings.showRealTime) {
				txt_2.Show(false);
				desc_txt2.Show(false);
				return;
			}
			if (!g_Game || !g_Game.GetMission())
				return;
			MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
			int offset = mission.serverToClientTimeOffset;
			if (offset < 0)
				offset += 24 * 3600;
			LBDate now = LBDate.Init(false);
			int hours = now.m_hour;
			int min = now.m_min;
			min += offset / 60;
			while (min >= 60) {
				min -= 60;
				hours += 1;
			}
			if (hours >= 24)
				hours -= 24;
			string timeStr = "";
			if (hours < 10) {
				timeStr = "0" + hours;
			} else {
				timeStr = "" + hours;
			}
			timeStr += ":";
			if (min < 10) {
				timeStr += "0" + min;
			} else {
				timeStr += "" + min;
			}
			txt_2.SetText(timeStr);
		}
	}

}