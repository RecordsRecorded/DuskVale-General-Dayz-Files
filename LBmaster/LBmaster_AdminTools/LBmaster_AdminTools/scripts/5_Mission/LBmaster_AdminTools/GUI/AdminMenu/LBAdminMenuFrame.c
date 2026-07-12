class LBAdminMenuFrame : ScriptedWidgetEventHandler {

	TextWidget header;
	Widget outline, background, top, resize_bottom, resize_right, resize_left, resize_bottom_right, resize_bottom_left, mainContent, resize_top_left, resize_top_right;
	Widget resize_top;
	ButtonWidget btn_close, btn_pin, btn_settings;
	ImageWidget img_pin, img_info, img_settings;

	Widget frame, layoutRoot;
	Widget mouseIcon;
	LBAdminMenuMain parent;
	LBAdminMenuMainButton buttonParent;
	int parentWidth;
	bool visible = false;

	ref array<ref LBItemPreviewHelper> helper = new array<ref LBItemPreviewHelper>();
	bool hasPrio = false, pinned = false;

	ref LBTooltipManager tooltip;
	static LBAdminMenuFrame lastFrame;

	void Init(LBAdminMenuMainButton parent_) {
		buttonParent = parent_;
		parent = parent_.parent;
		parentWidth = parent.panelWidth;
		frame = LBLayoutManager.Get().CreateLayout("LBAdminMenuMain_Frame");
		mainContent = frame.FindAnyWidget("mainContent");
		layoutRoot = LBLayoutManager.Get().CreateLayout(ClassName(), mainContent);
		ConnectClassWidgetVariables(this, frame, {"frame", "mouseIcon", "layoutRoot", "cursorWidgetCache"});
		frame.SetHandler(this);
		header.SetText(GetTitle());
		LoadWindowProperties();
		frame.Show(false);
		btn_settings.Show(HasSettings());
		img_info.Show(GetInfoString() != "");
		UpdateFrameColor();
		EditBoxWidget searchBar = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("edit_search"));
		if (searchBar) {
			LBClearInput.AddClearButton(searchBar, ScriptCaller.Create(ClearSearch));
		}
	}

	void ClearSearch(EditBoxWidget searchBar) {
		OnChange(searchBar, 0, 0, false);
	}

	void UpdateFrameColor() {
		background.SetColor(LBAdminMenuSettings.Get.GetFrameColor());
	}

	void SetItemHelper(Widget widget, bool horizontal, bool vertical) {
		LBItemPreviewHelper help = new LBItemPreviewHelper();
		help.SetAllowed(horizontal, vertical);
		help.OnWidgetScriptInit(widget);
		helper.Insert(help);
	}

	void LoadWindowProperties() {
		if (!LBAdminMenuWindowConfig.Get.RestoreWindowProperties(ClassName(), frame, pinned, visible)) {
			int initialW, initialH;
			GetInitialSize(initialW, initialH);
			initialW *= LBWidgetUtils.heightScale;
			initialH *= LBWidgetUtils.heightScale;
			frame.SetScreenSize(initialW, initialH);
			float centerX = (LBWidgetUtils.screenWidth - initialW) / 2;
			float centerY = (LBWidgetUtils.screenHeight - initialH) / 2;
			frame.SetScreenPos(centerX, centerY);
			LBAdminMenuWindowConfig.Get.SaveWindowProperties(ClassName(), frame, pinned);
		}
		UpdatePinnedColor();
	}

	void SetPinned(bool pin) {
		pinned = pin;
		UpdatePinnedColor();
	}

	void UpdatePinnedColor() {
		if (pinned)
			img_pin.SetColor(ARGB(255, 0, 255, 0));
		else
			img_pin.SetColor(ARGB(255, 255, 255, 255));
	}

	void ~LBAdminMenuFrame() {
		if (frame)
			frame.Unlink();
	}

	bool IsVisible() {
		return frame.IsVisible();
	}

	void Show(bool show) {
		frame.Show(show);
		if (show) {
			LoadWindowProperties();
		} else {
			SetPinned(false);
		}
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.ADMIN_MENU_ACTION, new Param2<string, bool>(ClassName(), show), true);
		visible = show;
		LBAdminMenuWindowConfig.Get.SaveWindowProperties(ClassName(), show, pinned);
	}

	void CloseMenu() {
		parent.CloseMe();
	}

	bool IsParentVisible() {
		return parent.IsVisible();
	}

	void OnResize();
	void GetSize(out int w, out int h) {
		float w_, h_;
		mainContent.GetScreenSize(w_, h_);
		w = w_;
		h = h_;
	}
	void SetSize(int w, int h) {
		frame.SetScreenSize(w + 2, h + 23);
		CheckWindowBounds();
	}
	bool HasPermissionToOpen() {
		TStringArray permissions = GetRequiredPermissionsOne();
		if (permissions && permissions.Count() > 0) {
			bool has = false;
			foreach (string perm : permissions) {
				if (LBAdmins.Get().HasPermission(perm)) {
					has = true;
					break;
				}
			}
			return has;
		}
		return true;
	}
	TStringArray GetRequiredPermissionsOne();
	string GetSharedButton();
	bool HasSettings();
	void ToggleSettings();
	string GetInfoString() {
		return "";
	}
	void UpdateSlow();
	void OnInit();
	void OnRPC(Object target, int rpc_type, ParamsReadContext ctx);
	string GetButtonIcon() {}
	string GetButtonText() {
		return GetTitle();
	}
	string GetTitle();
	void GetMinSize(out int width, out int height) {
		width = 50;
		height = 30;
	}
	void GetInitialSize(out int width, out int height) {
		width = 50;
		height = 30;
	}

	bool dragging = false, leftDown = false, resizeBottom, resizeLeft, resizeRight, resizeTop;
	int dragStartX, dragStartY;

	void OnShow() {
		if (LBAdminMenuWindowConfig.Get.GetEntry(ClassName()).visible)
			buttonParent.ShowFrame();
	}

	void OnHide() {
		if (mouseIcon)
			mouseIcon.Unlink();
		if (!pinned) {
			frame.Show(false);
		}
	}

	static Widget cursorWidgetCache;
	static int mouseX, mouseY;

	void Update(float timeslice, bool otherMenu, bool leftClicked) {
		if (otherMenu) {
			frame.Show(false);
			return;
		} else if (pinned) {
			frame.Show(true);
		}
		if (!parent.IsVisible() || !frame.IsVisible())
			return;
		if (!g_Game.IsAppActive()) {
			leftDown = false;
			if (resizeTop || resizeLeft || resizeRight || resizeBottom)
				StopResize(false);
			return;
		}
		SetMouseIcon();
		if (dragging) {
			StopResize(false);
			if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
				dragging = false;
				leftDown = false;
				LBAdminMenuWindowConfig.Get.SaveWindowProperties(ClassName(), frame, pinned);
			} else {
				frame.SetScreenPos(dragStartX + mouseX, dragStartY + mouseY);
			}
		} else if (mouseIcon && !parent.IsLeftDownAny() && leftClicked) {
			if (mouseX != 0 && mouseX != LBWidgetUtils.screenWidth - 1 && mouseY != 0 && mouseY != LBWidgetUtils.screenHeight) {
				StartResize();
				leftDown = true;
				SetPrio();
			}
		} else if (leftDown && !(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
			StopResize(true);
			leftDown = false;
		}

		if (resizeBottom) {
			int diffY = dragStartY - mouseY;
			if (diffY != 0) {
				dragStartY = mouseY;
				float sizeW, sizeH;
				frame.GetScreenSize(sizeW, sizeH);
				frame.SetScreenSize(sizeW, sizeH - diffY);
			}
		}
		if (resizeTop) {
			diffY = dragStartY - mouseY;
			if (diffY != 0) {
				dragStartY = mouseY;
				frame.GetScreenSize(sizeW, sizeH);
				frame.SetScreenSize(sizeW, sizeH + diffY);
				float posX, posY;
				frame.GetScreenPos(posX, posY);
				frame.SetScreenPos(posX, posY - diffY);
			}
		}
		if (resizeRight) {
			int diffX = dragStartX - mouseX;
			if (diffX != 0) {
				dragStartX = mouseX;
				frame.GetScreenSize(sizeW, sizeH);
				frame.SetScreenSize(sizeW - diffX, sizeH);
			}
		}
		if (resizeLeft) {
			diffX = dragStartX - mouseX;
			if (diffX != 0) {
				dragStartX = mouseX;
				frame.GetScreenSize(sizeW, sizeH);
				frame.SetScreenSize(sizeW + diffX, sizeH);
				frame.GetScreenPos(posX, posY);
				frame.SetScreenPos(posX - diffX, posY);
			}
		}
		SetMouseIcon();
		if (mouseIcon) {
			mouseIcon.SetScreenPos(mouseX, mouseY);
		}
		CheckWindowBounds();
		if (resizeLeft || resizeRight || resizeBottom || resizeTop) {
			OnResize();
		}
		if (tooltip && img_info != cursorWidgetCache) {
			HideInfoWidget();
		}
	}

	bool IsMouseOver() {
		if (!frame.IsVisible())
			return false;
		float w, h, x, y;
		frame.GetScreenSize(w, h);
		frame.GetScreenPos(x, y);
		float mX = LBAdminMenuFrame.mouseX;
		float mY = LBAdminMenuFrame.mouseY;
		if (x <= mX && y <= mY && mX <= x + w && mY <= y + h) {
			return true;
		}
		return false;
	}

	void CheckWindowBounds() {
		float posX, posY, width, height;
		frame.GetScreenPos(posX, posY);
		frame.GetScreenSize(width, height);

		bool update = false;
		int minW, minH;
		GetMinSize(minW, minH);
		minW = (minW + 2) * LBWidgetUtils.heightScale;
		minH = (minH + 23) * LBWidgetUtils.heightScale;
		if (width < minW) {
			width = minW;
			update = true;
		}
		if (height < minH) {
			height = minH;
			update = true;
		}
		if (posX < 0) {
			posX = 0;
			update = true;
		}
		if (posY < 0) {
			posY = 0;
			update = true;
		}
		if (posY + height > LBWidgetUtils.screenHeight) {
			if (height > LBWidgetUtils.screenHeight)
				height = LBWidgetUtils.screenHeight;
			posY = LBWidgetUtils.screenHeight - height;
			update = true;
		}
		if (posX + width > LBWidgetUtils.screenWidth - parentWidth) {
			if (width > LBWidgetUtils.screenWidth - parentWidth)
				width = LBWidgetUtils.screenWidth - parentWidth;
			posX = LBWidgetUtils.screenWidth - width - parentWidth;
			update = true;
		}
		if (update) {
			frame.SetScreenPos(posX, posY);
			frame.SetScreenSize(width, height);
		}
	}

	void StartResize() {
		if (LBWidgetUtils.IsUnderMouse(resize_bottom_left)) {
			resizeBottom = true;
			resizeLeft = true;
			resizeRight = false;
			resizeTop = false;
		} else if (LBWidgetUtils.IsUnderMouse(resize_bottom_right)) {
			resizeBottom = true;
			resizeLeft = false;
			resizeRight = true;
			resizeTop = false;
		} else if (LBWidgetUtils.IsUnderMouse(resize_top_left)) {
			resizeBottom = false;
			resizeLeft = true;
			resizeRight = false;
			resizeTop = true;
		} else if (LBWidgetUtils.IsUnderMouse(resize_top_right)) {
			resizeBottom = false;
			resizeLeft = false;
			resizeRight = true;
			resizeTop = true;
		} else if (LBWidgetUtils.IsUnderMouse(resize_left)) {
			resizeBottom = false;
			resizeLeft = true;
			resizeRight = false;
			resizeTop = false;
		} else if (LBWidgetUtils.IsUnderMouse(resize_right)) {
			resizeBottom = false;
			resizeLeft = false;
			resizeRight = true;
			resizeTop = false;
		} else if (LBWidgetUtils.IsUnderMouse(resize_top)) {
			resizeBottom = false;
			resizeLeft = false;
			resizeRight = false;
			resizeTop = true;
		} else if (LBWidgetUtils.IsUnderMouse(resize_bottom)) {
			resizeBottom = true;
			resizeLeft = false;
			resizeRight = false;
			resizeTop = false;
		}
		GetMousePos(dragStartX, dragStartY);
	}

	void SetMouseIcon() {
		if (mouseX == 0 || mouseX == LBWidgetUtils.screenWidth - 1 || mouseY == 0 || mouseY == LBWidgetUtils.screenHeight) {
			if (mouseIcon) {
				mouseIcon.Unlink();
			}
			return;
		}
		float rotation = 0.0;
		if (LBWidgetUtils.IsUnderMouse(btn_close) || LBWidgetUtils.IsUnderMouse(header) || cursorWidgetCache == null || (!LBWidgetUtils.IsChild(frame, cursorWidgetCache) && LBWidgetUtils.IsChild(parent.layoutRoot, cursorWidgetCache)) || !parent.IsVisible()) {
			if (mouseIcon) {
				mouseIcon.Unlink();
			}
			return;
		} else if (LBWidgetUtils.IsUnderMouse(resize_bottom_left)) {
			rotation = 90 + 45;
		} else if (LBWidgetUtils.IsUnderMouse(resize_bottom_right)) {
			rotation = 45;
		} else if (LBWidgetUtils.IsUnderMouse(resize_top_left)) {
			rotation = 180 + 45;
		} else if (LBWidgetUtils.IsUnderMouse(resize_top_right)) {
			rotation = 270 + 45;
		} else if (LBWidgetUtils.IsUnderMouse(resize_bottom)) {
			rotation = 90;
		} else if (LBWidgetUtils.IsUnderMouse(resize_left)) {
			rotation = 180;
		} else if (LBWidgetUtils.IsUnderMouse(resize_right)) {
			rotation = 0;
		} else if (LBWidgetUtils.IsUnderMouse(resize_top)) {
			rotation = 270;
		} else {
			if (mouseIcon) {
				mouseIcon.Unlink();
			}
			return;
		}
		if (!mouseIcon)
			mouseIcon = LBLayoutManager.Get().CreateLayout("LBResizeCursor");
		Widget img = mouseIcon.FindAnyWidget("img");
		img.SetRotation(0, 0, rotation);
		mouseIcon.SetSort(1025);
	}

	void StopResize(bool save) {
		resizeBottom = false;
		resizeLeft = false;
		resizeRight = false;
		resizeTop = false;
		if (save)
			LBAdminMenuWindowConfig.Get.SaveWindowProperties(ClassName(), frame, pinned);
	}

	override bool OnDrag(Widget w, int x, int y) {
		if (w == header) {
			CancelWidgetDragging();
			SetPrio();
			float screenX, screenY;
			header.GetScreenPos(screenX, screenY);
			dragStartX = screenX - x;
			dragStartY = screenY - y;
			dragging = true;
			leftDown = true;
			parent.CancelResize();
			return true;
		}
		return super.OnDrag(w, x, y);
	}

	string GetTooltip(Widget w);

	void ResetPrio() {
		frame.SetSort(0);
		hasPrio = false;
	}

	void ShowInfoWidget() {
		tooltip = new LBTooltipManager();
		tooltip.DisplayTooltip(img_info, GetInfoString());
	}

	void HideInfoWidget() {
		if (tooltip)
			tooltip = null;
	}

	bool SetPrio() {
		if (hasPrio)
			return false;
		if (lastFrame)
			lastFrame.ResetPrio();
		lastFrame = this;
		frame.SetSort(800);
		if (HasSpecialWidget(frame))
			frame.SetSort(0);
		else
			frame.SetSort(120);
		hasPrio = true;
		return true;
	}

	bool HasSpecialWidget(Widget parentWidget) {
		Widget next = parentWidget;
		while (next) {
			if (MapWidget.Cast(next) || ItemPreviewWidget.Cast(next)) {
				return true;
			}
			if (HasSpecialWidget(next.GetChildren()))
				return true;
			next = next.GetSibling();
		}
		return false;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button) {
		SetPrio();
		if (w && w.IsInherited(MapWidget)) {
			mapDownTime = g_Game.GetTime();
			mapDownX = x;
			mapDownY = y;
		}
		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (super.OnMouseEnter(w, x, y))
			return true;
		if (w == img_info) {
			ShowInfoWidget();
			return true;
		}
		string tt = GetTooltip(w);
		if (tt != "") {
			LBTooltipManager.Get().DisplayTooltip(w, tt);
			return true;
		}
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_pin) {
			SetPinned(!pinned);
			LBAdminMenuWindowConfig.Get.SaveWindowProperties(ClassName(), frame, pinned);
			return true;
		} else if (w == btn_close) {
			buttonParent.HideFrame();
		} else if (w == btn_settings) {
			ToggleSettings();
		}
		return false;
	}

	int mapDownTime, mapDownX, mapDownY;

	override bool OnMouseButtonUp(Widget w, int x, int y, int button) {
		super.OnMouseButtonUp(w, x, y, button);
		if (w.IsInherited(MapWidget)) {
			if (g_Game.GetTime() - mapDownTime < 300) {
				int dist = (x - mapDownX) * (x - mapDownX) + (y - mapDownY) * (y - mapDownY);
				LBLogger.Verbose("Distance between button down and up: " + dist, "AdminTools");
				if (dist < 25) {
					mapDownTime = 0;
					return OnClick(w, x, y, button);
				}
			}
		}
		return false;
	}

}