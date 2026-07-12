class MapLegendItem {

	string iconPath; // Path to the icon, which is displayed for this entry (Check the `availableIcons` list for icons you can use)
	int iconColorA = 255; // Alpha Color of the Icon
	int iconColorR = 255; // Red Color of the Icon
	int iconColorG = 255; // Green Color of the Icon
	int iconColorB = 255; // Blue Color of the Icon
	string name; // Name displayed on the Map Legend
	int nameColorA = 255; // Alpha Color of the Text
	int nameColorR = 255; // Red Color of the Text
	int nameColorG = 255; // Green Color of the Text
	int nameColorB = 255; // Blue Color of the Text

	[NonSerialized()]
	Widget mainWidget;
	[NonSerialized()]
	ImageWidget iconWidget;
	[NonSerialized()]
	TextWidget nameWidget;

	void MapLegendItem(string iconPath_, string name_) {
		this.iconPath = iconPath_;
		this.name = name_;
	}

	void ~MapLegendItem() {
		DeleteWidget();
	}

	void DeleteWidget() {
		if (mainWidget)
			mainWidget.Unlink();
	}

	void CreateLegendWidget(Widget parent, int i) {
		float w, h;
		DeleteWidget();
		mainWidget = LBLayoutManager.Get().CreateLayout("MapLegendEntry", parent);
		ConnectClassWidgetVariables(this, mainWidget, {"mainWidget"}, {"nameWidget", "name", "iconWidget", "icon"});
		mainWidget.GetScreenSize(w, h);
		int posY = i * (4 + h) + 5;
		mainWidget.SetPos(5, posY);
		parent.SetSize(1, posY + h + 5);

		InitWidgets();
	}

	void InitWidgets() {
		iconWidget.LoadImageFile(0, iconPath);
		iconWidget.SetColor(ARGB(iconColorA, iconColorR, iconColorG, iconColorB));
		nameWidget.SetText(" " + name);
		nameWidget.SetColor(ARGB(nameColorA, nameColorR, nameColorG, nameColorB));
	}

	string ToListString(int column) {
		return name;
	}

}