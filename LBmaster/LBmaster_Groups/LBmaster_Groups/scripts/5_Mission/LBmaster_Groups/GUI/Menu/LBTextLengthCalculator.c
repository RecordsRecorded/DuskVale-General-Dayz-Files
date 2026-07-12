class LBTextLengthCalculator {

	private TextWidget widget;

	static ref LBTextLengthCalculator g_LBTextLengthCalculator;

	static LBTextLengthCalculator Get() {
		if (!g_LBTextLengthCalculator)
			g_LBTextLengthCalculator = new LBTextLengthCalculator();
		return g_LBTextLengthCalculator;
	}

	static void Delete() {
		g_LBTextLengthCalculator = null;
	}

	void LBTextLengthCalculator() {
		widget = TextWidget.Cast(LBLayoutManager.Get().CreateLayout("TextLengthTester"));
	}

	void ~LBTextLengthCalculator() {
		if (widget) {
			widget.Unlink();
		}
	}

	float GetTextLength(string text, int size) {
		widget.SetTextExactSize(size);
		widget.SetText(text);
		widget.Update();
		float width, height;
		widget.GetScreenSize(width, height);
		width /= LBWidgetUtils.screenWidth;
		return width;
	}
}