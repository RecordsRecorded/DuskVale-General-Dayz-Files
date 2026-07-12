class MultiplixTerritoryComputerMenu extends UIScriptedMenu
{
	static const int MAX_VISIBLE_MEMBERS = 15;
	static const int MIN_SCROLL_PREVIEW_SLOTS = 30;
	static const int LAYOUT_DEFINED_MEMBER_BUTTONS = 8;
	static const int PERM_ROW_COUNT = 9;

	protected const int COLOR_TRANSPARENT       = ARGB(0, 0, 0, 0);
	protected const int COLOR_WINDOW            = ARGB(170, 4, 16, 4);
	protected const int COLOR_HEADER            = ARGB(220, 6, 22, 6);
	protected const int COLOR_SECTION           = ARGB(120, 3, 14, 3);
	protected const int COLOR_NODE_BG           = ARGB(255, 4, 22, 4);
	protected const int COLOR_NODE_HOVER_BG     = ARGB(255, 12, 56, 12);
	protected const int COLOR_NODE_SELECTED_BG  = ARGB(255, 28, 96, 28);
	protected const int COLOR_OWNER_BG          = ARGB(255, 30, 18, 0);
	protected const int COLOR_OWNER_HOVER_BG    = ARGB(255, 52, 32, 0);
	protected const int COLOR_OWNER_SELECTED_BG = ARGB(255, 96, 60, 8);
	protected const int COLOR_BUTTON            = ARGB(235, 8, 24, 8);
	protected const int COLOR_BUTTON_HOVER      = ARGB(255, 36, 96, 36);
	protected const int COLOR_BUTTON_DISABLED   = ARGB(160, 12, 12, 12);
	protected const int COLOR_DESTRUCTIVE       = ARGB(255, 80, 10, 10);
	protected const int COLOR_DESTRUCTIVE_HOVER = ARGB(255, 160, 30, 30);
	protected const int COLOR_CONNECTOR         = ARGB(255, 90, 170, 95);

	protected const int COLOR_TEXT              = ARGB(255, 112, 208, 120);
	protected const int COLOR_TEXT_BRIGHT       = ARGB(255, 190, 255, 196);
	protected const int COLOR_TEXT_DIM          = ARGB(200, 80, 140, 80);
	protected const int COLOR_TEXT_DISABLED     = ARGB(140, 80, 110, 80);
	protected const int COLOR_TEXT_AMBER        = ARGB(255, 255, 191, 60);
	protected const int COLOR_TEXT_AMBER_DIM    = ARGB(255, 200, 150, 50);
	protected const int COLOR_TEXT_RED          = ARGB(255, 255, 100, 100);
	protected const int COLOR_TEXT_RED_BRIGHT   = ARGB(255, 255, 180, 180);
	protected const int COLOR_HIGHLIGHT_AMBER   = ARGB(70, 220, 150, 30);
	protected const int COLOR_HIGHLIGHT_GREEN   = ARGB(60, 60, 200, 80);
	protected const int COLOR_VITAL_GREEN   = ARGB(255, 130, 230, 130);
	protected const int COLOR_VITAL_YELLOW  = ARGB(255, 245, 225,  90);
	protected const int COLOR_VITAL_ORANGE  = ARGB(255, 255, 165,  50);
	protected const int COLOR_VITAL_RED     = ARGB(255, 255, 110, 110);

	protected const string MENU_BACKGROUND_IMAGE = "ElysianTerritoriesPlus/images/MenuBackground.paa";
	protected const float CURSOR_BLINK_INTERVAL  = 0.5;
	protected const float REFRESH_INTERVAL       = 1.5;
	protected const int CONFIRM_TIMEOUT_MS       = 5000;

	protected Widget m_Root;
	protected Widget m_MenuRoot;

	protected ImageWidget m_MenuPanelBg;
	protected ImageWidget m_BezelPanelBg;
	protected ImageWidget m_ScreenBackgroundBg;
	protected ImageWidget m_StatusBarPanelBg;
	protected ImageWidget m_OrgPanelBg;
	protected ImageWidget m_DetailPanelBg;
	protected ImageWidget m_CommandPanelBg;
	protected ImageWidget m_OwnerNodeBgImg;
	protected ref array<ImageWidget> m_MemberNodeBgImgs;
	protected ref array<ImageWidget> m_PermRowBgImgs;
	protected ImageWidget m_KickButtonBgImg;
	protected ImageWidget m_ClaimButtonBgImg;
	protected ImageWidget m_InviteButtonBgImg;
	protected ImageWidget m_ResetMembersButtonBgImg;
	protected ImageWidget m_JoinLeaveButtonBgImg;
	protected ImageWidget m_EjectFloppyButtonBgImg;
	protected ImageWidget m_ResetAdminButtonBgImg;
	protected ImageWidget m_CloseButtonBgImg;

	protected Widget m_MenuPanel;
	protected Widget m_BezelPanel;
	protected Widget m_ScreenBackground;
	protected Widget m_StatusBarPanel;
	protected Widget m_OrgPanel;
	protected Widget m_DetailPanel;
	protected Widget m_CommandPanel;
	protected Widget m_OwnerNodeBg;
	protected Widget m_KickButtonBg;
	protected Widget m_ClaimButtonBg;
	protected Widget m_InviteButtonBg;
	protected Widget m_ResetMembersButtonBg;
	protected Widget m_JoinLeaveButtonBg;
	protected Widget m_EjectFloppyButtonBg;
	protected Widget m_ResetAdminButtonBg;
	protected Widget m_CloseButtonBg;
	protected ref array<Widget> m_MemberBgs;
	protected ref array<Widget> m_PermBgs;

	protected TextWidget m_TitleText;
	protected TextWidget m_FloppyStatusText;
	protected TextWidget m_StatusText;
	protected TextWidget m_OwnerHeaderText;
	protected TextWidget m_InviteText;
	protected TextWidget m_MemberCountText;
	protected TextWidget m_FloppyLifeText;
	protected TextWidget m_LoggedInAsText;
	protected TextWidget m_OrgTitleText;
	protected TextWidget m_OwnerRoleText;
	protected TextWidget m_OwnerNameText;
	protected TextWidget m_OwnerPermsText;
	protected TextWidget m_OwnerSelector;
	protected TextWidget m_EmptyMembersText;
	protected TextWidget m_OverflowMembersText;
	protected TextWidget m_DetailTitleText;
	protected TextWidget m_DetailNameText;
	protected TextWidget m_DetailRoleText;
	protected TextWidget m_DetailIdText;
	protected TextWidget m_DetailPermsHeader;
	protected TextWidget m_HintText;
	protected ref array<TextWidget> m_MemberNameTexts;
	protected ref array<TextWidget> m_MemberPermsTexts;
	protected ref array<TextWidget> m_MemberSelectors;
	protected ref array<TextWidget> m_PermCheckTexts;
	protected ref array<TextWidget> m_PermNameTexts;

	protected ButtonWidget m_OwnerButton;
	protected ButtonWidget m_KickButton;
	protected ButtonWidget m_ClaimButton;
	protected ButtonWidget m_InviteButton;
	protected ButtonWidget m_ResetMembersButton;
	protected ButtonWidget m_JoinLeaveButton;
	protected ButtonWidget m_EjectFloppyButton;
	protected ButtonWidget m_ResetAdminButton;
	protected ButtonWidget m_CloseButton;
	protected ref array<ButtonWidget> m_MemberButtons;
	protected ref array<ButtonWidget> m_PermButtons;

	protected string m_SelectedMemberGuid;
	protected int m_SelectedEmptySlotRow = -1;
	protected bool m_ConfirmResetMembers;
	protected bool m_ConfirmEjectFloppy;
	protected bool m_ConfirmKick;
	protected bool m_CursorVisible;
	protected float m_CursorBlinkTimer;
	protected float m_RefreshTimer;
	protected ref TStringArray m_VisibleMemberGuids;

	protected ref array<int> m_PermBitsTable;
	protected ref array<string> m_PermLabels;
	protected ref array<string> m_PermShortCodes;
	protected ref array<bool> m_PermEnabledFlags;
	protected bool m_PrevF1;
	protected bool m_PrevF2;
	protected bool m_PrevF3;
	protected bool m_PrevF4;
	protected bool m_PrevF5;
	protected bool m_PrevF6;
	protected float m_SessionTime;
	protected MultilineTextWidget m_HeaderBlockText;
	protected MultilineTextWidget m_OrgTreeText;
	protected MultilineTextWidget m_AccessRightsText;
	protected MultilineTextWidget m_MemberDetailText;
	protected MultilineTextWidget m_LogPanelText;
	protected ref array<ImageWidget> m_BorderStrips;
	protected ImageWidget m_ScrollTrack;
	protected ImageWidget m_ScrollThumb;
	protected int m_OrgScrollOffset;
	protected float m_LogUpdateTimer;
	protected int m_LogTick;
	protected ref array<string> m_LogLines;
	protected int m_TelemetryCpu;
	protected int m_TelemetryMem;
	protected int m_TelemetrySig;
	protected int m_TelemetryTemp;
	protected int m_TelemetryNet;

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("ElysianTerritoriesPlus/gui/layouts/multiterritories/territory_computer_menu.layout");
		m_Root = layoutRoot;
		m_MenuRoot = layoutRoot;

		BuildPermissionTable();
		FindLayoutWidgets();
		BuildBackgrounds();
		ApplyLayout();
		HideLegacyWidgets();
		BuildSectionBorders();

		m_VisibleMemberGuids = new TStringArray;
		m_PermEnabledFlags = new array<bool>;
		for (int p = 0; p < PERM_ROW_COUNT; p++) m_PermEnabledFlags.Insert(false);
		m_SelectedMemberGuid = "";
		m_SelectedEmptySlotRow = -1;
		m_SessionTime = 0;
		m_OrgScrollOffset = 0;
		m_ConfirmResetMembers = false;
		m_ConfirmEjectFloppy = false;
		m_ConfirmKick = false;
		m_CursorVisible = true;
		m_CursorBlinkTimer = 0;
		m_RefreshTimer = 0;

		m_LogLines = new array<string>;
		m_TelemetryCpu = 42;
		m_TelemetryMem = 60;
		m_TelemetrySig = 80;
		m_TelemetryTemp = 421;
		m_TelemetryNet = 35;

		REV_TerritoryComputer targetComputer = GetTargetComputer();
		if (targetComputer)
		{
			targetComputer.SyncTerritory();
			targetComputer.ForceProtectionRefresh(true);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Refresh, 350, false);
		}

		SeedLogLines();
		UpdateSystemLog();
		Refresh();
		return layoutRoot;
	}

	protected void BuildPermissionTable()
	{
		m_PermBitsTable = new array<int>;
		m_PermLabels = new array<string>;
		m_PermShortCodes = new array<string>;

		m_PermBitsTable.Insert(TerritoryPerm.ADMIN);
		m_PermLabels.Insert("ADMIN");
		m_PermShortCodes.Insert("A");

		m_PermBitsTable.Insert(TerritoryPerm.DEPLOY);
		m_PermLabels.Insert("DEPLOY");
		m_PermShortCodes.Insert("D");

		m_PermBitsTable.Insert(TerritoryPerm.BUILD);
		m_PermLabels.Insert("BUILD");
		m_PermShortCodes.Insert("B");

		m_PermBitsTable.Insert(TerritoryPerm.DISMANTLE);
		m_PermLabels.Insert("DISMANTLE");
		m_PermShortCodes.Insert("M");

		m_PermBitsTable.Insert(TerritoryPerm.LOWERFLAG);
		m_PermLabels.Insert("LOWER_FLAG");
		m_PermShortCodes.Insert("L");

		m_PermBitsTable.Insert(TerritoryPerm.ADDMEMBER);
		m_PermLabels.Insert("ADD_MEMBER");
		m_PermShortCodes.Insert("+");

		m_PermBitsTable.Insert(TerritoryPerm.REMOVEMEMBER);
		m_PermLabels.Insert("REMOVE_MEMBER");
		m_PermShortCodes.Insert("-");

		m_PermBitsTable.Insert(TerritoryPerm.DEPLOYSPECIAL);
		m_PermLabels.Insert("DEPLOY_SPECIAL");
		m_PermShortCodes.Insert("S");

		m_PermBitsTable.Insert(TerritoryPerm.REMOVECARCOVER);
		m_PermLabels.Insert("REMOVE_CAR_COVER");
		m_PermShortCodes.Insert("C");
	}

	protected void FindLayoutWidgets()
	{
		m_MenuPanel = Widget.Cast(m_MenuRoot.FindAnyWidget("MenuPanel"));
		m_BezelPanel = Widget.Cast(m_MenuRoot.FindAnyWidget("BezelPanel"));
		m_ScreenBackground = Widget.Cast(m_MenuRoot.FindAnyWidget("ScreenBackground"));
		m_StatusBarPanel = Widget.Cast(m_MenuRoot.FindAnyWidget("StatusBarPanel"));
		m_OrgPanel = Widget.Cast(m_MenuRoot.FindAnyWidget("OrgPanel"));
		m_DetailPanel = Widget.Cast(m_MenuRoot.FindAnyWidget("DetailPanel"));
		m_CommandPanel = Widget.Cast(m_MenuRoot.FindAnyWidget("CommandPanel"));
		m_OwnerNodeBg = Widget.Cast(m_MenuRoot.FindAnyWidget("OwnerNodeBg"));
		m_KickButtonBg = Widget.Cast(m_MenuRoot.FindAnyWidget("KickButtonBg"));
		m_ClaimButtonBg = Widget.Cast(m_MenuRoot.FindAnyWidget("ClaimButtonBg"));
		m_InviteButtonBg = Widget.Cast(m_MenuRoot.FindAnyWidget("InviteButtonBg"));
		m_ResetMembersButtonBg = Widget.Cast(m_MenuRoot.FindAnyWidget("ResetMembersButtonBg"));
		m_JoinLeaveButtonBg = Widget.Cast(m_MenuRoot.FindAnyWidget("JoinLeaveButtonBg"));
		m_EjectFloppyButtonBg = Widget.Cast(m_MenuRoot.FindAnyWidget("EjectFloppyButtonBg"));
		m_ResetAdminButtonBg = Widget.Cast(m_MenuRoot.FindAnyWidget("ResetAdminButtonBg"));
		m_CloseButtonBg = Widget.Cast(m_MenuRoot.FindAnyWidget("CloseButtonBg"));

		m_MemberBgs = new array<Widget>;
		m_MemberNameTexts = new array<TextWidget>;
		m_MemberPermsTexts = new array<TextWidget>;
		m_MemberSelectors = new array<TextWidget>;
		m_MemberButtons = new array<ButtonWidget>;
		for (int i = 0; i < MAX_VISIBLE_MEMBERS; i++)
		{
			string idx = i.ToString();
			m_MemberBgs.Insert(Widget.Cast(m_MenuRoot.FindAnyWidget("Member" + idx + "_Bg")));
			m_MemberNameTexts.Insert(TextWidget.Cast(m_MenuRoot.FindAnyWidget("Member" + idx + "_Name")));
			m_MemberPermsTexts.Insert(TextWidget.Cast(m_MenuRoot.FindAnyWidget("Member" + idx + "_Perms")));
			m_MemberSelectors.Insert(TextWidget.Cast(m_MenuRoot.FindAnyWidget("Member" + idx + "_Selector")));

			ButtonWidget btn;
			if (i < LAYOUT_DEFINED_MEMBER_BUTTONS)
			{
				btn = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("Member" + idx + "_Button"));
			}
			else
			{
				btn = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidget(ButtonWidgetTypeID, 0, 0, 100, 100, WidgetFlags.VISIBLE, COLOR_TRANSPARENT, 0, m_MenuRoot));
			}
			m_MemberButtons.Insert(btn);
		}

		m_PermBgs = new array<Widget>;
		m_PermCheckTexts = new array<TextWidget>;
		m_PermNameTexts = new array<TextWidget>;
		m_PermButtons = new array<ButtonWidget>;
		for (int j = 0; j < PERM_ROW_COUNT; j++)
		{
			string jdx = j.ToString();
			m_PermBgs.Insert(Widget.Cast(m_MenuRoot.FindAnyWidget("Perm" + jdx + "_Bg")));
			m_PermCheckTexts.Insert(TextWidget.Cast(m_MenuRoot.FindAnyWidget("Perm" + jdx + "_Check")));
			m_PermNameTexts.Insert(TextWidget.Cast(m_MenuRoot.FindAnyWidget("Perm" + jdx + "_Name")));
			m_PermButtons.Insert(ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("Perm" + jdx + "_Button")));
		}

		m_TitleText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("TitleText"));
		m_FloppyStatusText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("FloppyStatusText"));
		m_StatusText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("StatusText"));
		m_OwnerHeaderText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("OwnerHeaderText"));
		m_InviteText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("InviteText"));
		m_MemberCountText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("MemberCountText"));
		m_FloppyLifeText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("FloppyLifeText"));
		m_LoggedInAsText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("LoggedInAsText"));
		m_OrgTitleText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("OrgTitleText"));
		m_OwnerRoleText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("OwnerRoleText"));
		m_OwnerNameText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("OwnerNameText"));
		m_OwnerPermsText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("OwnerPermsText"));
		m_OwnerSelector = TextWidget.Cast(m_MenuRoot.FindAnyWidget("OwnerSelector"));
		m_EmptyMembersText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("EmptyMembersText"));
		m_OverflowMembersText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("OverflowMembersText"));
		m_DetailTitleText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("DetailTitleText"));
		m_DetailNameText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("DetailNameText"));
		m_DetailRoleText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("DetailRoleText"));
		m_DetailIdText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("DetailIdText"));
		m_DetailPermsHeader = TextWidget.Cast(m_MenuRoot.FindAnyWidget("DetailPermsHeader"));
		m_HintText = TextWidget.Cast(m_MenuRoot.FindAnyWidget("HintText"));

		m_OwnerButton = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("OwnerButton"));
		m_KickButton = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("KickButton"));
		m_ClaimButton = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("ClaimButton"));
		m_InviteButton = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("InviteButton"));
		m_ResetMembersButton = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("ResetMembersButton"));
		m_JoinLeaveButton = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("JoinLeaveButton"));
		m_EjectFloppyButton = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("EjectFloppyButton"));
		m_ResetAdminButton = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("ResetAdminButton"));
		m_CloseButton = ButtonWidget.Cast(m_MenuRoot.FindAnyWidget("CloseButton"));

		m_HeaderBlockText = MultilineTextWidget.Cast(m_MenuRoot.FindAnyWidget("HeaderBlockText"));
		m_OrgTreeText = MultilineTextWidget.Cast(m_MenuRoot.FindAnyWidget("OrgTreeText"));
		m_AccessRightsText = MultilineTextWidget.Cast(m_MenuRoot.FindAnyWidget("AccessRightsText"));
		m_MemberDetailText = MultilineTextWidget.Cast(m_MenuRoot.FindAnyWidget("MemberDetailText"));
		m_LogPanelText = MultilineTextWidget.Cast(m_MenuRoot.FindAnyWidget("LogPanelText"));
	}

	protected ImageWidget CreateBgImage(int sort, string texturePath, int color)
	{
		ImageWidget image = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidget(ImageWidgetTypeID, 0, 0, 100, 100, WidgetFlags.VISIBLE, ARGB(255, 0, 0, 0), sort, m_MenuRoot));
		if (image)
		{
			if (texturePath != string.Empty && !IsProceduralColorTexture(texturePath))
			{
				image.LoadImageFile(0, texturePath);
				image.SetImage(0);
			}
			image.SetColor(color);
			image.Enable(false);
		}

		return image;
	}

	protected bool IsProceduralColorTexture(string texturePath)
	{
		return texturePath.Length() >= 2 && texturePath.Substring(0, 2) == "#(";
	}

	protected void BuildBackgrounds()
	{
		if (!m_MenuRoot) return;

		string procColor = "#(argb,8,8,3)color(1,1,1,1,ca)";

		m_MenuPanelBg = CreateBgImage(-100, procColor, COLOR_WINDOW);
		m_BezelPanelBg = CreateBgImage(-99, procColor, COLOR_HEADER);
		m_ScreenBackgroundBg = CreateBgImage(-98, procColor, COLOR_SECTION);
		m_StatusBarPanelBg = CreateBgImage(-95, procColor, COLOR_SECTION);
		m_OrgPanelBg = CreateBgImage(-95, procColor, COLOR_SECTION);
		m_DetailPanelBg = CreateBgImage(-95, procColor, COLOR_SECTION);
		m_CommandPanelBg = CreateBgImage(-95, procColor, COLOR_SECTION);

		m_OwnerNodeBgImg = CreateBgImage(-80, procColor, COLOR_OWNER_BG);

		m_MemberNodeBgImgs = new array<ImageWidget>;
		for (int i = 0; i < MAX_VISIBLE_MEMBERS; i++)
		{
			m_MemberNodeBgImgs.Insert(CreateBgImage(-80, procColor, COLOR_NODE_BG));
		}

		m_PermRowBgImgs = new array<ImageWidget>;
		for (int j = 0; j < PERM_ROW_COUNT; j++)
		{
			m_PermRowBgImgs.Insert(CreateBgImage(-80, procColor, COLOR_BUTTON));
		}

		m_KickButtonBgImg = CreateBgImage(-80, procColor, COLOR_DESTRUCTIVE);
		m_ClaimButtonBgImg = CreateBgImage(-80, procColor, COLOR_BUTTON);
		m_InviteButtonBgImg = CreateBgImage(-80, procColor, COLOR_BUTTON);
		m_ResetMembersButtonBgImg = CreateBgImage(-80, procColor, COLOR_BUTTON);
		m_JoinLeaveButtonBgImg = CreateBgImage(-80, procColor, COLOR_BUTTON);
		m_EjectFloppyButtonBgImg = CreateBgImage(-80, procColor, COLOR_BUTTON);
		m_ResetAdminButtonBgImg = CreateBgImage(-80, procColor, COLOR_BUTTON);
		m_CloseButtonBgImg = CreateBgImage(-80, procColor, COLOR_BUTTON);
	}

	protected void HideLegacyWidgets()
	{
		if (m_OwnerPermsText) m_OwnerPermsText.Show(false);
		if (m_OwnerSelector) m_OwnerSelector.Show(false);
		if (m_OwnerNodeBg) m_OwnerNodeBg.Show(false);
		if (m_OwnerNodeBgImg) m_OwnerNodeBgImg.Show(false);
		if (m_EmptyMembersText) m_EmptyMembersText.Show(false);
		if (m_OverflowMembersText) m_OverflowMembersText.Show(false);
		if (m_StatusText) m_StatusText.Show(false);
		if (m_FloppyLifeText) m_FloppyLifeText.Show(false);
		if (m_LoggedInAsText) m_LoggedInAsText.Show(false);
		if (m_StatusBarPanel) m_StatusBarPanel.Show(false);
		if (m_StatusBarPanelBg) m_StatusBarPanelBg.Show(false);
		if (m_DetailNameText) m_DetailNameText.Show(false);
		if (m_DetailRoleText) m_DetailRoleText.Show(false);
		if (m_DetailIdText) m_DetailIdText.Show(false);
		if (m_OwnerNameText) m_OwnerNameText.Show(false);
		if (m_OwnerRoleText) m_OwnerRoleText.Show(false);
		if (m_InviteText) m_InviteText.Show(false);
		if (m_MemberCountText) m_MemberCountText.Show(false);

		for (int i = 0; i < MAX_VISIBLE_MEMBERS; i++)
		{
			if (m_MemberBgs[i]) m_MemberBgs[i].Show(false);
			if (m_MemberNodeBgImgs && i < m_MemberNodeBgImgs.Count() && m_MemberNodeBgImgs[i]) m_MemberNodeBgImgs[i].Show(false);
			if (m_MemberNameTexts[i]) m_MemberNameTexts[i].Show(false);
			if (m_MemberPermsTexts[i]) m_MemberPermsTexts[i].Show(false);
			if (m_MemberSelectors[i]) m_MemberSelectors[i].Show(false);
		}
		for (int j = 0; j < PERM_ROW_COUNT; j++)
		{
			if (m_PermBgs[j]) m_PermBgs[j].Show(false);
			if (m_PermRowBgImgs && j < m_PermRowBgImgs.Count() && m_PermRowBgImgs[j]) m_PermRowBgImgs[j].Show(false);
			if (m_PermCheckTexts[j]) m_PermCheckTexts[j].Show(false);
			if (m_PermNameTexts[j]) m_PermNameTexts[j].Show(false);
		}
	}

	protected void DrawBorderBox(float x, float y, float w, float h, int color)
	{
		float thickness = 0.0015;
		string procColor = "#(argb,8,8,3)color(1,1,1,1,ca)";
		int sort = -50;

		ImageWidget top = CreateBgImage(sort, procColor, color);
		if (top)
		{
			top.SetPos(x, y);
			top.SetSize(w, thickness);
			m_BorderStrips.Insert(top);
		}

		ImageWidget bot = CreateBgImage(sort, procColor, color);
		if (bot)
		{
			bot.SetPos(x, y + h - thickness);
			bot.SetSize(w, thickness);
			m_BorderStrips.Insert(bot);
		}

		ImageWidget lft = CreateBgImage(sort, procColor, color);
		if (lft)
		{
			lft.SetPos(x, y);
			lft.SetSize(thickness, h);
			m_BorderStrips.Insert(lft);
		}

		ImageWidget rgt = CreateBgImage(sort, procColor, color);
		if (rgt)
		{
			rgt.SetPos(x + w - thickness, y);
			rgt.SetSize(thickness, h);
			m_BorderStrips.Insert(rgt);
		}
	}

	protected void BuildSectionBorders()
	{
		m_BorderStrips = new array<ImageWidget>;

		int borderColor = ARGB(180, 180, 140, 40);

		DrawBorderBox(0.150, 0.055, 0.700, 0.730, borderColor);

		DrawBorderBox(0.180, 0.190, 0.310, 0.340, borderColor);

		DrawBorderBox(0.500, 0.190, 0.330, 0.165, borderColor);

		DrawBorderBox(0.500, 0.365, 0.330, 0.165, borderColor);

		DrawBorderBox(0.180, 0.535, 0.650, 0.150, borderColor);

		string procColor = "#(argb,8,8,3)color(1,1,1,1,ca)";
		int trackColor = ARGB(120, 90, 110, 40);
		int thumbColor = ARGB(220, 220, 170, 50);

		m_ScrollTrack = CreateBgImage(-49, procColor, trackColor);
		if (m_ScrollTrack)
		{
			m_ScrollTrack.SetPos(0.183, 0.225);
			m_ScrollTrack.SetSize(0.0035, 0.265);
			m_BorderStrips.Insert(m_ScrollTrack);
		}

		m_ScrollThumb = CreateBgImage(-48, procColor, thumbColor);
		if (m_ScrollThumb)
		{
			m_ScrollThumb.SetPos(0.183, 0.225);
			m_ScrollThumb.SetSize(0.0035, 0.080);
			m_BorderStrips.Insert(m_ScrollThumb);
		}
	}

	protected void ApplyLayout()
	{
		if (m_Root)
		{
			m_Root.SetPos(0, 0);
			m_Root.SetSize(1, 1);
			m_Root.SetColor(COLOR_TRANSPARENT);
		}

		if (m_MenuRoot)
		{
			m_MenuRoot.SetPos(0, 0);
			m_MenuRoot.SetSize(1, 1);
			m_MenuRoot.SetColor(COLOR_TRANSPARENT);
		}

		SetPanel(m_MenuPanel, m_MenuPanelBg, 0.15, 0.055, 0.70, 0.730, COLOR_TRANSPARENT, COLOR_WINDOW);
		SetPanel(m_BezelPanel, m_BezelPanelBg, 0.15, 0.055, 0.70, 0.045, COLOR_TRANSPARENT, COLOR_HEADER);

		if (m_ScreenBackground) m_ScreenBackground.Show(false);
		if (m_ScreenBackgroundBg) m_ScreenBackgroundBg.Show(false);
		if (m_OrgPanel) m_OrgPanel.Show(false);
		if (m_OrgPanelBg) m_OrgPanelBg.Show(false);
		if (m_DetailPanel) m_DetailPanel.Show(false);
		if (m_DetailPanelBg) m_DetailPanelBg.Show(false);
		if (m_CommandPanel) m_CommandPanel.Show(false);
		if (m_CommandPanelBg) m_CommandPanelBg.Show(false);

		SetPanel(m_KickButtonBg, m_KickButtonBgImg, 0.510, 0.345, 0.315, 0.018, COLOR_TRANSPARENT, COLOR_DESTRUCTIVE);
		SetPanel(m_ClaimButtonBg, m_ClaimButtonBgImg, 0.195, 0.700, 0.085, 0.038, COLOR_TRANSPARENT, COLOR_BUTTON);
		SetPanel(m_InviteButtonBg, m_InviteButtonBgImg, 0.285, 0.700, 0.085, 0.038, COLOR_TRANSPARENT, COLOR_BUTTON);
		SetPanel(m_ResetMembersButtonBg, m_ResetMembersButtonBgImg, 0.375, 0.700, 0.085, 0.038, COLOR_TRANSPARENT, COLOR_BUTTON);
		SetPanel(m_JoinLeaveButtonBg, m_JoinLeaveButtonBgImg, 0.465, 0.700, 0.085, 0.038, COLOR_TRANSPARENT, COLOR_BUTTON);
		SetPanel(m_EjectFloppyButtonBg, m_EjectFloppyButtonBgImg, 0.555, 0.700, 0.085, 0.038, COLOR_TRANSPARENT, COLOR_BUTTON);
		SetPanel(m_ResetAdminButtonBg, m_ResetAdminButtonBgImg, 0.645, 0.700, 0.085, 0.038, COLOR_TRANSPARENT, COLOR_BUTTON);
		SetPanel(m_CloseButtonBg, m_CloseButtonBgImg, 0.735, 0.700, 0.085, 0.038, COLOR_TRANSPARENT, COLOR_BUTTON);

		PositionStaticWidgets();
		PositionMemberWidgets();
		PositionPermWidgets();
	}

	protected void PositionStaticWidgets()
	{
		SetTxtSized(m_TitleText,           0.190, 0.085, 0.45, 0.022, 12);
		SetTxtSized(m_FloppyStatusText,    0.580, 0.085, 0.245, 0.022, 11);

		SetTxtSized(m_OrgTitleText,        0.190, 0.198, 0.295, 0.018, 11);
		SetTxtSized(m_DetailTitleText,     0.510, 0.198, 0.315, 0.018, 11);
		SetTxtSized(m_DetailPermsHeader,   0.510, 0.372, 0.315, 0.018, 11);
		SetTxtSized(m_OwnerHeaderText,     0.190, 0.543, 0.635, 0.018, 11);

		SetBtnSized(m_KickButton,          0.510, 0.345, 0.315, 0.018, 11);

		SetBtnSized(m_ClaimButton,        0.195, 0.700, 0.085, 0.038, 13);
		SetBtnSized(m_InviteButton,       0.285, 0.700, 0.085, 0.038, 13);
		SetBtnSized(m_ResetMembersButton, 0.375, 0.700, 0.085, 0.038, 13);
		SetBtnSized(m_JoinLeaveButton,    0.465, 0.700, 0.085, 0.038, 13);
		SetBtnSized(m_EjectFloppyButton,  0.555, 0.700, 0.085, 0.038, 13);
		SetBtnSized(m_ResetAdminButton,   0.645, 0.700, 0.085, 0.038, 13);
		SetBtnSized(m_CloseButton,        0.735, 0.700, 0.085, 0.038, 13);

		SetTxtSized(m_HintText,           0.190, 0.750, 0.620, 0.018, 10);

		if (m_HeaderBlockText)
		{
			m_HeaderBlockText.SetPos(0.180, 0.110);
			m_HeaderBlockText.SetSize(0.65, 0.055);
			m_HeaderBlockText.SetColor(COLOR_TEXT_BRIGHT);
			m_HeaderBlockText.SetText("HEADER_INIT :: awaiting first refresh tick...");
			m_HeaderBlockText.Show(true);
		}
		if (m_OrgTreeText)
		{
			m_OrgTreeText.SetPos(0.190, 0.225);
			m_OrgTreeText.SetSize(0.295, 0.265);
			m_OrgTreeText.Show(true);
		}
		if (m_MemberDetailText)
		{
			m_MemberDetailText.SetPos(0.510, 0.225);
			m_MemberDetailText.SetSize(0.315, 0.110);
			m_MemberDetailText.Show(true);
		}
		if (m_AccessRightsText)
		{
			m_AccessRightsText.SetPos(0.510, 0.400);
			m_AccessRightsText.SetSize(0.315, 0.115);
			m_AccessRightsText.Show(true);
		}
		if (m_LogPanelText)
		{
			m_LogPanelText.SetPos(0.190, 0.565);
			m_LogPanelText.SetSize(0.635, 0.110);
			m_LogPanelText.Show(true);
		}

		if (m_OrgTitleText) m_OrgTitleText.SetColor(COLOR_TEXT_AMBER);
		if (m_DetailTitleText) m_DetailTitleText.SetColor(COLOR_TEXT_AMBER);
		if (m_DetailPermsHeader) m_DetailPermsHeader.SetColor(COLOR_TEXT_AMBER);
		if (m_OwnerHeaderText) m_OwnerHeaderText.SetColor(COLOR_TEXT_AMBER);
	}

	protected void PositionMemberWidgets()
	{
		float rowH = 0.0165;
		float rowY0 = 0.225;
		float rowX = 0.190;
		float rowW = 0.295;
		float barYOffset = 0.0040;

		SetBtn(m_OwnerButton, rowX, rowY0, rowW, rowH);

		float barInsetX = 0.004;
		float fullRowBarW = rowW - barInsetX * 2;
		float ownerBarW = fullRowBarW;
		float memberBarW = fullRowBarW;

		if (m_OwnerNodeBgImg)
		{
			m_OwnerNodeBgImg.SetPos(rowX + barInsetX, rowY0 + barYOffset);
			m_OwnerNodeBgImg.SetSize(ownerBarW, rowH);
			m_OwnerNodeBgImg.SetColor(COLOR_HIGHLIGHT_AMBER);
			m_OwnerNodeBgImg.Show(false);
		}

		for (int i = 0; i < MAX_VISIBLE_MEMBERS; i++)
		{
			float by = rowY0 + (1 + i) * rowH;
			SetBtn(m_MemberButtons[i], rowX, by, rowW, rowH);

			if (m_MemberNodeBgImgs && i < m_MemberNodeBgImgs.Count() && m_MemberNodeBgImgs[i])
			{
				m_MemberNodeBgImgs[i].SetPos(rowX + barInsetX, by + barYOffset);
				m_MemberNodeBgImgs[i].SetSize(memberBarW, rowH);
				m_MemberNodeBgImgs[i].SetColor(COLOR_HIGHLIGHT_GREEN);
				m_MemberNodeBgImgs[i].Show(false);
			}
		}

		for (int p = 0; p < PERM_ROW_COUNT; p++)
		{
			float py = 0.470 + p * 0.013;
			if (m_PermRowBgImgs && p < m_PermRowBgImgs.Count() && m_PermRowBgImgs[p])
			{
				m_PermRowBgImgs[p].SetPos(0.535, py);
				m_PermRowBgImgs[p].SetSize(0.275, 0.013);
				m_PermRowBgImgs[p].SetColor(COLOR_HIGHLIGHT_GREEN);
				m_PermRowBgImgs[p].Show(false);
			}
		}
	}

	protected void PositionPermWidgets()
	{
		float rowH = 0.012;
		float rowY0 = 0.400;
		float rowX = 0.510;
		float rowW = 0.315;
		for (int j = 0; j < PERM_ROW_COUNT; j++)
		{
			float py = rowY0 + j * rowH;
			SetBtn(m_PermButtons[j], rowX, py, rowW, rowH);
		}
	}

	protected void SetTxt(TextWidget t, float x, float y, float w, float h)
	{
		if (!t) return;
		t.SetPos(x, y);
		t.SetSize(w, h);
		t.Show(true);
	}

	protected void SetTxtSized(TextWidget t, float x, float y, float w, float h, int sz)
	{
		if (!t) return;
		t.SetPos(x, y);
		t.SetSize(w, h);
		t.SetTextExactSize(sz);
		t.Show(true);
	}

	protected void SetMultilineSized(MultilineTextWidget t, float x, float y, float w, float h, int sz)
	{
		if (!t) return;
		t.SetPos(x, y);
		t.SetSize(w, h);
		t.Show(true);
	}

	protected void SetBtn(ButtonWidget b, float x, float y, float w, float h)
	{
		if (!b) return;
		b.SetPos(x, y);
		b.SetSize(w, h);
		b.Show(true);
	}

	protected void SetBtnSized(ButtonWidget b, float x, float y, float w, float h, int sz)
	{
		if (!b) return;
		b.SetPos(x, y);
		b.SetSize(w, h);
		b.Show(true);
	}

	protected void SetPanel(Widget panel, ImageWidget bg, float x, float y, float w, float h, int panelColor, int bgColor)
	{
		if (panel)
		{
			panel.SetPos(x, y);
			panel.SetSize(w, h);
			panel.SetColor(panelColor);
		}

		if (bg)
		{
			bg.SetPos(x, y);
			bg.SetSize(w, h);
			bg.SetColor(bgColor);
		}
	}

	override void OnShow()
	{
		super.OnShow();
		GetGame().GetUIManager().ShowUICursor(true);
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
	}

	override void OnHide()
	{
		super.OnHide();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelResetMembersConfirmation);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelEjectFloppyConfirmation);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelKickConfirmation);
		m_ConfirmResetMembers = false;
		m_ConfirmEjectFloppy = false;
		m_ConfirmKick = false;
		MultiplixTerritoryMenuState.Clear();
		GetGame().GetUIManager().ShowUICursor(false);
		GetGame().GetMission().PlayerControlEnable(true);
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		m_SessionTime = m_SessionTime + timeslice;
		m_CursorBlinkTimer = m_CursorBlinkTimer + timeslice;
		if (m_CursorBlinkTimer >= CURSOR_BLINK_INTERVAL)
		{
			m_CursorBlinkTimer = 0;
			m_CursorVisible = !m_CursorVisible;
		}
		UpdateTitleCursor();

		m_LogUpdateTimer = m_LogUpdateTimer + timeslice;
		if (m_LogUpdateTimer >= 0.35)
		{
			m_LogUpdateTimer = 0;
			UpdateSystemLog();
			TickTelemetry();
			REV_TerritoryComputer tc = GetTargetComputer();
			if (tc) BuildHeaderBlock(tc, GetPlayerGuid());
		}

		m_RefreshTimer = m_RefreshTimer + timeslice;
		if (m_RefreshTimer >= REFRESH_INTERVAL)
		{
			m_RefreshTimer = 0;
			Refresh();
		}

		UAInput backInput = GetUApi().GetInputByName("UAUIBack");
		if (backInput && backInput.LocalPress())
		{
			Close();
		}

		PollFunctionKeys();
	}

	protected void PollFunctionKeys()
	{
		bool f1 = KeyState(KeyCode.KC_F1) != 0;
		bool f2 = KeyState(KeyCode.KC_F2) != 0;
		bool f3 = KeyState(KeyCode.KC_F3) != 0;
		bool f4 = KeyState(KeyCode.KC_F4) != 0;
		bool f5 = KeyState(KeyCode.KC_F5) != 0;
		bool f6 = KeyState(KeyCode.KC_F6) != 0;

		if (f1 && !m_PrevF1 && m_ClaimButton && m_ClaimButton.IsVisible()) TriggerClaim();
		if (f2 && !m_PrevF2 && m_InviteButton && m_InviteButton.IsVisible()) TriggerInvite();
		if (f3 && !m_PrevF3 && m_ResetMembersButton && m_ResetMembersButton.IsVisible()) TriggerPurge();
		if (f4 && !m_PrevF4 && m_JoinLeaveButton && m_JoinLeaveButton.IsVisible()) TriggerJoinLeave();
		if (f5 && !m_PrevF5 && m_EjectFloppyButton && m_EjectFloppyButton.IsVisible()) TriggerEjectFloppy();
		if (f6 && !m_PrevF6 && m_ResetAdminButton && m_ResetAdminButton.IsVisible()) TriggerResetAdmin();

		m_PrevF1 = f1;
		m_PrevF2 = f2;
		m_PrevF3 = f3;
		m_PrevF4 = f4;
		m_PrevF5 = f5;
		m_PrevF6 = f6;
	}

	protected string Hex2(int v)
	{
		v = v & 0xFF;
		string hexChars = "0123456789ABCDEF";
		int hi = (v >> 4) & 0x0F;
		int lo = v & 0x0F;
		return hexChars.Substring(hi, 1) + hexChars.Substring(lo, 1);
	}

	protected string Hex4()
	{
		int v = Math.RandomIntInclusive(0, 65535);
		int b0 = (v >> 8) & 0xFF;
		int b1 = v & 0xFF;
		return Hex2(b0) + Hex2(b1);
	}

	protected string RandomLogLine()
	{
		int kind = Math.RandomIntInclusive(0, 19);
		string addr = "0x" + Hex4();
		string b1 = "0x" + Hex2(Math.RandomIntInclusive(0, 255));
		string b2 = "0x" + Hex2(Math.RandomIntInclusive(0, 255));
		string b3 = "0x" + Hex2(Math.RandomIntInclusive(0, 255));
		string b4 = "0x" + Hex2(Math.RandomIntInclusive(0, 255));

		if (kind == 0)  return "DATA " + addr + " :: " + b1 + " " + b2 + " " + b3 + " " + b4;
		if (kind == 1)  return "EXEC " + addr + " :: kern.0x" + Hex2(Math.RandomIntInclusive(0, 255));
		if (kind == 2)  return "RX   " + addr + " <- " + b1 + b2 + " (" + Math.RandomIntInclusive(8, 4096).ToString() + "B)";
		if (kind == 3)  return "TX   " + addr + " -> " + b3 + b4 + " (" + Math.RandomIntInclusive(8, 4096).ToString() + "B)";
		if (kind == 4)  return "CKSUM " + addr + " :: PASS";
		if (kind == 5)  return "MEM  " + addr + " READ " + b1 + " " + b2 + " " + b3 + " " + b4;
		if (kind == 6)  return "AUTH " + addr + " :: VERIFIED";
		if (kind == 7)  return "INT  " + Math.RandomIntInclusive(0, 7).ToString() + " :: HANDLED @ " + addr;
		if (kind == 8)  return "POLL " + addr + " :: IDLE";
		if (kind == 9)  return "DMA  " + addr + " -> 0x" + Hex4() + " :: OK";
		if (kind == 10) return "CACHE " + addr + " :: HIT  " + Math.RandomIntInclusive(50, 99).ToString() + "%";
		if (kind == 11) return "IRQ  " + Math.RandomIntInclusive(0, 15).ToString() + " :: ACK @ " + addr;
		if (kind == 12) return "BUS  " + addr + " :: ARBITRATION OK";

		if (kind == 13) return "KERN " + addr + " :: build/Multiplix r" + Math.RandomIntInclusive(100, 999).ToString();
		if (kind == 14) return "AUTH " + addr + " :: HANDSHAKE_OK <- Llama";
		if (kind == 15) return "ROM  " + addr + " :: (c) Multiplix Industries 1986";
		if (kind == 16) return "EXEC " + addr + " :: /usr/sys/llama_init.o";
		if (kind == 17) return "LINK " + addr + " :: lib_multiplix.a v5.7." + Math.RandomIntInclusive(0, 9).ToString();
		if (kind == 18) return "PING " + addr + " :: nodes/llama-prime :: " + Math.RandomIntInclusive(2, 18).ToString() + "ms";
		return "SIGN " + addr + " :: //Llama@Multiplix//";
	}

	protected string PadRight(string s, int n)
	{
		int len = s.Length();
		if (len >= n) return s;
		string res = s;
		for (int i = 0; i < n - len; i++) res = res + " ";
		return res;
	}

	protected string PadLeft(string s, int n)
	{
		int len = s.Length();
		if (len >= n) return s;
		string res = "";
		for (int i = 0; i < n - len; i++) res = res + " ";
		return res + s;
	}

	protected string BuildBar(float fraction, int width)
	{
		if (fraction < 0) fraction = 0;
		if (fraction > 1) fraction = 1;
		int filled = Math.Round(fraction * width);
		string bar = "";
		for (int i = 0; i < width; i++)
		{
			if (i < filled) bar = bar + "#";
			else bar = bar + ".";
		}
		return bar;
	}

	protected string FormatClock(int totalSec)
	{
		int hh = (totalSec / 3600) % 24;
		int mm = (totalSec / 60) % 60;
		int ss = totalSec % 60;
		string hhs = hh.ToString();
		string mms = mm.ToString();
		string sss = ss.ToString();
		if (hh < 10) hhs = "0" + hhs;
		if (mm < 10) mms = "0" + mms;
		if (ss < 10) sss = "0" + sss;
		return hhs + ":" + mms + ":" + sss;
	}

	protected void SeedLogLines()
	{
		m_LogLines.Clear();
		m_LogLines.Insert("[00:00:00]  BOOT  0x0000 :: ROM v5.7.2");
		m_LogLines.Insert("[00:00:00]  INIT  1541-BUS :: SYNC");
		m_LogLines.Insert("[00:00:00]  KERN  0x4000 :: LOADED 14336B");
		m_LogLines.Insert("[00:00:01]  AUTH  HANDSHAKE :: VERIFIED");
		m_LogLines.Insert("[00:00:01]  NET   LINK UP @ 1200baud");
		m_LogLines.Insert("[00:00:02]  POLL  0xA000 :: IDLE");
		m_LogLines.Insert("[00:00:02]  HEARTBEAT // NODE-04 :: OK");
		m_LogLines.Insert("[00:00:02]  > SHELL READY");
	}

	protected void UpdateSystemLog()
	{
		if (!m_LogPanelText) return;
		if (!m_LogLines) return;

		int totalSec = Math.Floor(m_SessionTime);
		for (int gen = 0; gen < 3; gen++)
		{
			string newLine = "[" + FormatClock(totalSec) + "]  " + RandomLogLine();
			m_LogLines.Insert(newLine);
		}

		int LOG_ROWS = 7;
		int LOG_COLS = 3;
		int MAX_LOG_LINES = LOG_ROWS * LOG_COLS;
		while (m_LogLines.Count() > MAX_LOG_LINES) m_LogLines.Remove(0);

		int COL_WIDTH = 42;
		int total = m_LogLines.Count();

		string s = "";
		for (int row = 0; row < LOG_ROWS; row++)
		{
			string rowStr = "";
			for (int col = 0; col < LOG_COLS; col++)
			{
				int idx = col * LOG_ROWS + row;
				string entry = "";
				if (idx < total) entry = m_LogLines[idx];
				rowStr = rowStr + PadRight(entry, COL_WIDTH);
			}
			s = s + rowStr;
			if (row < LOG_ROWS - 1) s = s + "\n";
		}

		m_LogPanelText.SetText(s);
		m_LogPanelText.SetColor(COLOR_TEXT_DIM);
		m_LogTick = m_LogTick + 1;

		if (m_OwnerHeaderText)
		{
			m_OwnerHeaderText.SetText("[ SYSTEM LOG // 1541-BUS // PID 0x" + Hex2(m_LogTick & 0xFF) + " ]");
		}
	}

	protected int ClampInt(int v, int lo, int hi)
	{
		if (v < lo) return lo;
		if (v > hi) return hi;
		return v;
	}

	protected void TickTelemetry()
	{
		m_TelemetryCpu  = ClampInt(m_TelemetryCpu  + Math.RandomIntInclusive(-3, 3), 18, 78);
		m_TelemetryMem  = ClampInt(m_TelemetryMem  + Math.RandomIntInclusive(-2, 2), 40, 92);
		m_TelemetrySig  = ClampInt(m_TelemetrySig  + Math.RandomIntInclusive(-4, 4), 50, 100);
		m_TelemetryTemp = ClampInt(m_TelemetryTemp + Math.RandomIntInclusive(-2, 2), 380, 470);
		m_TelemetryNet  = ClampInt(m_TelemetryNet  + Math.RandomIntInclusive(-5, 5), 5, 95);
	}

	protected void UpdateTitleCursor()
	{
		if (!m_TitleText) return;
		string cursor = "_";
		if (!m_CursorVisible) cursor = " ";
		int totalSec = Math.Floor(m_SessionTime);
		int hh = totalSec / 3600;
		int mm = (totalSec / 60) - (hh * 60);
		int ss = totalSec - (hh * 3600) - (mm * 60);
		string hhs = hh.ToString();
		string mms = mm.ToString();
		string sss = ss.ToString();
		if (mm < 10) mms = "0" + mms;
		if (ss < 10) sss = "0" + sss;
		string uptime = " / IBM TCU 5150 / UP " + hhs + ":" + mms + ":" + sss;
		m_TitleText.SetText("territory://console" + cursor + uptime);
	}

	protected REV_TerritoryComputer GetTargetComputer()
	{
		return REV_TerritoryComputer.Cast(MultiplixTerritoryMenuState.GetTargetComputer());
	}

	protected PlayerBase GetPlayer()
	{
		return PlayerBase.Cast(GetGame().GetPlayer());
	}

	protected string GetPlayerGuid()
	{
		PlayerBase player = GetPlayer();
		if (!player || !player.GetIdentity()) return "";
		return player.GetIdentity().GetId();
	}

	protected string GetPlayerName()
	{
		PlayerBase player = GetPlayer();
		if (!player || !player.GetIdentity()) return "Unknown";
		return player.GetIdentity().GetName();
	}

	protected void SendMenuRPC(int rpcType)
	{
		REV_TerritoryComputer targetComputer = GetTargetComputer();
		if (!targetComputer) return;

		targetComputer.RPCSingleParam(rpcType, new Param1<bool>(true), true, null);
		targetComputer.SyncTerritory();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Refresh, 250, false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Refresh, 750, false);
	}

	protected void SendEditPermissionRPC(string targetGuid, int permBit, bool grant)
	{
		REV_TerritoryComputer targetComputer = GetTargetComputer();
		if (!targetComputer) return;

		targetComputer.RPCSingleParam(MULTITERRITORYRPCs.MENU_EDIT_PERMISSION, new Param3<string, int, bool>(targetGuid, permBit, grant), true, null);
		targetComputer.SyncTerritory();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Refresh, 250, false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Refresh, 750, false);
	}

	protected void SendKickMemberRPC(string targetGuid)
	{
		REV_TerritoryComputer targetComputer = GetTargetComputer();
		if (!targetComputer) return;

		targetComputer.RPCSingleParam(MULTITERRITORYRPCs.MENU_KICK_MEMBER, new Param1<string>(targetGuid), true, null);
		targetComputer.SyncTerritory();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Refresh, 250, false);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Refresh, 750, false);
	}

	protected void CancelResetMembersConfirmation()
	{
		m_ConfirmResetMembers = false;
		Refresh();
	}

	protected void CancelEjectFloppyConfirmation()
	{
		m_ConfirmEjectFloppy = false;
		Refresh();
	}

	protected void CancelKickConfirmation()
	{
		m_ConfirmKick = false;
		Refresh();
	}

	protected string BuildPermBadge(int perms)
	{
		string s = "[";
		for (int i = 0; i < 6 && i < m_PermBitsTable.Count(); i++)
		{
			if ((perms & m_PermBitsTable[i]) != 0)
			{
				s += m_PermShortCodes[i];
			}
			else
			{
				s += " ";
			}
		}
		s += "]";
		return s;
	}

	protected float GetClientFloppyFraction(REV_TerritoryComputer computer)
	{
		if (!computer) return 0;
		return computer.GetRefresherTime01();
	}

	protected int EstimateVitalPct(PlayerBase pl, int kind)
	{
		if (!pl) return 0;
		float frac = -1;
		EStatLevels level = EStatLevels.GREAT;

		if (kind == 0)
		{
			frac = pl.GetHealth01("", "");
			level = pl.GetStatLevelHealth();
		}
		else if (kind == 1)
		{
			frac = pl.GetHealth01("", "Blood");
			level = pl.GetStatLevelBlood();
		}
		else if (kind == 2)
		{
			PlayerStatBase ws = pl.GetStatWater();
			if (ws) frac = ws.GetNormalized();
			level = pl.GetStatLevelWater();
		}
		else if (kind == 3)
		{
			PlayerStatBase es = pl.GetStatEnergy();
			if (es) frac = es.GetNormalized();
			level = pl.GetStatLevelEnergy();
		}

		if (frac > 0)
		{
			if (frac > 1) frac = 1;
			return Math.Round(frac * 100);
		}

		if (level == EStatLevels.GREAT)    return 95;
		if (level == EStatLevels.HIGH)     return 75;
		if (level == EStatLevels.MEDIUM)   return 50;
		if (level == EStatLevels.LOW)      return 25;
		return 8;
	}

	protected void RequestVitalsFromServer(REV_TerritoryComputer targetComputer)
	{
		if (!targetComputer) return;
		if (!GetGame().IsClient()) return;
		targetComputer.RPCSingleParam(MULTITERRITORYRPCs.MENU_REQUEST_VITALS,
			new Param1<bool>(true), true, null);
	}

	protected string AppendAlert(string current, string label)
	{
		if (current == "") return label;
		return current + ", " + label;
	}

	protected int ChooseVitalSeverityColor(bool subjectIsSelf)
	{
		if (!subjectIsSelf) return COLOR_TEXT_DIM;
		if (MultiplixTerritoryMenuState.s_VitalHpPct < 0) return COLOR_TEXT_DIM;

		int hp  = MultiplixTerritoryMenuState.s_VitalHpPct;
		int bl  = MultiplixTerritoryMenuState.s_VitalBlPct;
		int wa  = MultiplixTerritoryMenuState.s_VitalWaPct;
		int en  = MultiplixTerritoryMenuState.s_VitalEnPct;
		int tmp = MultiplixTerritoryMenuState.s_VitalTmpPct;

		int worst = hp;
		if (bl < worst) worst = bl;
		if (wa < worst) worst = wa;
		if (en < worst) worst = en;
		int tmpDist = tmp - 50;
		if (tmpDist < 0) tmpDist = -tmpDist;
		int tmpEquivalent = 100 - tmpDist * 2;
		if (tmpEquivalent < worst) worst = tmpEquivalent;

		if (worst >= 70) return COLOR_VITAL_GREEN;
		if (worst >= 50) return COLOR_VITAL_YELLOW;
		if (worst >= 25) return COLOR_VITAL_ORANGE;
		return COLOR_VITAL_RED;
	}

	protected int VitalPctColor(int pct)
	{
		if (pct < 0)    return COLOR_TEXT_DIM;
		if (pct >= 70)  return COLOR_VITAL_GREEN;
		if (pct >= 50)  return COLOR_VITAL_YELLOW;
		if (pct >= 25)  return COLOR_VITAL_ORANGE;
		return COLOR_VITAL_RED;
	}

	protected int TempPctColor(int pct)
	{
		if (pct < 0) return COLOR_TEXT_DIM;
		int dist = pct - 50;
		if (dist < 0) dist = -dist;
		int equivalent = 100 - dist * 2;
		return VitalPctColor(equivalent);
	}

	protected string AppendSymptom(SymptomManager mgr, int symptomId, string label, string alerts)
	{
		if (!mgr) return alerts;
		if (mgr.GetSymptomCount(symptomId) <= 0) return alerts;
		if (alerts != "") return alerts + ", " + label;
		return label;
	}

	protected string GetFloppyDetailText(REV_TerritoryComputer computer)
	{
		if (!computer) return "--";
		if (!computer.IsTurnedOn()) return "OFFLINE";
		EntityAI floppy = computer.FindAttachmentBySlotName("Material_FPole_Flag");
		if (!floppy) return "NONE";
		if (!computer.HasValidFloppy()) return "INVALID";

		float fraction = GetClientFloppyFraction(computer);
		float duration = computer.GetEffectiveRefresherDuration();
		float remainingSec = fraction * duration;
		int pct = Math.Round(fraction * 100);

		return pct.ToString() + "% (" + FormatTimeRemaining(remainingSec) + ")";
	}

	protected string GetFloppyShortText(REV_TerritoryComputer computer)
	{
		if (!computer) return "--";
		if (!computer.IsTurnedOn()) return "OFFLINE";
		EntityAI floppy = computer.FindAttachmentBySlotName("Material_FPole_Flag");
		if (!floppy) return "NO MEDIA";
		if (!computer.HasValidFloppy()) return "INVALID";

		float fraction = GetClientFloppyFraction(computer);
		int pct = Math.Round(fraction * 100);
		return pct.ToString() + "%";
	}

	protected string BuildIntegrityBar(float fraction, int width)
	{
		int filled = Math.Round(fraction * width);
		if (filled < 0) filled = 0;
		if (filled > width) filled = width;
		string bar = "";
		for (int i = 0; i < width; i++)
		{
			if (i < filled) bar = bar + "#";
			else bar = bar + ".";
		}
		return bar;
	}

	protected string BuildDiagnosticBlock(REV_TerritoryComputer computer)
	{
		string s = "";
		s = s + "[ SYS DIAGNOSTICS ]\n";
		s = s + "\n";

		string protectionLabel = "INACTIVE";
		int protLabelLen = 8;
		if (computer && computer.CanTerritoryBeActive())
		{
			protectionLabel = "ACTIVE";
			protLabelLen = 6;
		}
		s = s + "  PROTECT  : " + protectionLabel + "\n";

		float fraction = 0;
		if (computer) fraction = computer.GetRefresherTime01();
		int pct = Math.Round(fraction * 100);
		string bar = BuildIntegrityBar(fraction, 18);
		s = s + "  INTEGRITY: " + pct.ToString() + "%\n";
		s = s + "  [" + bar + "]\n";
		s = s + "\n";

		float refreshSec = GetMultiTerritoriesConfig().FlagRefreshFrequency;
		s = s + "  CYCLE    : " + FormatTimeRemaining(refreshSec) + "\n";
		float radius = GetMultiTerritoriesConfig().GetProtectionRadius();
		int rad = Math.Round(radius);
		s = s + "  RADIUS   : " + rad.ToString() + "m\n";

		if (computer)
		{
			vector pos = computer.GetPosition();
			int px = Math.Round(pos[0]);
			int pz = Math.Round(pos[2]);
			s = s + "  COORD    : " + px.ToString() + ", " + pz.ToString() + "\n";
		}

		s = s + "\n";
		s = s + "  > AWAITING ENROLLMENT\n";
		s = s + "  > PRESS [F2] TO INVITE";

		return s;
	}

	protected string FormatTimeRemaining(float seconds)
	{
		if (seconds < 60) return "<1m";

		int totalMinutes = Math.Floor(seconds / 60);
		int days = totalMinutes / 1440;
		int hours = (totalMinutes / 60) - (days * 24);
		int minutes = totalMinutes - (days * 1440) - (hours * 60);

		if (days > 0)
		{
			return days.ToString() + "d " + hours.ToString() + "h";
		}

		if (hours > 0)
		{
			return hours.ToString() + "h " + minutes.ToString() + "m";
		}

		return minutes.ToString() + "m";
	}

	override protected void Refresh()
	{
		REV_TerritoryComputer targetComputer = GetTargetComputer();
		if (!targetComputer) return;

		RequestVitalsFromServer(targetComputer);

		string selfGuid = GetPlayerGuid();
		bool isOwner = targetComputer.IsTerritoryOwner(selfGuid);
		bool isMember = targetComputer.IsTerritoryMember(selfGuid);
		bool isServerAdmin = GetMultiTerritoriesConfig().ServerAdmins.Find(selfGuid) != -1;
		bool canEditPerms = isOwner || isServerAdmin;
		bool canClaim = targetComputer.CanReceiveNewOwner();
		bool canResetAdmin = !canClaim && targetComputer.GetMemberCount() == 0 && (isOwner || isServerAdmin);
		bool canResetMembers = targetComputer.CheckPlayerPermission(selfGuid, TerritoryPerm.REMOVEMEMBER);
		bool canInvite = targetComputer.CheckPlayerPermission(selfGuid, TerritoryPerm.ADDMEMBER);
		bool inviteActive = targetComputer.CanAddMember();
		bool canJoin = inviteActive && !isMember;
		bool canLeave = isMember && !isOwner;
		bool hasFloppyAttached = targetComputer.FindAttachmentBySlotName("Material_FPole_Flag") != null;
		bool canEjectFloppy = (targetComputer.CanReceiveNewOwner() || targetComputer.CheckPlayerPermission(selfGuid, TerritoryPerm.ADMIN)) && hasFloppyAttached;

		BuildHeaderBlock(targetComputer, selfGuid);
		BuildOrgTreeText(targetComputer, selfGuid);
		RefreshDetailPanel(targetComputer, selfGuid, canEditPerms);
		RefreshCommandBar(canClaim, canResetAdmin, canResetMembers, canInvite, inviteActive, canJoin, canLeave, canEjectFloppy);
	}

	protected void BuildHeaderBlock(REV_TerritoryComputer targetComputer, string selfGuid)
	{
		if (m_HeaderBlockText)
		{
			string state = "OFFLINE";
			if (targetComputer.IsTurnedOn())
			{
				if (targetComputer.IsFloppyLocked() && targetComputer.HasValidFloppy()) state = "ONLINE ";
				else state = "STANDBY";
			}

			string ownerName = targetComputer.GetTerritoryOwnerDisplayName();
			int memberCount = targetComputer.GetMemberCount();
			int memberLimit = GetMultiTerritoriesConfig().MaxMembersPerTerritory;
			if (memberLimit <= 0) memberLimit = 8;
			string invite = "CLOSED";
			if (targetComputer.CanAddMember()) invite = "OPEN  ";

			string playerName = GetPlayerName();
			string role = "VISITOR  ";
			if (targetComputer.IsTerritoryOwner(selfGuid)) role = "COMMANDER";
			else if (targetComputer.IsTerritoryMember(selfGuid)) role = "MEMBER   ";

			string floppyStr = GetFloppyDetailText(targetComputer);
			string uptime = FormatClock(Math.Floor(m_SessionTime));

			vector pos = targetComputer.GetPosition();
			int px = Math.Round(pos[0]);
			int pz = Math.Round(pos[2]);
			string coord = px.ToString() + "," + pz.ToString();

			string tempStr = (m_TelemetryTemp / 10).ToString() + "." + (m_TelemetryTemp % 10).ToString() + "C";
			string cpuBar = BuildBar(m_TelemetryCpu / 100.0, 10);
			string memBar = BuildBar(m_TelemetryMem / 100.0, 10);

			string ledPwr = "[+]";
			string ledAuth = "[+]";
			string ledNet = "[+]";
			if (Math.RandomIntInclusive(0, 30) == 0) ledNet = "[-]";
			string ledMem = "[+]";
			if (m_TelemetryMem > 85) ledMem = "[!]";

			string threat = "LOW   ";
			if (state == "STANDBY") threat = "MED   ";
			if (targetComputer.CanAddMember()) threat = "MED   ";
			if (memberCount >= memberLimit) threat = "HIGH  ";

			string l1 = "ELYSIAN OS v5.7.2  // MULTITERRITORY // (c) Multiplix 1986    " + ledPwr + "PWR  " + ledAuth + "AUTH  " + ledNet + "NET  " + ledMem + "MEM";
			string l2 = "> STATUS  " + PadRight(state, 9) + "   > OWNER  " + PadRight(ownerName, 12) + "   > INVITE " + PadRight(invite, 7) + "   > THREAT " + threat;
			string l3 = "> FLOPPY  " + PadRight(floppyStr, 14) + "   > MEMBERS " + memberCount.ToString() + "/" + memberLimit.ToString() + "          > SESSION " + PadRight(playerName, 10) + "  > UP " + uptime;
			string l4 = "> CPU [" + cpuBar + "] " + PadLeft(m_TelemetryCpu.ToString(), 2) + "%   > MEM [" + memBar + "] " + PadLeft(m_TelemetryMem.ToString(), 2) + "%   > NET " + PadLeft(m_TelemetryNet.ToString(), 3) + " KB/s    > TEMP " + tempStr + "  > " + coord + "m";

			m_HeaderBlockText.SetText(l1 + "\n" + l2 + "\n" + l3 + "\n" + l4);

			int headerColor = COLOR_TEXT;
			if (state == "ONLINE ") headerColor = COLOR_TEXT_BRIGHT;
			else if (state == "STANDBY") headerColor = COLOR_TEXT_AMBER;
			m_HeaderBlockText.SetColor(headerColor);
			m_HeaderBlockText.Show(true);
		}

		if (m_FloppyStatusText)
		{
			if (targetComputer.IsTurnedOn() && targetComputer.HasValidFloppy())
			{
				float floppyFrac = GetClientFloppyFraction(targetComputer);
				float floppyDur = targetComputer.GetEffectiveRefresherDuration();
				int floppyPct = Math.Round(floppyFrac * 100);
				string remaining = FormatTimeRemaining(floppyFrac * floppyDur);
				m_FloppyStatusText.SetText("[O] FLOPPY " + floppyPct.ToString() + "%  " + remaining);
				m_FloppyStatusText.SetColor(COLOR_TEXT_BRIGHT);
			}
			else
			{
				m_FloppyStatusText.SetText("[ ] NO MEDIA");
				m_FloppyStatusText.SetColor(COLOR_TEXT_AMBER_DIM);
			}
		}

		if (m_HintText)
		{
			m_HintText.SetText("> CLICK NODE TO INSPECT  |  CLICK [ ] TO TOGGLE PERMISSIONS  |  CONFIRM REQUIRED FOR DESTRUCTIVE ACTIONS");
		}
	}

	protected void BuildOrgTreeText(REV_TerritoryComputer targetComputer, string selfGuid)
	{
		TStringArray members = targetComputer.TerritoryMembers();
		if (!members) members = new TStringArray;

		m_VisibleMemberGuids.Clear();
		int totalMembers = members.Count();

		int memberLimitForScroll = GetMultiTerritoriesConfig().MaxMembersPerTerritory;
		if (memberLimitForScroll <= 0) memberLimitForScroll = 8;
		int totalSlots = totalMembers;
		if (memberLimitForScroll > totalSlots) totalSlots = memberLimitForScroll;
		if (MIN_SCROLL_PREVIEW_SLOTS > totalSlots) totalSlots = MIN_SCROLL_PREVIEW_SLOTS;

		int maxOffset = totalSlots - MAX_VISIBLE_MEMBERS;
		if (maxOffset < 0) maxOffset = 0;
		if (m_OrgScrollOffset < 0) m_OrgScrollOffset = 0;
		if (m_OrgScrollOffset > maxOffset) m_OrgScrollOffset = maxOffset;

		int remaining = totalMembers - m_OrgScrollOffset;
		int visible = Math.Min(remaining, MAX_VISIBLE_MEMBERS);
		if (visible < 0) visible = 0;
		for (int i = 0; i < visible; i++)
		{
			m_VisibleMemberGuids.Insert(members[m_OrgScrollOffset + i]);
		}

		UpdateScrollThumb(totalSlots);

		if (m_SelectedMemberGuid != "" && m_SelectedMemberGuid != targetComputer.GetTerritoryOwnerId())
		{
			bool found = false;
			for (int k = 0; k < m_VisibleMemberGuids.Count(); k++)
			{
				if (m_VisibleMemberGuids[k] == m_SelectedMemberGuid)
				{
					found = true;
					break;
				}
			}
			if (!found) m_SelectedMemberGuid = "";
		}

		if (m_SelectedEmptySlotRow >= 0 && m_SelectedEmptySlotRow < visible)
		{
			m_SelectedEmptySlotRow = -1;
		}

		string ownerGuid = targetComputer.GetTerritoryOwnerId();
		bool hasOwner = ownerGuid != "";
		string ownerName = "AWAITING ADMIN";
		if (hasOwner) ownerName = targetComputer.GetTerritoryOwnerDisplayName();

		int memberLimit = GetMultiTerritoriesConfig().MaxMembersPerTerritory;
		if (memberLimit <= 0) memberLimit = 8;

		string ownerPrefix = "  ";
		if (hasOwner && m_SelectedMemberGuid == ownerGuid) ownerPrefix = ">>";

		string ownerAuthCode = "[--------]";
		if (hasOwner)
		{
			int slowTick = Math.Floor(m_SessionTime * 0.7);
			int ownerHash = ownerGuid.Hash();
			int variantIdx = (ownerHash ^ slowTick) % 20;
			if (variantIdx < 0) variantIdx = -variantIdx;

			string variant = "0xLLM_MPLX_D3L74_1011";
			if      (variantIdx ==  1) variant = "MPLX:0b101::LLAMA::D37A";
			else if (variantIdx ==  2) variant = "0110_LLAMA_0xMPLX_DELTA";
			else if (variantIdx ==  3) variant = "0xLM4_MP7X_D1T4Q_BEEF";
			else if (variantIdx ==  4) variant = "D37A_L14MA_1100_MPLX";
			else if (variantIdx ==  5) variant = "MPLX_1011_LLA_D3L74";
			else if (variantIdx ==  6) variant = "0xD37A::MPLX:0110::LLAMA";
			else if (variantIdx ==  7) variant = "LLA_1101_MPLX_0011_DLT";
			else if (variantIdx ==  8) variant = "011011_MPLX_1LAM4_D37A";
			else if (variantIdx ==  9) variant = "D37A_MPLX_L14M4_0xA0";
			else if (variantIdx == 10) variant = "2x11xG4S_MPLX_D37A";
			else if (variantIdx == 11) variant = "LLA:0110:MPLX:I1xGG4:D37A";
			else if (variantIdx == 12) variant = "MPLX_D37A_010_L1AM4";
			else if (variantIdx == 13) variant = "101_DEL7AQ_1101_MPLX_LM";
			else if (variantIdx == 14) variant = "0xA000_LM_B00_MPLX_D37A";
			else if (variantIdx == 15) variant = "D374Q_MPLX_LM_11xGG4S";
			else if (variantIdx == 16) variant = "MPLX::CACHE::L1AM4:D37AQ";
			else if (variantIdx == 17) variant = "1001_LLA_111000_MPLX_D37AQ";
			else if (variantIdx == 18) variant = "L1AM4_MUL71_D3LT4Q_1011";
			else if (variantIdx == 19) variant = "0xD37A_0xMPLX_0xLLA";
			ownerAuthCode = "[" + variant + "]";
		}

		string tree = "";
		if (hasOwner)
		{
			tree = tree + ownerPrefix + " [01] COMMANDER " + PadRight(ownerName, 14) + " " + ownerAuthCode + "\n";
		}
		else
		{
			tree = tree + "   [01] UNCLAIMED -- awaiting admin --\n";
		}

		for (int j = 0; j < MAX_VISIBLE_MEMBERS; j++)
		{
			int absoluteSlot = m_OrgScrollOffset + j + 2;
			string slotNumStr = absoluteSlot.ToString();
			if (absoluteSlot < 10) slotNumStr = "0" + slotNumStr;

			string connector = "  L ";

			if (j < visible)
			{
				string memberGuid = m_VisibleMemberGuids[j];
				string memberName = targetComputer.ResolveIdentityName(memberGuid);
				int perms = targetComputer.GetMemberPermissions(memberGuid);
				string badge = BuildPermBadge(perms);
				string roleLabel = "MEMBER ";
				if ((perms & TerritoryPerm.ADMIN) != 0) roleLabel = "OFFICER";
				string prefix = connector;
				if (m_SelectedMemberGuid == memberGuid) prefix = ">>" + connector.Substring(2, connector.Length() - 2);
				tree = tree + prefix + "[" + slotNumStr + "] " + roleLabel + " " + PadRight(memberName, 14) + " " + badge + "\n";
			}
			else
			{
				tree = tree + connector + "[" + slotNumStr + "]  -- vacant slot --\n";
			}
		}

		if (m_OrgTreeText)
		{
			m_OrgTreeText.SetText(tree);
			m_OrgTreeText.SetColor(COLOR_TEXT_BRIGHT);
		}

		int totalShown = visible;
		if (hasOwner) totalShown = totalShown + 1;
		string enrollSuffix = "  ENROLLMENT  " + totalShown.ToString() + " / " + (memberLimit + 1).ToString();
		if (totalMembers > MAX_VISIBLE_MEMBERS)
		{
			int extra = totalMembers - MAX_VISIBLE_MEMBERS;
			enrollSuffix = enrollSuffix + "  +" + extra.ToString() + " HIDDEN";
		}
		if (m_OrgTitleText) m_OrgTitleText.SetText("[ ORGANIZATION TREE ]" + enrollSuffix);

		if (m_OwnerButton)
		{
			m_OwnerButton.Show(hasOwner);
			m_OwnerButton.Enable(hasOwner);
		}

		bool ownerSelected = hasOwner && m_SelectedMemberGuid == ownerGuid;
		if (m_OwnerNodeBgImg) m_OwnerNodeBgImg.Show(ownerSelected);

		for (int b = 0; b < MAX_VISIBLE_MEMBERS; b++)
		{
			bool slotActive = b < visible;
			if (m_MemberButtons[b])
			{
				m_MemberButtons[b].Show(true);
				m_MemberButtons[b].Enable(true);
			}
			if (m_MemberNodeBgImgs && b < m_MemberNodeBgImgs.Count() && m_MemberNodeBgImgs[b])
			{
				bool memberSelected = false;
				if (slotActive && b < m_VisibleMemberGuids.Count())
				{
					memberSelected = (m_SelectedMemberGuid == m_VisibleMemberGuids[b]);
				}
				else if (!slotActive && m_SelectedEmptySlotRow == b)
				{
					memberSelected = true;
				}
				m_MemberNodeBgImgs[b].Show(memberSelected);
			}
		}
	}

	protected void RenderEmptySlotDetail(int slotNumber)
	{
		string slotStr = slotNumber.ToString();
		if (slotNumber < 10) slotStr = "0" + slotStr;

		if (m_DetailTitleText) m_DetailTitleText.SetText("[ EMPTY SLOT ]");

		if (m_MemberDetailText)
		{
			string detail = "";
			detail = detail + "> NAME    -- VACANT --\n";
			detail = detail + "> ROLE    EMPTY\n";
			detail = detail + "> ID      [SLOT " + slotStr + "]\n";
			detail = detail + "> JOINED  --             > DIST   --\n";
			detail = detail + "> STATUS  unoccupied     > TRUST  --\n";
			detail = detail + "> BIO     --\n";
			detail = detail + "> ALRT    -- unoccupied slot --";
			m_MemberDetailText.SetText(detail);
			m_MemberDetailText.SetColor(COLOR_TEXT_DIM);
		}

		if (m_DetailPermsHeader) m_DetailPermsHeader.SetText("[ ACCESS RIGHTS ]   0 / " + PERM_ROW_COUNT.ToString());

		if (m_AccessRightsText)
		{
			string blank = "";
			for (int i = 0; i < PERM_ROW_COUNT; i++)
			{
				blank = blank + "[ ]  " + m_PermLabels[i] + "  (vacant)";
				if (i < PERM_ROW_COUNT - 1) blank = blank + "\n";
			}
			m_AccessRightsText.SetText(blank);
			m_AccessRightsText.SetColor(COLOR_TEXT_DIM);
		}

		for (int p = 0; p < PERM_ROW_COUNT; p++)
		{
			m_PermEnabledFlags[p] = false;
			if (m_PermButtons[p])
			{
				m_PermButtons[p].Enable(false);
				m_PermButtons[p].Show(true);
			}
		}

		if (m_KickButton)     m_KickButton.Show(false);
		if (m_KickButtonBg)   m_KickButtonBg.Show(false);
		if (m_KickButtonBgImg) m_KickButtonBgImg.Show(false);
	}

	protected void RefreshDetailPanel(REV_TerritoryComputer targetComputer, string selfGuid, bool canEditPerms)
	{
		if (m_SelectedMemberGuid == "" && m_SelectedEmptySlotRow >= 0)
		{
			int vacantSlotNum = m_OrgScrollOffset + m_SelectedEmptySlotRow + 2;
			RenderEmptySlotDetail(vacantSlotNum);
			return;
		}

		string ownerGuid = targetComputer.GetTerritoryOwnerId();
		string subjectGuid = m_SelectedMemberGuid;
		bool fellbackToSelf = false;

		if (subjectGuid == "")
		{
			subjectGuid = selfGuid;
			fellbackToSelf = true;
		}

		bool subjectIsOwner = (subjectGuid == ownerGuid) && (ownerGuid != "");
		bool subjectIsSelf = (subjectGuid == selfGuid);

		string subjectName;
		string subjectRole;
		int subjectPerms;

		if (subjectIsOwner)
		{
			subjectName = targetComputer.GetTerritoryOwnerDisplayName();
			subjectRole = "COMMANDER";
			subjectPerms = TerritoryFlag.GetAllPermissionBits();
		}
		else if (targetComputer.IsTerritoryMember(subjectGuid))
		{
			subjectName = targetComputer.ResolveIdentityName(subjectGuid);
			subjectRole = "MEMBER";
			subjectPerms = targetComputer.GetMemberPermissions(subjectGuid);
		}
		else
		{
			if (subjectIsSelf)
			{
				subjectName = GetPlayerName();
				subjectRole = "VISITOR";
			}
			else
			{
				subjectName = targetComputer.ResolveIdentityName(subjectGuid);
				subjectRole = "UNKNOWN";
			}
			subjectPerms = GetMultiTerritoriesConfig().PublicPermissions;
		}

		string headerSuffix = "";
		if (fellbackToSelf) headerSuffix = " (YOU)";
		else if (subjectIsSelf) headerSuffix = " (YOU)";

		if (m_MemberDetailText)
		{
			string shortId = subjectGuid;
			if (shortId.Length() > 20) shortId = shortId.Substring(0, 8) + "..." + shortId.Substring(shortId.Length() - 6, 6);

			int hashSeed = subjectGuid.Hash();
			if (hashSeed < 0) hashSeed = -hashSeed;
			int joinedDays = (hashSeed % 89) + 1;
			int joinedHours = (hashSeed / 89) % 24;
			string joinedStr = joinedDays.ToString() + "d " + joinedHours.ToString() + "h ago";

			string distStr = "--";
			if (subjectIsSelf)
			{
				PlayerBase localPlayer = GetPlayer();
				if (localPlayer)
				{
					float dist = vector.Distance(localPlayer.GetPosition(), targetComputer.GetPosition());
					int distInt = Math.Round(dist);
					distStr = distInt.ToString() + "m";
				}
			}

			string statusStr = "--";
			if (subjectIsSelf) statusStr = "ONLINE";
			else if (targetComputer.IsTerritoryMember(subjectGuid)) statusStr = "OFFLINE";
			else statusStr = "TRANSIENT";

			int permCount = 0;
			for (int pp = 0; pp < PERM_ROW_COUNT; pp++)
			{
				if ((subjectPerms & m_PermBitsTable[pp]) != 0) permCount++;
			}
			string trustStr = "LOW";
			if (subjectIsOwner) trustStr = "MAX";
			else if (permCount >= 6) trustStr = "HIGH";
			else if (permCount >= 3) trustStr = "MID";

			string bioLine = "> BIO     -- awaiting vitals --";
			string alrtLine = "> ALRT    --";

			if (subjectIsSelf && MultiplixTerritoryMenuState.s_VitalHpPct >= 0)
			{
				int hpPct  = MultiplixTerritoryMenuState.s_VitalHpPct;
				int blPct  = MultiplixTerritoryMenuState.s_VitalBlPct;
				int tmpPct = MultiplixTerritoryMenuState.s_VitalTmpPct;
				int waPct  = MultiplixTerritoryMenuState.s_VitalWaPct;
				int enPct  = MultiplixTerritoryMenuState.s_VitalEnPct;
				int symBits = MultiplixTerritoryMenuState.s_VitalSymptomBits;

				int tmpDeviation = tmpPct - 50;
				if (tmpDeviation < 0) tmpDeviation = -tmpDeviation;

				string condition = "[NOMINAL]";
				int worstStat = 0;
				if (hpPct < 50 || blPct < 50 || waPct < 50 || enPct < 50 || tmpDeviation > 25) worstStat = 1;
				if (hpPct < 25 || blPct < 25 || waPct < 25 || enPct < 25 || tmpDeviation > 40) worstStat = 2;
				if (worstStat == 1) condition = "[WARN]";
				if (worstStat == 2) condition = "[CRIT]";

				bioLine = "> BIO     HP " + PadLeft(hpPct.ToString(),  3) + "%  BL " + PadLeft(blPct.ToString(),  3) + "%  T " + PadLeft(tmpPct.ToString(), 3) + "%  WA " + PadLeft(waPct.ToString(),  3) + "%  EN " + PadLeft(enPct.ToString(),  3) + "%  " + condition;

				string alerts = "";
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_BLEED) != 0)      alerts = AppendAlert(alerts, "BLEED");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_PAIN_HEAVY) != 0) alerts = AppendAlert(alerts, "PAIN+");
				else if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_PAIN) != 0)  alerts = AppendAlert(alerts, "PAIN");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_COUGH) != 0)      alerts = AppendAlert(alerts, "COUGH");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_VOMIT) != 0)      alerts = AppendAlert(alerts, "VOMIT");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_FEVER) != 0)      alerts = AppendAlert(alerts, "FEVER");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_COLD) != 0)       alerts = AppendAlert(alerts, "COLD");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_HOT) != 0)        alerts = AppendAlert(alerts, "HOT");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_WET) != 0)        alerts = AppendAlert(alerts, "WET");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_BLIND) != 0)      alerts = AppendAlert(alerts, "BLIND");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_DEAF) != 0)       alerts = AppendAlert(alerts, "DEAF");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_SHAKE) != 0)      alerts = AppendAlert(alerts, "SHAKE");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_UNCNS) != 0)      alerts = AppendAlert(alerts, "UNCNS");
				if ((symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_INFCT) != 0)      alerts = AppendAlert(alerts, "INFCT");
				if (waPct < 25) alerts = AppendAlert(alerts, "DEHYD");
				if (enPct < 25) alerts = AppendAlert(alerts, "STARV");

				bool coldPresent = (symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_COLD) != 0;
				bool hotPresent  = (symBits & MULTITERRITORY_VITAL_FLAGS.VFLAG_HOT)  != 0;
				if (tmpPct < 30 && !coldPresent) alerts = AppendAlert(alerts, "COLD");
				if (tmpPct > 70 && !hotPresent)  alerts = AppendAlert(alerts, "HOT");

				if (alerts == "") alerts = "nominal";
				alrtLine = "> ALRT    " + alerts;
			}
			else if (!subjectIsSelf)
			{
				bioLine = "> BIO     -- remote telemetry offline --";
				alrtLine = "> ALRT    --";
			}

			string detail = "";
			detail = detail + "> NAME    " + subjectName + headerSuffix + "\n";
			detail = detail + "> ROLE    " + subjectRole + "\n";
			detail = detail + "> ID      " + shortId + "\n";
			detail = detail + "> JOINED  " + PadRight(joinedStr, 12) + "  > DIST   " + distStr + "\n";
			detail = detail + "> STATUS  " + PadRight(statusStr, 12) + "  > TRUST  " + trustStr + "\n";
			detail = detail + bioLine + "\n";
			detail = detail + alrtLine;

			m_MemberDetailText.SetText(detail);
			if (subjectIsOwner) m_MemberDetailText.SetColor(COLOR_TEXT_AMBER);
			else m_MemberDetailText.SetColor(COLOR_TEXT_BRIGHT);
		}

		bool canEditThisSubject = canEditPerms && !subjectIsOwner && targetComputer.IsTerritoryMember(subjectGuid) && !subjectIsSelf;
		int grantedCount = 0;
		string ar = "";

		for (int i = 0; i < PERM_ROW_COUNT; i++)
		{
			int bit = m_PermBitsTable[i];
			bool granted = (subjectPerms & bit) != 0;
			bool toggleable = canEditThisSubject;
			if (bit == TerritoryPerm.ADMIN) toggleable = false;
			m_PermEnabledFlags[i] = toggleable;

			string check = "[ ]";
			if (granted)
			{
				grantedCount++;
				if (subjectIsOwner) check = "[*]";
				else check = "[X]";
			}

			string lockMark = "";
			if (!toggleable && !subjectIsOwner) lockMark = "  (locked)";

			ar = ar + check + "  " + m_PermLabels[i] + lockMark;
			if (i < PERM_ROW_COUNT - 1) ar = ar + "\n";

			if (m_PermButtons[i])
			{
				m_PermButtons[i].Enable(toggleable);
				m_PermButtons[i].Show(true);
			}
		}

		if (m_AccessRightsText)
		{
			m_AccessRightsText.SetText(ar);
			if (subjectIsOwner) m_AccessRightsText.SetColor(COLOR_TEXT_AMBER);
			else if (canEditThisSubject) m_AccessRightsText.SetColor(COLOR_TEXT_BRIGHT);
			else m_AccessRightsText.SetColor(COLOR_TEXT);
		}

		if (m_DetailPermsHeader)
		{
			string permsLabel = "[ ACCESS RIGHTS ]   " + grantedCount.ToString() + " / " + PERM_ROW_COUNT.ToString();
			if (subjectIsOwner) permsLabel = "[ COMMANDER RIGHTS ]   " + grantedCount.ToString() + " / " + PERM_ROW_COUNT.ToString();
			m_DetailPermsHeader.SetText(permsLabel);
		}

		if (m_DetailTitleText)
		{
			if (subjectIsOwner) m_DetailTitleText.SetText("[ COMMANDER RECORD ]");
			else m_DetailTitleText.SetText("[ MEMBER RECORD ]");
		}

		bool canKick = canEditPerms && !subjectIsOwner && targetComputer.IsTerritoryMember(subjectGuid) && !subjectIsSelf;
		if (m_KickButton)
		{
			m_KickButton.Show(canKick);
			m_KickButton.Enable(canKick);
			if (canKick)
			{
				if (m_ConfirmKick)
				{
					m_KickButton.SetText("--- CONFIRM KICK ---");
					m_KickButton.SetColor(COLOR_TEXT_RED_BRIGHT);
					if (m_KickButtonBgImg) m_KickButtonBgImg.SetColor(COLOR_DESTRUCTIVE_HOVER);
				}
				else
				{
					m_KickButton.SetText("--- KICK MEMBER ---");
					m_KickButton.SetColor(COLOR_TEXT_RED);
					if (m_KickButtonBgImg) m_KickButtonBgImg.SetColor(COLOR_DESTRUCTIVE);
				}
			}
		}
		if (m_KickButtonBg) m_KickButtonBg.Show(canKick);
		if (m_KickButtonBgImg) m_KickButtonBgImg.Show(canKick);
	}

	protected void RefreshCommandBar(bool canClaim, bool canResetAdmin, bool canResetMembers, bool canInvite, bool inviteActive, bool canJoin, bool canLeave, bool canEjectFloppy)
	{
		SetCommandButton(m_ClaimButton, m_ClaimButtonBg, m_ClaimButtonBgImg, canClaim, "[F1] CLAIM", false);
		SetCommandButton(m_ResetAdminButton, m_ResetAdminButtonBg, m_ResetAdminButtonBgImg, canResetAdmin, "[F6] RESET", false);

		string inviteText = "[F2] INVITE";
		if (inviteActive) inviteText = "[F2] REVOKE";
		SetCommandButton(m_InviteButton, m_InviteButtonBg, m_InviteButtonBgImg, canInvite, inviteText, false);

		string purgeText = "[F3] PURGE";
		if (m_ConfirmResetMembers) purgeText = "[F3] CONFIRM";
		SetCommandButton(m_ResetMembersButton, m_ResetMembersButtonBg, m_ResetMembersButtonBgImg, canResetMembers, purgeText, m_ConfirmResetMembers);

		bool showJoinLeave = canJoin || canLeave;
		string joinLeaveText = "[F4] JOIN";
		if (canLeave) joinLeaveText = "[F4] LEAVE";
		SetCommandButton(m_JoinLeaveButton, m_JoinLeaveButtonBg, m_JoinLeaveButtonBgImg, showJoinLeave, joinLeaveText, false);

		string ejectText = "[F5] EJECT";
		if (m_ConfirmEjectFloppy) ejectText = "[F5] CONFIRM";
		SetCommandButton(m_EjectFloppyButton, m_EjectFloppyButtonBg, m_EjectFloppyButtonBgImg, canEjectFloppy, ejectText, m_ConfirmEjectFloppy);

		SetCommandButton(m_CloseButton, m_CloseButtonBg, m_CloseButtonBgImg, true, "[ESC] EXIT", false);
	}

	protected void SetCommandButton(ButtonWidget button, Widget bgPanel, ImageWidget bgImg, bool enabled, string text, bool destructiveConfirm)
	{
		if (button)
		{
			button.Show(enabled);
			button.Enable(enabled);
			button.SetText(text);
			if (!enabled) button.SetColor(COLOR_TEXT_DISABLED);
			else if (destructiveConfirm) button.SetColor(COLOR_TEXT_RED_BRIGHT);
			else button.SetColor(COLOR_TEXT);
		}

		if (bgPanel) bgPanel.Show(enabled);

		if (bgImg)
		{
			bgImg.Show(enabled);
			if (!enabled) bgImg.SetColor(COLOR_BUTTON_DISABLED);
			else if (destructiveConfirm) bgImg.SetColor(COLOR_DESTRUCTIVE_HOVER);
			else bgImg.SetColor(COLOR_BUTTON);
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		ApplyHoverState(w, true);
		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		ApplyHoverState(w, false);
		return super.OnMouseLeave(w, enterW, x, y);
	}

	protected void ApplyHoverState(Widget w, bool hovered)
	{
		if (!w) return;

		if (w == m_OwnerButton && m_OwnerNodeBgImg)
		{
			string ownerGuid = "";
			REV_TerritoryComputer comp = GetTargetComputer();
			if (comp) ownerGuid = comp.GetTerritoryOwnerId();
			bool selected = m_SelectedMemberGuid == ownerGuid && ownerGuid != "";
			if (selected) m_OwnerNodeBgImg.SetColor(COLOR_OWNER_SELECTED_BG);
			else if (hovered) m_OwnerNodeBgImg.SetColor(COLOR_OWNER_HOVER_BG);
			else m_OwnerNodeBgImg.SetColor(COLOR_OWNER_BG);
			return;
		}

		for (int i = 0; i < MAX_VISIBLE_MEMBERS; i++)
		{
			if (w == m_MemberButtons[i] && m_MemberNodeBgImgs[i])
			{
				bool memberSelected = false;
				if (i < m_VisibleMemberGuids.Count())
					memberSelected = m_SelectedMemberGuid == m_VisibleMemberGuids[i];
				if (memberSelected) m_MemberNodeBgImgs[i].SetColor(COLOR_NODE_SELECTED_BG);
				else if (hovered) m_MemberNodeBgImgs[i].SetColor(COLOR_NODE_HOVER_BG);
				else m_MemberNodeBgImgs[i].SetColor(COLOR_NODE_BG);
				return;
			}
		}

		for (int j = 0; j < PERM_ROW_COUNT; j++)
		{
			if (w == m_PermButtons[j] && m_PermRowBgImgs[j])
			{
				bool enabled = false;
				if (j < m_PermEnabledFlags.Count()) enabled = m_PermEnabledFlags[j];
				if (!enabled)
				{
					m_PermRowBgImgs[j].SetColor(COLOR_BUTTON_DISABLED);
					return;
				}
				if (hovered) m_PermRowBgImgs[j].SetColor(COLOR_BUTTON_HOVER);
				else m_PermRowBgImgs[j].SetColor(COLOR_BUTTON);
				return;
			}
		}

		ApplyCommandHover(w, hovered);
	}

	protected void ApplyCommandHover(Widget w, bool hovered)
	{
		ImageWidget bg = null;
		bool isDestructive = false;
		if (w == m_ClaimButton) bg = m_ClaimButtonBgImg;
		else if (w == m_InviteButton) bg = m_InviteButtonBgImg;
		else if (w == m_ResetMembersButton) { bg = m_ResetMembersButtonBgImg; isDestructive = m_ConfirmResetMembers; }
		else if (w == m_JoinLeaveButton) bg = m_JoinLeaveButtonBgImg;
		else if (w == m_EjectFloppyButton) { bg = m_EjectFloppyButtonBgImg; isDestructive = m_ConfirmEjectFloppy; }
		else if (w == m_ResetAdminButton) bg = m_ResetAdminButtonBgImg;
		else if (w == m_CloseButton) bg = m_CloseButtonBgImg;
		else if (w == m_KickButton)
		{
			if (!m_KickButtonBgImg) return;
			if (m_ConfirmKick) m_KickButtonBgImg.SetColor(COLOR_DESTRUCTIVE_HOVER);
			else if (hovered) m_KickButtonBgImg.SetColor(COLOR_DESTRUCTIVE_HOVER);
			else m_KickButtonBgImg.SetColor(COLOR_DESTRUCTIVE);
			return;
		}

		if (!bg) return;
		if (w && !w.IsVisible())
		{
			bg.SetColor(COLOR_BUTTON_DISABLED);
			return;
		}
		if (isDestructive) bg.SetColor(COLOR_DESTRUCTIVE_HOVER);
		else if (hovered) bg.SetColor(COLOR_BUTTON_HOVER);
		else bg.SetColor(COLOR_BUTTON);
	}

	protected void SelectOwner()
	{
		REV_TerritoryComputer comp = GetTargetComputer();
		if (!comp) return;
		string ownerGuid = comp.GetTerritoryOwnerId();
		if (ownerGuid == "") return;
		m_SelectedMemberGuid = ownerGuid;
		m_SelectedEmptySlotRow = -1;
		ClearAllConfirms();
		Refresh();
	}

	protected void SelectMember(int idx)
	{
		if (idx < 0 || idx >= MAX_VISIBLE_MEMBERS) return;
		if (idx < m_VisibleMemberGuids.Count())
		{
			m_SelectedMemberGuid = m_VisibleMemberGuids[idx];
			m_SelectedEmptySlotRow = -1;
		}
		else
		{
			m_SelectedMemberGuid = "";
			m_SelectedEmptySlotRow = idx;
		}
		ClearAllConfirms();
		Refresh();
	}

	protected void ClearAllConfirms()
	{
		m_ConfirmResetMembers = false;
		m_ConfirmEjectFloppy = false;
		m_ConfirmKick = false;
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelResetMembersConfirmation);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelEjectFloppyConfirmation);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelKickConfirmation);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_CloseButton)
		{
			Close();
			return true;
		}

		if (w == m_ClaimButton) { TriggerClaim(); return true; }
		if (w == m_ResetAdminButton) { TriggerResetAdmin(); return true; }
		if (w == m_ResetMembersButton) { TriggerPurge(); return true; }
		if (w == m_InviteButton) { TriggerInvite(); return true; }
		if (w == m_JoinLeaveButton) { TriggerJoinLeave(); return true; }
		if (w == m_EjectFloppyButton) { TriggerEjectFloppy(); return true; }
		if (w == m_KickButton) { TriggerKick(); return true; }

		if (w == m_OwnerButton) { SelectOwner(); return true; }

		for (int i = 0; i < MAX_VISIBLE_MEMBERS; i++)
		{
			if (w == m_MemberButtons[i]) { SelectMember(i); return true; }
		}

		for (int j = 0; j < PERM_ROW_COUNT; j++)
		{
			if (w == m_PermButtons[j])
			{
				TogglePermission(j);
				return true;
			}
		}

		return super.OnClick(w, x, y, button);
	}

	protected void TriggerClaim()
	{
		ClearAllConfirms();
		SendMenuRPC(MULTITERRITORYRPCs.MENU_CLAIM_ADMIN);
	}

	protected void TriggerResetAdmin()
	{
		ClearAllConfirms();
		SendMenuRPC(MULTITERRITORYRPCs.MENU_RESET_ADMIN);
	}

	protected void TriggerInvite()
	{
		ClearAllConfirms();
		SendMenuRPC(MULTITERRITORYRPCs.MENU_TOGGLE_INVITE);
	}

	protected void TriggerJoinLeave()
	{
		ClearAllConfirms();
		SendMenuRPC(MULTITERRITORYRPCs.MENU_JOIN_OR_LEAVE);
	}

	protected void TriggerPurge()
	{
		m_ConfirmEjectFloppy = false;
		m_ConfirmKick = false;
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelEjectFloppyConfirmation);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelKickConfirmation);

		if (!m_ConfirmResetMembers)
		{
			m_ConfirmResetMembers = true;
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelResetMembersConfirmation);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.CancelResetMembersConfirmation, CONFIRM_TIMEOUT_MS, false);
			Refresh();
			return;
		}

		m_ConfirmResetMembers = false;
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelResetMembersConfirmation);
		SendMenuRPC(MULTITERRITORYRPCs.MENU_RESET_MEMBERS);
	}

	protected void TriggerEjectFloppy()
	{
		m_ConfirmResetMembers = false;
		m_ConfirmKick = false;
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelResetMembersConfirmation);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelKickConfirmation);

		if (!m_ConfirmEjectFloppy)
		{
			m_ConfirmEjectFloppy = true;
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelEjectFloppyConfirmation);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.CancelEjectFloppyConfirmation, CONFIRM_TIMEOUT_MS, false);
			Refresh();
			return;
		}

		m_ConfirmEjectFloppy = false;
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelEjectFloppyConfirmation);
		REV_TerritoryComputer comp = GetTargetComputer();
		if (comp) comp.ClientPreviewPowerOff();
		SendMenuRPC(MULTITERRITORYRPCs.MENU_EJECT_FLOPPY);
	}

	protected void TriggerKick()
	{
		if (m_SelectedMemberGuid == "") return;
		REV_TerritoryComputer comp = GetTargetComputer();
		if (!comp) return;
		if (m_SelectedMemberGuid == comp.GetTerritoryOwnerId()) return;
		if (!comp.IsTerritoryMember(m_SelectedMemberGuid)) return;

		m_ConfirmResetMembers = false;
		m_ConfirmEjectFloppy = false;
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelResetMembersConfirmation);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelEjectFloppyConfirmation);

		if (!m_ConfirmKick)
		{
			m_ConfirmKick = true;
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelKickConfirmation);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.CancelKickConfirmation, CONFIRM_TIMEOUT_MS, false);
			Refresh();
			return;
		}

		string target = m_SelectedMemberGuid;
		m_ConfirmKick = false;
		m_SelectedMemberGuid = "";
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.CancelKickConfirmation);
		SendKickMemberRPC(target);
	}

	protected void TogglePermission(int idx)
	{
		if (idx < 0 || idx >= m_PermBitsTable.Count()) return;
		if (m_SelectedMemberGuid == "") return;

		REV_TerritoryComputer comp = GetTargetComputer();
		if (!comp) return;

		string ownerGuid = comp.GetTerritoryOwnerId();
		if (m_SelectedMemberGuid == ownerGuid) return;
		if (!comp.IsTerritoryMember(m_SelectedMemberGuid)) return;

		string selfGuid = GetPlayerGuid();
		bool isOwner = comp.IsTerritoryOwner(selfGuid);
		bool isServerAdmin = GetMultiTerritoriesConfig().ServerAdmins.Find(selfGuid) != -1;
		if (!isOwner && !isServerAdmin) return;

		int bit = m_PermBitsTable[idx];
		if (bit == TerritoryPerm.ADMIN) return;

		int currentPerms = comp.GetMemberPermissions(m_SelectedMemberGuid);
		bool currentlyGranted = (currentPerms & bit) != 0;
		bool grant = !currentlyGranted;

		SendEditPermissionRPC(m_SelectedMemberGuid, bit, grant);
	}

	override bool OnKeyDown(Widget w, int x, int y, int key)
	{
		if (key == KeyCode.KC_ESCAPE) { Close(); return true; }
		if (key == KeyCode.KC_F1) { if (m_ClaimButton && m_ClaimButton.IsVisible()) TriggerClaim(); return true; }
		if (key == KeyCode.KC_F2) { if (m_InviteButton && m_InviteButton.IsVisible()) TriggerInvite(); return true; }
		if (key == KeyCode.KC_F3) { if (m_ResetMembersButton && m_ResetMembersButton.IsVisible()) TriggerPurge(); return true; }
		if (key == KeyCode.KC_F4) { if (m_JoinLeaveButton && m_JoinLeaveButton.IsVisible()) TriggerJoinLeave(); return true; }
		if (key == KeyCode.KC_F5) { if (m_EjectFloppyButton && m_EjectFloppyButton.IsVisible()) TriggerEjectFloppy(); return true; }
		if (key == KeyCode.KC_F6) { if (m_ResetAdminButton && m_ResetAdminButton.IsVisible()) TriggerResetAdmin(); return true; }

		if (key == KeyCode.KC_PRIOR) { ScrollOrgTree(-MAX_VISIBLE_MEMBERS); return true; }
		if (key == KeyCode.KC_NEXT)  { ScrollOrgTree( MAX_VISIBLE_MEMBERS); return true; }

		return super.OnKeyDown(w, x, y, key);
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		ScrollOrgTree(-wheel);
		return true;
	}

	protected void ScrollOrgTree(int delta)
	{
		m_OrgScrollOffset = m_OrgScrollOffset + delta;
		m_SelectedEmptySlotRow = -1;
		Refresh();
	}


	protected void UpdateScrollThumb(int totalMembers)
	{
		if (!m_ScrollThumb || !m_ScrollTrack) return;

		float trackY = 0.225;
		float trackH = 0.265;

		bool needsScroll = totalMembers > MAX_VISIBLE_MEMBERS;
		if (!needsScroll)
		{
			m_ScrollThumb.Show(false);
			if (m_ScrollTrack) m_ScrollTrack.SetColor(ARGB(60, 90, 110, 40));
			return;
		}

		m_ScrollThumb.Show(true);
		if (m_ScrollTrack) m_ScrollTrack.SetColor(ARGB(140, 90, 110, 40));

		float visibleFrac = 1.0;
		if (totalMembers > 0) visibleFrac = MAX_VISIBLE_MEMBERS / totalMembers;
		if (visibleFrac < 0.18) visibleFrac = 0.18;
		float thumbH = trackH * visibleFrac;

		float maxOffset = totalMembers - MAX_VISIBLE_MEMBERS;
		float offsetFrac = 0;
		if (maxOffset > 0) offsetFrac = m_OrgScrollOffset / maxOffset;
		if (offsetFrac < 0) offsetFrac = 0;
		if (offsetFrac > 1) offsetFrac = 1;
		float thumbY = trackY + (trackH - thumbH) * offsetFrac;

		m_ScrollThumb.SetPos(0.183, thumbY);
		m_ScrollThumb.SetSize(0.0035, thumbH);
	}
}
