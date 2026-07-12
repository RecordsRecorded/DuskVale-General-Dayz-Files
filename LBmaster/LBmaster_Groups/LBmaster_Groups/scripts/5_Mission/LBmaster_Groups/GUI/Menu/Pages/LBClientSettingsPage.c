class LBClientSettingsPage : LBGroupPage {

	const int CHAT_SIZE_START = 7;

	SliderWidget sliderA, sliderR, sliderG, sliderB;
	EditBoxWidget editA, editR, editG, editB;
	Widget colorPreview;
	TextListboxWidget availableColors;
	ButtonWidget toDefault;

	ButtonWidget btn_ResetAll, btn_Reload_From_Config, btn_Save_Config;
	XComboBoxWidget comboboxPingIcon, comboboxPingSize, comboboxChatSize, comboboxPlayerMarkerPos, comboboxPlayerIcon, comboboxPlayerSize;
	ImageWidget imagePingIcon, imagePlayerIcon;
	CheckBoxWidget chckbx_Playerlist, chckbx_Compass, chckbx_ShowChatTag, chckbx_ShowClantextures, chckbx_StreamerMode, chckbx_GPS, chckbx_show_no_build, checkMarkerStyleIcon, checkMarkerStyleName, checkMarkerStyleDistance, chckbx_centerPlayer;

	SliderWidget sliderY, sliderX, sliderGPSZoom;
	Widget previewIcon;
	TextListboxWidget availableLayoutsList;
	CheckBoxWidget invertX, invertY;
	ButtonWidget toDefaultPosition;

	TextListboxWidget availableLayoutsListStyles;
	XComboBoxWidget comboBoxStyle, comboPlayerlistStyleTemp, comboboxGPSSize;
	ButtonWidget toDefaultStyle;

	XComboBoxWidget comboboxChatVisibility;
	ButtonWidget btnToggleChatVisibility;
	Widget chatVisibilityMark, panel_chat_settings;

	Widget txtChatBorder;
	SliderWidget sliderBorder;

	ref TStringArray colorArray = new TStringArray();
	ref TStringArray positionsArray = new TStringArray();
	ref TStringArray stylesArray = new TStringArray();
	ref TStringArray pingIconsArray = new TStringArray();
	ref TStringArray playerIconsArray = new TStringArray();

	override bool InitPage(LBGroupUI parentUI) {
		return super.InitPage(parentUI, 3, 0, LBPagesConfig.Get.GetButtonText(pageType), true, LBPagesConfig.Get.GetPagePriority(pageType));
	}

	override void InitMainWidget() {
		ConnectClassWidgetVariables(this, rootWidget, {"buttonWidget", "rootWidget"}, {"invertX", "checkInvertX", "invertY", "checkInvertY"});
		comboPlayerlistStyleTemp.AddItem("default");
		comboPlayerlistStyleTemp.AddItem("small");
		comboPlayerlistStyleTemp.AddItem("tiny");
		comboPlayerlistStyleTemp.AddItem("cross");

		comboboxGPSSize.AddItem("default");
		comboboxGPSSize.AddItem("small");
		comboboxGPSSize.AddItem("tiny");
		comboboxGPSSize.AddItem("big");

		comboboxPlayerMarkerPos.AddItem("Head");
		comboboxPlayerMarkerPos.AddItem("Stomach");
		comboboxPlayerMarkerPos.AddItem("Feet");
	}

	override void OnShow() {
		super.OnShow();
		ReloadAll();
	}

	void ReloadButtonVisibility() {
		comboboxGPSSize.Enable(LBGroupMiscConfig.Get.enableGPS);
		sliderGPSZoom.Enable(LBGroupMiscConfig.Get.enableGPS);

		comboboxPingIcon.Enable(LBMarkerSettingsConfig.Get.CanUsePing());
		comboboxPingSize.Enable(LBMarkerSettingsConfig.Get.CanUsePing());

		comboPlayerlistStyleTemp.Enable(LBGroupSettingsConfig.Get.enablePlayerList);
		bool showChat = false;
#ifndef LB_DISABLE_CHAT
		showChat = true;
#endif

		comboboxChatSize.Enable(showChat);
		chckbx_ShowChatTag.Enable(showChat);
		if (txtChatBorder)
			txtChatBorder.Enable(showChat);
		if (sliderBorder)
			sliderBorder.Enable(showChat);

		if (comboboxChatVisibility)
			comboboxChatVisibility.Show(showChat);
		if (btnToggleChatVisibility)
			btnToggleChatVisibility.Show(showChat);
		if (chatVisibilityMark)
			chatVisibilityMark.Show(showChat);
		if (!showChat) {
			chckbx_ShowChatTag.SetTextColor(ARGB(255, 128, 128, 128));
		} else {
			chckbx_ShowChatTag.SetTextColor(ARGB(255, 255, 255, 255));
		}
		chckbx_show_no_build.Enable(LB_NoBuildConfig.Get.enabled);
		if (!LB_NoBuildConfig.Get.enabled) {
			chckbx_show_no_build.SetTextColor(ARGB(255, 128, 128, 128));
		} else {
			chckbx_ShowChatTag.SetTextColor(ARGB(255, 255, 255, 255));
		}
		chckbx_GPS.Enable(LBGroupMiscConfig.Get.enableGPS);
		if (!LBGroupMiscConfig.Get.enableGPS) {
			chckbx_GPS.SetTextColor(ARGB(255, 128, 128, 128));
		} else {
			chckbx_ShowChatTag.SetTextColor(ARGB(255, 255, 255, 255));
		}
		chckbx_Compass.Enable(LBGroupMiscConfig.Get.enableCompassHud);
		if (!LBGroupMiscConfig.Get.enableCompassHud) {
			chckbx_Compass.SetTextColor(ARGB(255, 128, 128, 128));
		} else {
			chckbx_ShowChatTag.SetTextColor(ARGB(255, 255, 255, 255));
		}
		chckbx_Playerlist.Enable(LBGroupSettingsConfig.Get.enablePlayerList);
		if (!LBGroupSettingsConfig.Get.enablePlayerList) {
			chckbx_Playerlist.SetTextColor(ARGB(255, 128, 128, 128));
		} else {
			chckbx_ShowChatTag.SetTextColor(ARGB(255, 255, 255, 255));
		}

	}

	void ReloadAll(bool force = false) {
		ReloadButtonVisibility();
		ReloadColors(force);
		ReloadPositions(force);
		ReloadPingMarkers(force);
		ReloadPlayerMarkers(force);
		ReloadStyles();
		ReloadTextSizes();
		ReloadPingSizes();
		ReloadPlayerSizes();
		OnIconPreviewPositionChange();
		ReloadPlayerMarkerPos();
		ReloadPlayerMarkerStyle();
		ReloadGPS();
		ReloadChatVisibility();
		chckbx_Playerlist.SetChecked(LBMarkerVisibilityManager.Get.playerlistEnabled);
		chckbx_Compass.SetChecked(LBMarkerVisibilityManager.Get.compassEnabled);
		chckbx_ShowClantextures.SetChecked(!LBMarkerVisibilityManager.Get.disableShowClantextures);
		chckbx_StreamerMode.SetChecked(LBLayoutConfig.Get.streamerModeEnabled);
		chckbx_show_no_build.SetChecked(LBMarkerVisibilityManager.Get.showNoBuildZones);
		chckbx_centerPlayer.SetChecked(LBMarkerVisibilityManager.Get.centerMapOnPlayer);

		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		chckbx_ShowChatTag.Enable(pb.GetPermission() && pb.GetPermission().nextGroupUID == -1);
		chckbx_ShowChatTag.SetChecked(pb.GetLBGroup() && pb.GetLBGroup().showTagInChat);
	}

	void ReloadChatVisibility() {
#ifndef LB_DISABLE_CHAT
		comboboxChatVisibility.ClearAll();
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		LBLogger.Verbose("ReloadChatVisibility. Channels: " + mission.channels.Count(), "AdvancedGroups");
		foreach (ChannelCfg channel : mission.channels) {
			comboboxChatVisibility.AddItem(channel.channelName);
		}
		comboboxChatVisibility.SetCurrentItem(0);
		OnChannelSelected();
		if (sliderBorder)
			sliderBorder.SetCurrent(LBMarkerVisibilityManager.Get.chatBorderVisibility);
		panel_chat_settings.Show(true);
#else
		panel_chat_settings.Show(false);
#endif
	}

	void OnChannelSelected() {
#ifndef LB_DISABLE_CHAT
		int selected = comboboxChatVisibility.GetCurrentItem();
		if (selected < 0 || selected >= comboboxChatVisibility.GetNumItems())
			return;
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		string name = mission.channels.Get(selected).channelName;
		bool hidden = LBMarkerVisibilityManager.Get.IsChannelHidden(name);
		LBLogger.Verbose("OnChannelSelected: " + name + " Hidden ?" + hidden, "AdvancedGroups");
		if (hidden) {
			btnToggleChatVisibility.SetText("#lb_ag_btn_hidden");
			chatVisibilityMark.SetColor(COLOR_RED);
		} else {
			btnToggleChatVisibility.SetText("#lb_ag_btn_shown");
			chatVisibilityMark.SetColor(COLOR_GREEN);
		}
#endif
	}

	void OnChannelVisibilityChangeButtonClicked() {
#ifndef LB_DISABLE_CHAT
		int selected = comboboxChatVisibility.GetCurrentItem();
		if (selected < 0 || selected >= comboboxChatVisibility.GetNumItems())
			return;
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		string name = mission.channels.Get(selected).channelName;
		bool hidden = LBMarkerVisibilityManager.Get.IsChannelHidden(name);
		LBMarkerVisibilityManager.Get.SetChannelHidden(name, !hidden);
		OnChannelSelected();
#endif
	}

	void ReloadGPS() {
		if (comboboxGPSSize)
			comboboxGPSSize.SetCurrentItem(LBLayoutConfig.Get.gpsSizeIndex);
		if (sliderGPSZoom)
			sliderGPSZoom.SetCurrent(LBLayoutConfig.Get.gpsZoom);
		chckbx_GPS.SetChecked(LBMarkerVisibilityManager.Get.gpsEnabled);
	}

	void ReloadPlayerMarkerPos() {
		if (comboboxPlayerMarkerPos)
			comboboxPlayerMarkerPos.SetCurrentItem(LBLayoutConfig.Get.playerMarkerPosIndex);
	}

	void ReloadPlayerMarkerStyle() {
		int index = LBLayoutConfig.Get.playerMarkerStyleIndex;
		LBLogger.Debug("ReloadPlayerMarkerStyle. Index: " + index, "AdvancedGroups");
		if (checkMarkerStyleIcon)
			checkMarkerStyleIcon.SetChecked((index & 0x01) == 0);
		if (checkMarkerStyleName)
			checkMarkerStyleName.SetChecked((index & 0x02) == 0);
		if (checkMarkerStyleDistance)
			checkMarkerStyleDistance.SetChecked((index & 0x04) == 0);
	}

	void ReloadStyles() {
		if (comboPlayerlistStyleTemp && comboPlayerlistStyleTemp.GetNumItems() > 0)
			comboPlayerlistStyleTemp.SetCurrentItem(LBLayoutConfig.Get.playerlistLayoutIndex % comboPlayerlistStyleTemp.GetNumItems());
	}

	void ReloadColors(bool force = false) {
		TStringArray colorArray2 = LBColorManager.Get.GetColorStrings();
		if (force || colorArray2.Count() != colorArray.Count()) {
			availableColors.ClearItems();
			colorArray = colorArray2;
			foreach (string s : colorArray)
				availableColors.AddItem(s, null, 0);
			availableColors.SelectRow(0);
			availableColors.EnsureVisible(0);
		}
	}

	void ReloadPositions(bool force = false) {
		TStringArray positionsArray2 = LBPositionManager.Get.GetPositionStrings();
		if (force || positionsArray2.Count() != positionsArray.Count()) {
			availableLayoutsList.ClearItems();
			positionsArray = positionsArray2;
			foreach (string s : positionsArray)
				availableLayoutsList.AddItem(s, null, 0);
			availableLayoutsList.SelectRow(0);
			availableLayoutsList.EnsureVisible(0);
		}
	}

	void ReloadTextSizes() {
		comboboxChatSize.ClearAll();
		for (int i = CHAT_SIZE_START; i <= 25; i++) {
			comboboxChatSize.AddItem("" + i);
		}
		comboboxChatSize.SetCurrentItem(LBMarkerVisibilityManager.Get.GetChatSize() - CHAT_SIZE_START);
	}

	void ReloadPingSizes() {
		comboboxPingSize.ClearAll();
		for (int i = 10; i <= 30; i++) {
			comboboxPingSize.AddItem("" + i);
			i++;
		}
		int index = (LBMarkerVisibilityManager.Get.pingSize - 10) / 2;
		LBLogger.Debug("Ping Size: " + LBMarkerVisibilityManager.Get.pingSize + " -> Index: " + index, "AdvancedGroups");
		comboboxPingSize.SetCurrentItem(index);
	}

	void ReloadPlayerSizes() {
		comboboxPlayerSize.ClearAll();
		for (int i = 10; i <= 30; i++) {
			comboboxPlayerSize.AddItem("" + i);
			i++;
		}
		int index = (LBMarkerVisibilityManager.Get.playerSize - 10) / 2;
		LBLogger.Debug("Player Size: " + LBMarkerVisibilityManager.Get.playerSize + " -> Index: " + index, "AdvancedGroups");
		comboboxPlayerSize.SetCurrentItem(index);
	}

	void ReloadPingMarkers(bool force = false) {
		TStringArray pingIconsArray2 = LBMarkerVisibilityManager.Get.GetPingMarkerIcons();
		if (force || pingIconsArray.Count() != pingIconsArray2.Count()) {
			comboboxPingIcon.ClearAll();
			pingIconsArray = pingIconsArray2;
			foreach (string s : pingIconsArray2)
				comboboxPingIcon.AddItem("");
			comboboxPingIcon.SetCurrentItem(0);
			string currentIcon = LBMarkerVisibilityManager.Get.pingMarkerIcon;
			int index = pingIconsArray.Find(currentIcon);
			if (index >= 0)
				comboboxPingIcon.SetCurrentItem(index);
			PingIconChanged();
		}
	}

	void ReloadPlayerMarkers(bool force = false) {
		TStringArray playerIconsArray2 = LBMarkerVisibilityManager.Get.GetPlayerMarkerIcons();
		if (force || playerIconsArray.Count() != playerIconsArray2.Count()) {
			comboboxPlayerIcon.ClearAll();
			playerIconsArray = playerIconsArray2;
			foreach (string s : playerIconsArray2)
				comboboxPlayerIcon.AddItem("");
			comboboxPlayerIcon.SetCurrentItem(0);
			string currentIcon = LBMarkerVisibilityManager.Get.playerMarkerIcon;
			int index = playerIconsArray.Find(currentIcon);
			if (index >= 0)
				comboboxPlayerIcon.SetCurrentItem(index);
			PlayerIconChanged();
		}
	}

	void PingIconChanged() {
		string icon = pingIconsArray.Get(comboboxPingIcon.GetCurrentItem());
		LBMarkerVisibilityManager.Get.SetPingMarkerIcon(icon);
		imagePingIcon.LoadImageFile(0, icon);
		imagePingIcon.SetColor(LBColorManager.Get.GetColor("Ping 3D Marker"));
	}

	void PlayerIconChanged() {
		string icon = playerIconsArray.Get(comboboxPlayerIcon.GetCurrentItem());
		LBMarkerVisibilityManager.Get.SetPlayerMarkerIcon(icon);
		imagePlayerIcon.LoadImageFile(0, icon);
		imagePlayerIcon.SetColor(LBColorManager.Get.GetColor("Player 3D Marker"));
	}

	override void OnHide() {
		super.OnHide();
		LBColorManager_.InvokeOnChanged();
		LBPositionManager_.InvokeOnChanged();
		LBLayoutConfig_.InvokeGPSChanged();
	}

	override void OnUpdateFrame() {
		colorPreview.SetColor(GetCurrentColor());
	}

	void LoadColor(int color) {
		int a, r, g, b;
		LBConverter.ARGBToComponents(color, a, r, g, b);
		sliderA.SetCurrent(a);
		sliderR.SetCurrent(r);
		sliderG.SetCurrent(g);
		sliderB.SetCurrent(b);
		editA.SetText("" + ((int) sliderA.GetCurrent()));
		editR.SetText("" + ((int) sliderR.GetCurrent()));
		editG.SetText("" + ((int) sliderG.GetCurrent()));
		editB.SetText("" + ((int) sliderB.GetCurrent()));
	}

	void SetCurrentColor() {
		int selectedItem = availableColors.GetSelectedRow();
		if (selectedItem < 0) {
			LoadColor(-1);
			return;
		}
		LBColorManager.Get.SetColor(colorArray.Get(selectedItem), GetCurrentColor());
		imagePingIcon.SetColor(LBColorManager.Get.GetColor("Ping 3D Marker"));
		imagePlayerIcon.SetColor(LBColorManager.Get.GetColor("Player 3D Marker"));
	}

	void ResetToDefaultColor() {
		int selectedItem = availableColors.GetSelectedRow();
		if (selectedItem < 0) {
			LoadColor(-1);
			return;
		}
		string colorStr = colorArray.Get(selectedItem);
		LBColorManager.Get.ResetColorToDefault(colorStr);
		int color = LBColorManager.Get.GetColor(colorStr);
		LoadColor(color);
	}

	void ResetToDefaultPosition() {
		LBWidgetPosition position = GetCurrentPosition();
		if (!position) {
			return;
		}
		LBPositionManager.Get.ResetPositionToDefault(position.param1);
		LoadPosition();
		SetInvertBoxes(position.param3);
	}

	int GetCurrentColor() {
		int a = sliderA.GetCurrent();
		int r = sliderR.GetCurrent();
		int g = sliderG.GetCurrent();
		int b = sliderB.GetCurrent();
		return ARGB(a, r, g, b);
	}

	void LoadPosition() {
		LBWidgetPosition positionParam = GetCurrentPosition();
		if (!positionParam) {
			return;
		}
		vector pos = positionParam.param2;
		sliderX.SetCurrent(pos[0]);
		sliderY.SetCurrent(pos[1]);
		OnIconPreviewPositionChange();
	}

	void OnPingSizeChanged() {
		int size = comboboxPingSize.GetCurrentItem() * 2 + 10;
		LBLogger.Debug("Ping Size Index: " + comboboxPingSize.GetCurrentItem() + " -> " + size, "AdvancedGroups");
		LBMarkerVisibilityManager.Get.pingSize = size;
	}
	void OnPlayerSizeChanged() {
		int size = comboboxPlayerSize.GetCurrentItem() * 2 + 10;
		LBLogger.Debug("Player Size Index: " + comboboxPlayerSize.GetCurrentItem() + " -> " + size, "AdvancedGroups");
		LBMarkerVisibilityManager.Get.playerSize = size;
	}

#ifndef LB_DISABLE_CHAT
	void OnChatSizeChanged() {
		int size = comboboxChatSize.GetCurrentItem() + CHAT_SIZE_START;
		LBMarkerVisibilityManager.Get.chatSize = size;
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		if (mission && mission.m_Chat) {
			mission.m_Chat.ResetTextSizes();
		}
	}
#endif
	void OnIconPreviewPositionChange() {
		LBWidgetPosition positionParam = GetCurrentPosition();
		if (!positionParam) {
			return;
		}
		int index = positionParam.param3;
		float posX = sliderX.GetCurrent();
		float posY = sliderY.GetCurrent();
		vector pos = Vector(posX, posY, 0);
		LBWidgetUtils.SetWidgetPositionIndex(previewIcon, pos, index);
		LBWidgetUtils.SetWidgetAlignmentIndex(previewIcon, index);
		LBPositionManager.Get.SetPosition(positionsArray.Get(availableLayoutsList.GetSelectedRow()), pos);
	}

	void SetInvertBoxes(int index) {
		int value = LBWidgetUtils.FromIndex(index);
		invertX.SetChecked(!LBWidgetUtils.IsLeft(value));
		invertY.SetChecked(!LBWidgetUtils.IsTop(value));
	}

	void SaveInvertBoxes() {
		int index = 0;
		if (invertX.IsChecked())
			index += 2;
		if (invertY.IsChecked())
			index += 6;
		LBWidgetPosition positionParam = GetCurrentPosition();
		if (!positionParam) {
			return;
		}
		positionParam.param3 = index;
		OnIconPreviewPositionChange();
	}

	LBWidgetPosition GetCurrentPosition() {
		int selectedItem = availableLayoutsList.GetSelectedRow();
		if (selectedItem < 0) {
			return null;
		}
		return LBPositionManager.Get.positions.Get(selectedItem);
	}

	override bool OnClick(Widget w) {
		if (super.OnClick(w))
			return true;
		if (w == toDefault) {
			ResetToDefaultColor();
			return true;
		} else if (w == btn_ResetAll) {
			LBColorManager.Get.ResetAll();
			LBPositionManager.Get.ResetAll();
			LBLayoutConfig.Get.ResetAll();
			LBMarkerVisibilityManager.Get.ResetPingToDefault();
			ReloadAll(true);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(LBLayoutConfig_.InvokeOnLayoutChanged);
			NotificationSystem.AddNotificationExtended(4.0, "#lb_message_group_system", "#lb_message_settings_to_default", "set:ccgui_enforce image:MapUserMarker");
		} else if (w == btn_Save_Config) {
			LBColorManager.Loader.Save();
			LBPositionManager.Loader.Save();
			LBLayoutConfig.Loader.Save();
			LBMarkerVisibilityManager.Loader.Save();
			NotificationSystem.AddNotificationExtended(4.0, "#lb_message_group_system", "#lb_message_settings_saved", "set:ccgui_enforce image:MapUserMarker");
		} else if (w == btn_Reload_From_Config) {
			LBColorManager.Loader.Load();
			LBPositionManager.Loader.Load();
			LBLayoutConfig.Loader.Load();
			LBMarkerVisibilityManager.Loader.Load();
			ReloadAll(true);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(LBLayoutConfig_.InvokeOnLayoutChanged);
			NotificationSystem.AddNotificationExtended(4.0, "#lb_message_group_system", "#lb_message_settings_reloaded", "set:ccgui_enforce image:MapUserMarker");
		} else if (w == invertX || w == invertY) {
			if (w == invertX) {
				sliderX.SetCurrent(1.0 - sliderX.GetCurrent());
			} else {
				sliderY.SetCurrent(1.0 - sliderY.GetCurrent());
			}
			SaveInvertBoxes();
			LoadPosition();
			return true;
		} else if (w == toDefaultPosition) {
			ResetToDefaultPosition();
			return true;
		} else if (w == toDefaultStyle) {

		} else if (w == chckbx_Playerlist) {
			LBMarkerVisibilityManager.Get.playerlistEnabled = !LBMarkerVisibilityManager.Get.playerlistEnabled;
		} else if (w == chckbx_Compass) {
			LBMarkerVisibilityManager.Get.compassEnabled = !LBMarkerVisibilityManager.Get.compassEnabled;
		} else if (w == chckbx_ShowClantextures) {
			LBMarkerVisibilityManager.Get.disableShowClantextures = !chckbx_ShowClantextures.IsChecked();
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(!LBMarkerVisibilityManager.Get.disableShowClantextures);
			rpc.Send(null, LBGroupRPCs.CLAN_CLOTHING_UPDATE, true);
		} else if (w == chckbx_ShowChatTag) {
			bool checked = chckbx_ShowChatTag.IsChecked();
			PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
			if (pb && pb.GetLBGroup()) {
				pb.GetLBGroup().SendChatTagVisibilityRequest(checked);
			}
		} else if (w == comboPlayerlistStyleTemp) {
			LBLayoutConfig.Get.SetPlayerlistLayout(comboPlayerlistStyleTemp.GetCurrentItem());
		} else if (w == chckbx_StreamerMode) {
			LBLayoutConfig.Get.SetStreamerMode(chckbx_StreamerMode.IsChecked());
		} else if (w == chckbx_GPS) {
			LBMarkerVisibilityManager.Get.gpsEnabled = chckbx_GPS.IsChecked();
		} else if (w == chckbx_show_no_build) {
			LBMarkerVisibilityManager.Get.showNoBuildZones = chckbx_show_no_build.IsChecked();
		} else if (w == chckbx_centerPlayer) {
			LBMarkerVisibilityManager.Get.centerMapOnPlayer = chckbx_centerPlayer.IsChecked();
		} else if (w == comboboxChatVisibility) {
			OnChannelSelected();
		} else if (w == btnToggleChatVisibility) {
			OnChannelVisibilityChangeButtonClicked();
		}
		return false;
	}

	override bool OnItemSelected(Widget w, int row, int column) {
		if (super.OnItemSelected(w, row, column))
			return true;
		if (w == availableColors) {
			int selectedItem = availableColors.GetSelectedRow();
			if (selectedItem < 0) {
				LoadColor(-1);
				return true;
			}
			Param2<string, int> colorParam = LBColorManager.Get.colors.Get(selectedItem);
			if (!colorParam) {
				LoadColor(-1);
				return true;
			}
			LoadColor(colorParam.param2);
			return true;
		} else if (w == availableLayoutsList) {
			LBWidgetPosition positionParam = GetCurrentPosition();
			if (!positionParam) {
				SetInvertBoxes(0);
				LoadPosition();
				return true;
			}
			SetInvertBoxes(positionParam.param3);
			LoadPosition();
			return true;
		} else if (w == availableLayoutsListStyles) {

		}
		return false;
	}

	override bool OnChange(Widget w) {
		if (super.OnChange(w))
			return true;
		bool changed = false;
		if (w == sliderA) {
			int a = sliderA.GetCurrent();
			editA.SetText(a.ToString());
			changed = true;
		} else if (w == sliderR) {
			int r = sliderR.GetCurrent();
			editR.SetText(r.ToString());
			changed = true;
		} else if (w == sliderG) {
			int g = sliderG.GetCurrent();
			editG.SetText(g.ToString());
			changed = true;
		} else if (w == sliderB) {
			int b = sliderB.GetCurrent();
			editB.SetText(b.ToString());
			changed = true;
		} else if (w == editA) {
			int txtInt = Math.Clamp(editA.GetText().ToInt(), 0, 255);
			sliderA.SetCurrent(txtInt);
			editA.SetText(txtInt.ToString());
			changed = true;
		} else if (w == editR) {
			txtInt = Math.Clamp(editR.GetText().ToInt(), 0, 255);
			sliderR.SetCurrent(txtInt);
			editR.SetText(txtInt.ToString());
			changed = true;
		} else if (w == editG) {
			txtInt = Math.Clamp(editG.GetText().ToInt(), 0, 255);
			sliderG.SetCurrent(txtInt);
			editG.SetText(txtInt.ToString());
			changed = true;
		} else if (w == editB) {
			txtInt = Math.Clamp(editB.GetText().ToInt(), 0, 255);
			sliderB.SetCurrent(txtInt);
			editB.SetText(txtInt.ToString());
			changed = true;
		} else if (w == sliderY || w == sliderX) {
			OnIconPreviewPositionChange();
		} else if (w == comboBoxStyle) {

		} else if (w == comboboxPingIcon) {
			PingIconChanged();
		} else if (w == comboboxPlayerIcon) {
			PlayerIconChanged();
		} else if (w == comboboxChatSize) {
#ifndef LB_DISABLE_CHAT
			OnChatSizeChanged();
#endif
		} else if (w == comboboxPingSize) {
			OnPingSizeChanged();
		} else if (w == comboboxPlayerSize) {
			OnPlayerSizeChanged();
		} else if (w == comboboxGPSSize) {
			int index = comboboxGPSSize.GetCurrentItem();
			LBLayoutConfig.Get.gpsSizeIndex = index;
			LBLogger.Debug("Changing GPS Size to " + index, "AdvancedGroups");
			return true;
		} else if (w == comboboxPlayerMarkerPos) {
			index = comboboxPlayerMarkerPos.GetCurrentItem();
			LBLayoutConfig.Get.playerMarkerPosIndex = index;
			LBLogger.Debug("Changing Player Marker Pos to " + index, "AdvancedGroups");
			return true;
		} else if (w == checkMarkerStyleIcon) {
			if (checkMarkerStyleIcon.IsChecked())
				LBLayoutConfig.Get.playerMarkerStyleIndex = ~(~LBLayoutConfig.Get.playerMarkerStyleIndex | 0x01);
			else
				LBLayoutConfig.Get.playerMarkerStyleIndex = (LBLayoutConfig.Get.playerMarkerStyleIndex | 0x01);
			LBLogger.Debug("Changing Player Marker Style to " + LBLayoutConfig.Get.playerMarkerStyleIndex, "AdvancedGroups");
			return true;
		} else if (w == checkMarkerStyleName) {
			if (checkMarkerStyleName.IsChecked())
				LBLayoutConfig.Get.playerMarkerStyleIndex = ~(~LBLayoutConfig.Get.playerMarkerStyleIndex | 0x02);
			else
				LBLayoutConfig.Get.playerMarkerStyleIndex = (LBLayoutConfig.Get.playerMarkerStyleIndex | 0x02);
			LBLogger.Debug("Changing Player Marker Style to " + LBLayoutConfig.Get.playerMarkerStyleIndex, "AdvancedGroups");
			return true;
		} else if (w == checkMarkerStyleDistance) {
			if (checkMarkerStyleDistance.IsChecked())
				LBLayoutConfig.Get.playerMarkerStyleIndex = ~(~LBLayoutConfig.Get.playerMarkerStyleIndex | 0x04);
			else
				LBLayoutConfig.Get.playerMarkerStyleIndex = (LBLayoutConfig.Get.playerMarkerStyleIndex | 0x04);
			LBLogger.Debug("Changing Player Marker Style to " + LBLayoutConfig.Get.playerMarkerStyleIndex, "AdvancedGroups");
			return true;
		} else if (w == sliderGPSZoom) {
			float zoom = sliderGPSZoom.GetCurrent();
			LBLayoutConfig.Get.gpsZoom = zoom;
			LBLogger.Debug("Changing GPS Zoom to: " + zoom, "AdvancedGroups");
			return true;
		} else if (w == sliderBorder) {
			LBMarkerVisibilityManager.Get.chatBorderVisibility = sliderBorder.GetCurrent();
#ifndef LB_DISABLE_CHAT
			OnChatSizeChanged();
#endif
		}
		if (changed) {
			SetCurrentColor();
			return true;
		}
		return false;
	}

}
