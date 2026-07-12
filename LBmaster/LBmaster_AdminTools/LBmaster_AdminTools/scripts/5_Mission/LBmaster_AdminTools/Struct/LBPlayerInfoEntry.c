class LBPlayerInfoEntry : ScriptedWidgetEventHandler {

	TextWidget slider_txt, name;
	SliderWidget slider;
	ButtonWidget btn_apply;

	Widget root;
	float serverValue, clientValue;
	int index;
	LBPlayerInfoEntries mgr;
	string display;

	void SetIndex(int index_) {
		this.index = index_;
	}

	void InitWidget(Widget parent, LBPlayerInfoEntries mgr_) {
		if (!parent || root)
			return;
		root = LBLayoutManager.Get().CreateLayout("AdminPlayerInfoItem", parent);
		root.SetHandler(this);
		this.mgr = mgr_;
		slider_txt = TextWidget.Cast(root.FindAnyWidget("slider_txt"));
		name = TextWidget.Cast(root.FindAnyWidget("name"));
		slider = SliderWidget.Cast(root.FindAnyWidget("slider"));
		btn_apply = ButtonWidget.Cast(root.FindAnyWidget("btn_apply"));
		name.SetText(display + " ");
		slider.SetMinMax(GetMin(), GetMax());
		slider.SetStep(GetAccuracy());
		root.Show(IsVisible());
	}

	void SetName(string name_) {
		this.display = name_;
	}

	bool IsVisible() {
		return true;
	}
	float GetValueFromPlayer(PlayerBase player);
	void SetPlayerValue(PlayerBase player, float value);
	string GetSliderText();
	float GetMax() {
		return 1000;
	}

	float GetAccuracy() {
		return 1;
	}

	float GetMin() {
		return 0;
	}

	float GetClientValue() {
		return slider.GetCurrent();
	}

	void SetFromServer(float value_) {
		this.serverValue = value_;
		if (!btn_apply.IsVisible())
			this.clientValue = value_;
		slider_txt.SetText(GetSliderText());
		root.Show(IsVisible());
	}

	void SetFromClient(float value_) {
		this.clientValue = value_;
		slider_txt.SetText(GetSliderText());
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (super.OnMouseEnter(w, x, y))
			return true;
		if (w == btn_apply) {
			LBTooltipManager.Get().DisplayTooltip(w, LBTranslatedString("#lb_adm_apply_player_stat"));
			return true;
		}
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_apply) {
			btn_apply.Show(false);
			if (button == MouseState.LEFT) {
				mgr.SetValue(clientValue, index);
			} else {
				SetFromClient(serverValue);
			}
			mgr.UpdateApplyAll();
			return true;
		}
		return false;
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == slider) {
			SetFromClient(GetClientValue());
			if (serverValue != clientValue)
				btn_apply.Show(true);
			else
				btn_apply.Show(false);
			mgr.UpdateApplyAll();
			return true;
		}
		return false;
	}

}