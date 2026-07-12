class LBCustomPanelPage : LBGroupPage {

	ScrollWidget scrollPanel;
	int currentHeight = 5;
	int defaultTextSize = 25;
	int defaultLineSpacing = 2;

	LBCustomPageSettings settings;

	void LBCustomPanelPage(LBPageSettingsType type_, LBCustomPageSettings settings_) {
		this.settings = settings_;
	}

	override bool InitPage(LBGroupUI parentUI) {
		return super.InitPage(parentUI, 6, 0, settings.buttonText, settings.fullSize, settings.priority);
	}

	override void InitMainWidget() {
		scrollPanel = ScrollWidget.Cast(rootWidget.FindAnyWidget("scrollPanel"));
		Widget content = scrollPanel.FindAnyWidget("content");
		LBWidgetUtils.CreateLines(scrollPanel, content, settings.lines);
	}

	override bool OnChange(Widget w) {
		LBWidgetUtils.UpdateClipChildren(scrollPanel);
		return super.OnChange(w);
	}

	override bool CanDisplayButton() {
		if (settings)
			return settings.enabled;
		return false;
	}

}