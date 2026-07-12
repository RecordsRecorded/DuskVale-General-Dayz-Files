class LBPageSettings {

	bool enabled = true; // Enable / Disable the Button in the Map Panel
	int priority = 275; // Priority for the button. Higher priorities will place it further to the left. Priorities of other buttons: (Info: 500, Group: 450, Markers: 400, Settings: 350, Achievements: 120, Item Restrictions: 100) Make sure to have a higher or lower priority setup here if you want to position it brefore or behind a button
	string buttonText = ""; // Name of the button on the Map Panel

	void LBPageSettings(int prio, string text) {
		this.priority = prio;
		this.buttonText = text;
	}

	string ToListString(int column) {
		return buttonText;
	}

}