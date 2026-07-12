class LBCustomPageSettings : LBPageSettings {

	bool fullSize = true; // Set the panel to full size to fill the whole screen when opened. When disabled, it will only take up the left panel on the map. Useful if you don't want to show a lot of information on this page and don't want it to take up the whole screen
	ref TStringArray lines = new TStringArray(); // Lines for the page. Read the [How to format lines](/faq/?id=151) guide. Each new entry in this list is a new line. Line breaks are added automatically, but it's highly recommended to not rely on them, because DayZ has some inconsistencies causing lines to sometimes not show correctly.

}