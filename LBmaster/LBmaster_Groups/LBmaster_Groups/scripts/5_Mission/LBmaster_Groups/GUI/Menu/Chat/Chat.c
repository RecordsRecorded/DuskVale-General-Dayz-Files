#ifndef LB_DISABLE_CHAT
modded class Chat {

	const int LINE_COUNT = 100;
	const int VISIBLE_LINES_COUNT = 12;

	bool addedPrefix = false;
	float prefixLength = 0;

	bool added = false;
	ScrollWidget rootPositionWidget = null;
	Widget lb_chat_root, lb_chat_root_parent, lb_original_root;

	int addedMessages = 1;
	private string nextPrefix, nextGroupPrefix;

	void Chat() {
		LBWidgetUtils.Event_OnScreenSizeChanged.Insert(ReinitializeLater);
	}

	void ~Chat() {
		if (LBWidgetUtils.Event_OnScreenSizeChanged)
			LBWidgetUtils.Event_OnScreenSizeChanged.Remove(ReinitializeLater);
		if (LBPositionManager_.Event_OnPositionChange)
			LBPositionManager_.Event_OnPositionChange.Remove(UpdatePosition);
	}

	override void Init(Widget root_widget) {
		lb_original_root = root_widget;
		Destroy();
		LBPositionManager_.Event_OnPositionChange.Remove(UpdatePosition);
		lb_chat_root = LBLayoutManager.Get().CreateLayout("Chat");
		lb_chat_root_parent = lb_chat_root.GetParent();
		rootPositionWidget = ScrollWidget.Cast(lb_chat_root.FindAnyWidget("RootPosition"));
		m_RootWidget = lb_chat_root.FindAnyWidget("ChatFrameWidget");
		if (m_RootWidget) {
			float scollMultiplier = ((float) LINE_COUNT) / VISIBLE_LINES_COUNT;
			m_RootWidget.SetSize(1.0, scollMultiplier);
			float w, h;
			m_RootWidget.Update();
			rootPositionWidget.Update();
			rootPositionWidget.GetSize(w, h);
			m_LineHeight = h / VISIBLE_LINES_COUNT;
			m_LastLine = 0;

			for (int i = 0; i < LINE_COUNT; i++) {
				ChatLine line = new ChatLine(m_RootWidget);
				m_Lines.Insert(line);
			}
		}
		OnChatMenuOpened();
		OnChatMenuClosed();
		LBPositionManager_.Event_OnPositionChange.Remove(UpdatePosition);
		LBPositionManager_.Event_OnPositionChange.Insert(UpdatePosition);
		UpdatePosition();
	}

	void ReinitializeLater() {
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Reinitialize, 100);
	}

	void Reinitialize() {
		float w;
		float h;
		rootPositionWidget.Update();
		rootPositionWidget.GetSize(w, h);
		m_LineHeight = h / VISIBLE_LINES_COUNT;
		for (int i = 0; i < m_Lines.Count(); i++) {
			ChatLine line = m_Lines.Get((m_LastLine + 1 + i) % LINE_COUNT);
			line.m_RootWidget.SetPos(0, i * m_LineHeight);
		}
		//Init(null); // TODO Try to store previous chat messages and restore them afterwards
	}

	void OnChatMenuOpened() {
		foreach (ChatLine line : m_Lines) {
			line.OnChatMenuOpened();
		}
		rootPositionWidget.SetColor(ARGB(255, 255, 255, 255));
		ResetChatScroll();
	}

	void OnChatMenuClosed() {
		foreach (ChatLine line : m_Lines) {
			line.OnChatMenuClosed();
		}
		ResetChatScroll();
		rootPositionWidget.SetColor(ARGB(0, 0, 0, 0));
	}

	void ClearChatLines() {
		ChatMessageEventParams param = new ChatMessageEventParams(0, "", "", "");
		foreach (ChatLine line : m_Lines) {
			line.Set(param);
			line.m_NameTag.SetText("");
			line.m_GroupTag.SetText("");
		}
		addedMessages = 1;
	}

	void AddLBChat(int channel, string channelName, string name, string message, string extra, string prefix, int prefixColor, string groupPrefix, int channelColor, int groupPrefixColor, bool changeGroupTagColor) {
		if (channel & CCDirect) {
			MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
			channelName = mission.GetDirectChannelName();
		}
		if (LBMarkerVisibilityManager.Get.IsChannelHidden(channelName))
			return;

		SetNextPrefix(prefix, prefixColor, groupPrefix, channelColor, groupPrefixColor, changeGroupTagColor);
		addedPrefix = true;
		LBTextLengthCalculator calc = LBTextLengthCalculator.Get();
		int size = LBMarkerVisibilityManager.Get.GetChatSize();
		prefixLength = calc.GetTextLength(groupPrefix + " " + prefix, size);
		Add(new ChatMessageEventParams(channel, name, message, extra));
		if (added) {
			prefixLength = 0;
			addedPrefix = false;
			ChatLine.currentColor = ARGB(255, 255, 255, 255);
		} else {
			SetNextPrefix("", 0, "", 0, 0, false);
		}
	}

	void UpdatePosition() {
		bool atDefault = LBPositionManager.Get.IsAtDefaultPos("Chat");
		if (!atDefault && lb_chat_root && lb_chat_root.GetParent() == lb_original_root) {
			lb_chat_root_parent.AddChild(lb_chat_root);
		} else if (atDefault && lb_chat_root && lb_chat_root.GetParent() != lb_original_root) {
			lb_original_root.AddChild(lb_chat_root);
		}
		vector pos = LBPositionManager.Get.GetPosition("Chat");
		int index = LBPositionManager.Get.GetIndex("Chat");
		LBWidgetUtils.SetWidgetAlignmentIndex(rootPositionWidget, index);
		LBWidgetUtils.SetWidgetPositionIndex(rootPositionWidget, pos, index);
	}

	void UpdateChatVisibility() {
		IngameHud hud = IngameHud.Cast(g_Game.GetMission().GetHud());
		if (hud && rootPositionWidget) {
			bool visible = false;
			if (hud.LBIsHudVisible() && LBPlayerUtils.IsClientPlayerAlive())
				visible = true;
			rootPositionWidget.Show(visible);
		}
	}

	override void Add(ChatMessageEventParams params) {
		added = false;
		string message = params.param3;
		if (message.Length() == 0)
			return;
		if (message[0] == "+" || message[0] == "!")
			return;
		if (message[0] == "2") {
			params.param3 = message.Substring(1, message.Length() - 1);
		}
		int channel = params.param1;
		LBLogger.Debug("Received Chat Message in Channel " + channel, "AdvancedGroups");
		LBLogger.Debug("CCSystem=" + CCSystem + " CCBattlEye=" + CCBattlEye + " CCAdmin=" + CCAdmin + " CCDirect=" + CCDirect + " CCMegaphone=" + CCMegaphone + " CCTransmitter=" + CCTransmitter + " CCPublicAddressSystem=" + CCPublicAddressSystem, "AdvancedGroups");

		ChatLine.battleyeColor = MissionGameplay.battleyeChatColor.GetColorARGB();
		int count = ("" + message).Replace(":", "");
		if ((channel & CCBattlEye) && (message.Length() >= 10) && (count >= 4) && (message[0] == "R") && (message[1] == "G") && (message[2] == "B")) {
			int first = message.IndexOf(":") + 1;
			int second = message.IndexOfFrom(first, ":") + 1;
			int third = message.IndexOfFrom(second, ":") + 1;
			int fourth = message.IndexOfFrom(third, ":") + 1;
			//LBLogger.Debug("Message: " + message + "  " + first + " " + second + " " + third + " " + fourth, "AdvancedGroups");
			int r = message.Substring(first, second - first - 1).ToInt();
			int g = message.Substring(second, third - second - 1).ToInt();
			int b = message.Substring(third, fourth - third - 1).ToInt();
			//LBLogger.Debug("" + r + " " + g + " " + b + " ARGB:" + ARGB(255,r,g,b), "AdvancedGroups");
			ChatLine.battleyeColor = ARGB(255, r, g, b);
			message = message.Substring(fourth, message.Length() - fourth);
			params.param3 = message;
			//LBLogger.Debug("Color: " + ChatLine.battleyeColor + " Message: " + params.param3, "AdvancedGroups");
		}

		LBTextLengthCalculator calc = LBTextLengthCalculator.Get();

		int size = LBMarkerVisibilityManager.Get.GetChatSize();
		float name_lenght = calc.GetTextLength(params.param2, size);
		float text_lenght = calc.GetTextLength(params.param3, size);
		float total_lenght = text_lenght + name_lenght + prefixLength;

		if (channel & CCSystem || channel & CCBattlEye) { //TODO separate battleye bellow
			if (g_Game.GetProfileOption(EDayZProfilesOptions.GAME_MESSAGES))
				return;
		}
		//TODO add battleye filter to options
		/*else if( channel & CCBattlEye )
		{
			if( g_Game.GetProfileOption( EDayZProfilesOptions.BATTLEYE_MESSAGES ) )
				return;
		}*/
		else if (channel & CCAdmin) {
			if (g_Game.GetProfileOption(EDayZProfilesOptions.ADMIN_MESSAGES))
				return;
		} else if (channel == 0 || channel & CCDirect || channel & CCMegaphone || channel & CCTransmitter || channel & CCPublicAddressSystem) {
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
				return;
			MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
			if (LBMarkerVisibilityManager.Get.IsChannelHidden(mission.GetDirectChannelName()))
				return;
		} else if (channel == 4096) {
			if (g_Game.GetProfileOption(EDayZProfilesOptions.PLAYER_MESSAGES))
				return;
		}
		bool firstLine = true;
		float max_length = 0.4;
		if (total_lenght > max_length) {
			string preMessage = "";
			if (addedPrefix) {
				preMessage = nextPrefix + " " + nextGroupPrefix + " ";
			}
			preMessage = preMessage + params.param2;
			float preLength = calc.GetTextLength(preMessage, size);
			TStringArray parts = new TStringArray();
			params.param3.Split(" ", parts);
			int i = 0;

			ChatMessageEventParams tmp = new ChatMessageEventParams(params.param1, params.param2, "", params.param4);

			while (i < parts.Count()) {
				string mess = "";
				while (calc.GetTextLength(mess + " " + parts[i], size) + preLength < max_length && i < parts.Count()) {
					mess = mess + " " + parts[i];
					i++;
				}
				if (mess.Length() <= 0) {
					string part = parts[i];
					int l = 1;
					for (int a = 0; a < part.Length(); a++) {
						if (calc.GetTextLength(part.Substring(0, a + 1), size) + preLength >= max_length) {
							l = a;
							break;
						}
					}
					mess = part.Substring(0, l);
					parts[i] = part.Substring(l, part.Length() - l);
				}
				tmp.param3 = mess;
				preLength = 0;

				if (!addedPrefix)
					SetNextPrefix("", 0, "", ARGB(255, 255, 255, 255), 0, false);
				else if (!firstLine)
					SetNextPrefix("", 0, "", ChatLine.currentColor, 0, false);
				firstLine = false;
				AddInternal(tmp);
				added = true;

				tmp.param2 = "";
			}
		} else {
			if (!addedPrefix) {
				SetNextPrefix("", 0, "", ARGB(255, 255, 255, 255), 0, false);
			}
			AddInternal(params);
			added = true;
		}
		addedPrefix = false;
	}

	void OnMouseWheel(int count) {
		float max = Math.Clamp((addedMessages) / LINE_COUNT, 0, 1.0);
		rootPositionWidget.VScrollStep(count);
		float pos = 1.0 - rootPositionWidget.GetVScrollPos01();
		if (pos > max)
			rootPositionWidget.VScrollToPos01(1.0 - max);
	}

	void ResetChatScroll() {
		rootPositionWidget.VScrollToPos01(1.0);
	}

	void ResetTextSizes() {
		int size = LBMarkerVisibilityManager.Get.GetChatSize();
		foreach (ChatLine line : m_Lines) {
			line.m_NameTag.SetTextExactSize(size);
			line.m_NameWidget.SetTextExactSize(size);
			line.m_TextWidget.SetTextExactSize(size);
			line.m_GroupTag.SetTextExactSize(size);
			line.SetBackground();
		}
	}

	override void AddInternal(ChatMessageEventParams params) {
		m_LastLine = (m_LastLine + 1) % m_Lines.Count();

		ChatLine line = m_Lines.Get(m_LastLine);
		line.Set(params);

		for (int i = 0; i < m_Lines.Count(); i++) {
			line = m_Lines.Get((m_LastLine + 1 + i) % LINE_COUNT);
			line.m_RootWidget.SetPos(0, i * m_LineHeight);
		}
		++addedMessages;
	}

	void SetNextPrefix(string prefix, int prefixColor, string groupPrefix, int channelColor, int groupPrefixColor, bool changeGroupTagColor) {
		ChatLine line = m_Lines.Get((m_LastLine + 1) % m_Lines.Count());
		int size = LBMarkerVisibilityManager.Get.GetChatSize();
		line.m_NameTag.SetTextExactSize(size);
		line.m_NameTag.SetColor(prefixColor);
		line.m_NameTag.SetText(prefix);
		line.m_GroupTag.SetText(groupPrefix);
		if (changeGroupTagColor)
			line.m_GroupTag.SetColor(groupPrefixColor);
		line.changeGroupTagColor = changeGroupTagColor;
		ChatLine.currentColor = channelColor;

		nextPrefix = prefix;
		nextGroupPrefix = groupPrefix;
	}

}
#endif
