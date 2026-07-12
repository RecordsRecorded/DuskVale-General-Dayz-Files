#ifndef LB_DISABLE_CHAT
modded class ChatInputMenu {

	Widget slowPanel;
	TextWidget slow_time;

	static ChatInputMenu last_menu;
	ref ChatEventHandler eventHandler;
	
	override Widget Init() {
		last_menu = this;
		Widget old = super.Init(); // Call super to init "m_BackInputWrapper" which causes many nullpointers otherwise
		old.Unlink(); // Immediately destroy the Widget, which has been created using super.Init();
		LBLogger.Debug("Init Chat Menu ", "AdvancedGroups");
		layoutRoot = LBLayoutManager.Get().CreateLayout("ChatInput");
		m_edit_box = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("InputEditBoxWidget"));

		slow_time = TextWidget.Cast(layoutRoot.FindAnyWidget("slow_time"));
		slowPanel = layoutRoot.FindAnyWidget("slowPanel");

		UpdateChannel();
		LBLogger.Debug("Finished Init Chat Menu", "AdvancedGroups");
		return layoutRoot;
	}
	void ChatInputMenu() {
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(InitEventHandler);
	}

	EditBoxWidget GetEditBox() {
		return m_edit_box;
	}

	void InitEventHandler() {
		if (!eventHandler) {
			eventHandler = new ChatEventHandler(this);
		}
	}

	bool OnChangeLB(Widget w, int x, int y, bool finished) {
		UpdateChannel();
		if (UIScriptedWindow.GetActiveWindows()) {
			for (int i = 0; i < UIScriptedWindow.GetActiveWindows().Count(); i++) {
				if (UIScriptedWindow.GetActiveWindows().GetElement(i).OnChange(w, x, y, finished))
					return true;
			}
		}
		if (!finished)
			return false;

		string text = m_edit_box.GetText();
		if (text.Length() > 200)
			text = text.Substring(0, 200);
		if (text != "") {
			bool command = false;
			if (text[0] == "!")
				command = true;
			if (text[0] == "2" || text[0] == "+")
				text = "2" + text;
			MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
			ChannelCfg channel = mission.GetCurrentChannel();
			if (channel && !command) {
				int slowRemain = GetSlowModeTimeClient(channel, mission);
				if (slowRemain > 0 && !LBAdmins.Get().HasPermissionActive("groups.ignore.chatslowmode"))
					return true;
				mission.lastSentClient.Set(channel.channelName, g_Game.GetTime() / 1000);
			}
			mission.SendChatMessage(text);
		}

		m_close_timer.Run(0.1, this, "Close");

		GetUApi().GetInputByName("UAPersonView").Supress();

		return true;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel) {
		if (super.OnMouseWheel(w, x, y, wheel))
			return true;
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		mission.m_Chat.OnMouseWheel(wheel);
		return true;
	}

	override void OnShow() {
		super.OnShow();
		g_Game.GetUIManager().ShowUICursor(true);
		UpdateChannel();
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		mission.DisplayVoiceLevels(false);
		// Thanks to @DaOne and his VPPAdminTools for breaking my mod and having to make a workaround to get it working again.
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateChannel, 20, true);
		mission.m_Chat.OnChatMenuOpened();
	}

	override void OnHide() {
		super.OnHide();
		g_Game.GetUIManager().ShowUICursor(false);
		UpdateChannel();
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		mission.DisplayVoiceLevels(true);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(UpdateChannel);
		mission.m_Chat.OnChatMenuClosed();
	}

	override void UpdateChannel() {
		TextWidget channel_text = TextWidget.Cast(layoutRoot.FindAnyWidget("ChannelText"));
		if (!channel_text)
			return;
		channel_text.Show(m_edit_box.GetText().Length() == 0);
		ChannelCfg channel = null;
		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		if (mission)
			channel = mission.GetCurrentChannel();
		if (channel) {
			channel_text.SetText(channel.channelName);
			if (slowPanel) {
				if (!LBAdmins.Get().HasPermissionActive("groups.ignore.chatslowmode")) {
					int slowRemain = GetSlowModeTimeClient(channel, mission);
					slowPanel.Show(slowRemain > 0);
					slow_time.SetText(LBDate.Init(slowRemain).ToDiffString());
				} else {
					slowPanel.Show(false);
				}
			}
		} else {
			channel_text.SetText("#lb_ag_unknown");
			if (slowPanel)
				slowPanel.Show(false);
		}
	}

	int GetSlowModeTimeClient(ChannelCfg channel, MissionGameplay mission) {
		int last = -1000000;
		if (!mission.lastSentClient.Find(channel.channelName, last))
			last = -10000000;
		if (channel.slowModeSecondsBetweenMessages <= 0)
			return -1;
		int now = g_Game.GetTime() / 1000;
		int remain = channel.slowModeSecondsBetweenMessages + last - now;
		if (remain < 0)
			return 0;
		return remain;
	}

}
#endif