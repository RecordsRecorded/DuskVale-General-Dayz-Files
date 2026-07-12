#ifndef LB_DISABLE_CHAT
modded class ChatLine {

	TextWidget m_NameTag;
	TextWidget m_GroupTag;

	static int currentColor;
	static int battleyeColor;
	bool changeGroupTagColor = false;
	bool wasShown = false, fadeRunning = false, timeoutRunning = false, chatOpen = false, wasSet = false;
	int openTime = 0;

	void ChatLine(Widget root_widget) {
		if (m_RootWidget)
			m_RootWidget.Unlink();
		m_RootWidget	= LBLayoutManager.Get().CreateLayout("ChatItem", root_widget);

		m_NameTag	= TextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemSenderTagWidget"));
		m_GroupTag	= TextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemSenderGroupTagWidget"));
		m_NameWidget	= TextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemSenderWidget"));
		m_TextWidget	= TextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemTextWidget"));
	}

	void OnChatMenuOpened() {
		fadeRunning = m_FadeTimer.IsRunning();
		m_FadeTimer.Pause();
		timeoutRunning = m_TimeoutTimer.IsRunning();
		m_TimeoutTimer.Pause();
		wasShown = m_RootWidget.IsVisible();
		m_RootWidget.Show(true);
		openTime = g_Game.GetTime();
		chatOpen = true;
	}

	void OnChatMenuClosed() {
		if (fadeRunning)
			m_FadeTimer.Continue();
		if (timeoutRunning)
			m_TimeoutTimer.Continue();
		float time = (g_Game.GetTime() - openTime) / 1000;
		m_RootWidget.Show(wasShown);
		//btn_more.Show(false);
		chatOpen = false;
		m_TimeoutTimer.Tick(time);
		m_FadeTimer.Tick(time);
	}

	override void Set(ChatMessageEventParams params) {
		super.Set(params);
		if (chatOpen) {
			timeoutRunning = m_TimeoutTimer.IsRunning();
			m_TimeoutTimer.Pause();
		}
		openTime = g_Game.GetTime();
		wasShown = true;
		int channel = params.param1;
		//LBLogger.Debug("Set Chat Line: " + ChatLine.battleyeColor + " " + params.param3, "AdvancedGroups");
		if (channel & CCSystem) {
			if (params.param2 != "") {
				m_NameWidget.SetText("");
			}
			SetColour(GAME_TEXT_COLOUR);
		}
		if (channel & CCAdmin) {
			if (params.param2 != "") {
				m_NameWidget.SetText("");
			}
			SetColour(ADMIN_TEXT_COLOUR);
		}

		if (channel & CCBattlEye) {
			if (params.param2 != "") {
				m_NameWidget.SetText("");
			}
			SetColour(battleyeColor);
		}
		if (channel & CCDirect || channel == 0) {
			foreach (ChannelCfg cfg : ChatConfig.Get.channels) {
				if (cfg.directChannel) {
					SetColour(cfg.channelColor.GetColorARGB());
					break;
				}
			}
		}

		if (channel == 4096) {
			if (params.param2.Length() > 0)
				m_NameWidget.SetText(params.param2 + ": ");
			m_TextWidget.SetText(params.param3);
			SetColour(currentColor);
		}
		int size = LBMarkerVisibilityManager.Get.GetChatSize();
		m_NameWidget.SetTextExactSize(size);
		m_TextWidget.SetTextExactSize(size);
		m_GroupTag.SetTextExactSize(size);
	}

	void SetBackground() {
		array<TextWidget> widgets = {m_GroupTag, m_NameTag, m_NameWidget, m_TextWidget};
		float intensity = LBMarkerVisibilityManager.Get.GetChatBorderVisibility();
		foreach (TextWidget w : widgets) {
			w.SetShadow(w.GetShadowSize(), w.GetShadowColor(), intensity);
			w.SetOutline(w.GetOutlineSize(), ARGB(255 * intensity, 0, 0, 0));
		}
	}

	override void SetColour(int colour) {
		super.SetColour(colour);
		if (!changeGroupTagColor)
			m_GroupTag.SetColor(colour);
	}
}
#endif