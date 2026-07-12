class LBClanTagColorDialog : ScriptedWidgetEventHandler {

	Widget rootWidget;
	TextWidget txt_preview;
	SliderWidget slider_r, slider_g, slider_b;
	ButtonWidget btn_save, btn_cancel;
	EditBoxWidget edit_r, edit_g, edit_b;
	CheckBoxWidget chckbx_use;

	void Init(LBGroupUI parentUI) {
		rootWidget = LBLayoutManager.Get().CreateLayout("ClanTagColorPicker", parentUI.layoutRoot);
		rootWidget.Show(false);
		rootWidget.SetHandler(this);

		ConnectClassWidgetVariables(this, rootWidget, {"rootWidget"});
	}

	void ShowPopup() {
		rootWidget.Show(true);

		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		LBGroup grp = player.GetLBGroup();
		chckbx_use.SetChecked(true);
		slider_r.SetCurrent(grp.chatColorR);
		slider_g.SetCurrent(grp.chatColorG);
		slider_b.SetCurrent(grp.chatColorB);
		edit_r.SetText("" + grp.chatColorR);
		edit_g.SetText("" + grp.chatColorG);
		edit_b.SetText("" + grp.chatColorB);
		txt_preview.SetText("[" + grp.shortname + "]");
		UpdatePreview();
	}

	void HidePopup() {
		rootWidget.Show(false);
	}

	void UpdatePreview() {
		txt_preview.SetColor(ARGB(255, slider_r.GetCurrent(), slider_g.GetCurrent(), slider_b.GetCurrent()));
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == slider_r) {
			return OnSliderUpdate(slider_r, edit_r);
		} else if (w == slider_g) {
			return OnSliderUpdate(slider_g, edit_g);
		} else if (w == slider_b) {
			return OnSliderUpdate(slider_b, edit_b);
		} else if (w == edit_r) {
			return OnTextUpdated(slider_r, edit_r);
		} else if (w == edit_g) {
			return OnTextUpdated(slider_g, edit_g);
		} else if (w == edit_b) {
			return OnTextUpdated(slider_b, edit_b);
		}
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_save) {
			SendChangeRequest();
			HidePopup();
			return true;
		} else if (w == btn_cancel) {
			HidePopup();
			return true;
		}
		return false;
	}

	void SendChangeRequest() {
		ScriptRPC rpc = new ScriptRPC();
		int r = slider_r.GetCurrent();
		int g = slider_g.GetCurrent();
		int b = slider_b.GetCurrent();
		bool has = chckbx_use.IsChecked();
		rpc.Write(has);
		rpc.Write(r);
		rpc.Write(g);
		rpc.Write(b);
		rpc.Send(null, LBGroupRPCs.CHANGE_TAG_COLOR, true);
	}

	bool OnSliderUpdate(SliderWidget slider, EditBoxWidget edit) {
		edit.SetText("" + slider.GetCurrent());
		UpdatePreview();
		return true;
	}

	bool OnTextUpdated(SliderWidget slider, EditBoxWidget edit) {
		int val = edit.GetText().ToInt();
		val = Math.Min(255, Math.Max(0, val));
		edit.SetText("" + val);
		slider.SetCurrent(val);
		UpdatePreview();
		return true;
	}


}