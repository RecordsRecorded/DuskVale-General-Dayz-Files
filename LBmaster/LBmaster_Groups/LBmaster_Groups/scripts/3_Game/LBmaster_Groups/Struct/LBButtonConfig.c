class LBButtonConfig {

	string buttonName; // Display Name of the Button (Text on the Button)
	string subtext; // Short descriptive Text under the button
	string hoverTooltip; // Tooltip displayed when hovering over the button
	string link; // URL to the Website, that should be opened if the button is clicked

	static LBButtonConfig InitButton(string name, string sub, string lnk, string tooltip) {
		LBButtonConfig cfg = new LBButtonConfig();
		cfg.hoverTooltip = tooltip;
		cfg.buttonName = name;
		cfg.subtext = sub;
		cfg.link = lnk;
		return cfg;
	}

	bool IsValid() {
		return buttonName != "" || subtext != "" || link != "";
	}

}