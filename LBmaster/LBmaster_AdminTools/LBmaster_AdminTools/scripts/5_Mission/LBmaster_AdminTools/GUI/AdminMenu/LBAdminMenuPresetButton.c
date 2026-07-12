class LBAdminMenuPresetButton : ScriptedWidgetEventHandler {

	TextWidget number, number_small;
	Widget background;
	ButtonWidget btn_select;

	int index;
	bool visible = false;
	UAInput input;

	void LBAdminMenuPresetButton(Widget parent, int index_) {
		this.index = index_;
		background = LBLayoutManager.Get().CreateLayout("LBAdminMenuMain_Preset", parent);
		background.SetHandler(this);
		ConnectClassWidgetVariables(this, background, {"background"});
		LBAdminMenuWindowConfig.Get.Event_Frame_Name_Changed.Insert(SetText);
		SetText(index, LBAdminMenuWindowConfig.Get.GetPresetName(index));
		Collapse();
		input = GetUApi().GetInputByName("UALBMADMPreset" + (index + 1));
	}

	void SetText(int changedIndex, string name) {
		if (changedIndex != index)
			return;
		number.SetText(name);
		number_small.SetText(name);
		UpdateTextSize(number, 40, 10);
		UpdateTextSize(number_small, 12, 5);
	}

	void UpdateTextSize(TextWidget txt, int start, int min) {
		float w, h;
		int current = start;
		float pW, pH;
		txt.GetParent().GetScreenSize(pW, pH);
		while (current >= min) {
			txt.SetTextExactSize(current);
			txt.Update();
			txt.GetScreenSize(w, h);
			if (w < pW * 0.9)
				break;
			--current;
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (super.OnMouseEnter(w, x, y)) {
			return true;
		}
		Expand();
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_select) {
			SelectPreset();
			return true;
		}
		return false;
	}

	void Update() {
		if (visible && !LBWidgetUtils.IsUnderMouse(background))
			Collapse();
		if (LBAdminMenuWindowConfig.Get.selectedEntry == index) {
			background.SetColor(ARGB(100, 0, 255, 0));
		} else {
			background.SetColor(ARGB(100, 100, 100, 100));
		}
		if (input.LocalPress()) {
			SelectPreset();
		}
	}

	void SelectPreset() {
		LBAdminMenuMain menu = LBAdminMenuMain.Cast(LBMenuManager.Get().GetOpenLBMenu());
		if (menu)
			menu.ForceHide();
		LBAdminMenuWindowConfig.Get.SelectEntry(index);
		if (menu)
			menu.OnShow();
		LBAdminMenuWindowConfig.Loader.Save();
	}

	void Expand() {
		float mul = LBWidgetUtils.heightScale;
		background.SetPos(index * 52 * mul, 0);
		number_small.Show(false);
		number.Show(true);
		visible = true;
		LBTooltipManager.Get().DisplayTooltip(background, LBTranslatedString("#lb_adm_button_layout_preset", {"%keybind%", LBInputUtils.GetInputKeybindOut("UALBMADMPreset" + (index + 1))}).Get());
	}

	void Collapse() {
		float mul = LBWidgetUtils.heightScale;
		background.SetPos(index * 52 * mul, -35 * mul);
		number_small.Show(true);
		number.Show(false);
		visible = false;
	}

}