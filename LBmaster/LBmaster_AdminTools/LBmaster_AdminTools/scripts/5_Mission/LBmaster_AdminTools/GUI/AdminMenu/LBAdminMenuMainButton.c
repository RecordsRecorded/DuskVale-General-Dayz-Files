class LBAdminMenuMainButton : ScriptedWidgetEventHandler {

	TextWidget text;
	ButtonWidget btn;
	ImageWidget image;

	Widget mainWidget, background;
	LBAdminMenuMain parent;
	string primaryFrameClassname;
	ref map<string, ref LBAdminMenuFrame> frames = new map<string, ref LBAdminMenuFrame>();
	ref ScriptCaller clickCallback;

	static LBAdminMenuMainButton Init(LBAdminMenuMain parent_, string frameClassname_, int sort) {
		typename frameType = frameClassname_.ToType();
		if (!frameType) {
			LBLogger.Fatal("Could not find Classname " + frameClassname_ + " to create the Frame", "AdminTools");
			return null;
		}
		LBLogger.Info("Creating Admin menu frame " + frameClassname_, "AdminTools");
		LBAdminMenuFrame frame;
		Class.CastTo(frame, frameType.Spawn());
		if (!frame) {
			LBLogger.Fatal("Could not create frame " + frameClassname_, "AdminTools");
			return null;
		}
		LBAdminMenuMainButton button = new LBAdminMenuMainButton();
		if (frame.GetSharedButton() != "") {
			button = parent_.GetFrameButton(frame.GetSharedButton());
			button.frames.Insert(frameClassname_, frame);
			frame.Init(button);
			frame.OnShow();
			return null;
		} else {
			button.frames.Insert(frameClassname_, frame);
			button.primaryFrameClassname = frameClassname_;
			button.parent = parent_;
			Widget list_main = button.parent.layoutRoot.FindAnyWidget("list_main");
			button.mainWidget = LBLayoutManager.Get().CreateLayout("LBAdminMenuMain_Button", list_main);
			button.mainWidget.SetSort(sort);
			button.btn = ButtonWidget.Cast(button.mainWidget.FindAnyWidget("btn"));
			button.image = ImageWidget.Cast(button.mainWidget.FindAnyWidget("image"));
			button.text = TextWidget.Cast(button.mainWidget.FindAnyWidget("text"));
			button.background = button.mainWidget.FindAnyWidget("background");
			button.SetBackgroundColor();
			button.mainWidget.SetHandler(button);
			frame.Init(button);
			frame.OnShow();
			button.OverwriteInfo(frame.GetButtonText(), frame.GetButtonIcon());
			if (!frame.HasPermissionToOpen())
				button.mainWidget.Show(false);
		}
		return button;
	}

	LBAdminMenuFrame GetFrame(string classname) {
		return frames.Get(classname);
	}

	LBAdminMenuFrame GetPrimaryFrame() {
		return frames.Get(primaryFrameClassname);
	}

	void UpdateFrameColor() {
		foreach (string name, LBAdminMenuFrame frame : frames) {
			frame.UpdateFrameColor();
		}
	}

	void SetBackgroundColor() {
		background.SetColor(LBAdminMenuSettings.Get.GetFrameColor());
	}

	bool Update(float timeslice, bool otherMenu, bool leftClicked) {
		bool dragging = false;
		foreach (string name, LBAdminMenuFrame frame : frames) {
			frame.Update(timeslice, otherMenu, leftClicked);
			if (frame.dragging || frame.mouseIcon || frame.IsMouseOver()) {
				dragging = true;
			}
		}
		return dragging;
	}

	void CancelResize() {
		foreach (string name, LBAdminMenuFrame frame : frames) {
			if (frame && frame.leftDown && !frame.dragging) {
				frame.StopResize(true);
				frame.leftDown = false;
			}
		}
	}

	bool IsLeftDownAny() {
		foreach (string name, LBAdminMenuFrame frame : frames) {
			if (frame.leftDown)
				return true;
		}
		return false;
	}

	void OnInit() {
		foreach (string name, LBAdminMenuFrame frame : frames) {
			frame.OnInit();
		}
	}

	void UpdateSlow() {
		foreach (string name, LBAdminMenuFrame frame : frames) {
			frame.UpdateSlow();
		}
	}

	void OverwriteInfo(string name, string icon) {
		text.SetText(name);
		image.LoadImageFile(0, icon);
	}

	void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		foreach (string name, LBAdminMenuFrame frame : frames) {
			frame.OnRPC(target, rpc_type, ctx);
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn) {
			if (clickCallback)
				clickCallback.Invoke();
			if (GetPrimaryFrame().IsVisible()) {
				HideFrame();
			} else {
				ShowFrame();
			}
			return true;
		}
		return false;
	}

	void ShowFrame() { // Show the frame
		if (!GetPrimaryFrame().HasPermissionToOpen())
			return;
		image.SetColor(ARGB(255, 0, 255, 0));
		foreach (string name, LBAdminMenuFrame frame : frames) {
			frame.Show(true);
			if (name == primaryFrameClassname)
				frame.SetPrio();
		}
	}

	void HideFrame() { // Hide the frame (Parent menu still open)
		image.SetColor(ARGB(255, 255, 255, 255));
		foreach (string name, LBAdminMenuFrame frame : frames) {
			frame.Show(false);
			frame.ResetPrio();
		}
	}

	void ForceHide() {
		foreach (string name, LBAdminMenuFrame frame : frames) {
			if (frame)
				frame.pinned = false;
		}
		OnHide();
		image.SetColor(ARGB(255, 255, 255, 255));
	}

	void OnHide() { // Called when parent Menu was hidden
		foreach (string name, LBAdminMenuFrame frame : frames) {
			frame.OnHide();
		}
	}

	void OnShow() { // Called when parent Menu was shown (also called on first menu initialize)
		bool hasPerm = GetPrimaryFrame().HasPermissionToOpen();
		foreach (string name, LBAdminMenuFrame frame : frames) {
			if (!hasPerm) {
				mainWidget.Show(false);
				return;
			}
			mainWidget.Show(true);
			frame.LoadWindowProperties();
			frame.OnShow();
		}
	}

}