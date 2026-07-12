class LBAdminMenuMain : LBMenuBase {

	WrapSpacerWidget list_main;
	Widget panel_layouts, panel_selection;
	ButtonWidget btn_collapse, btn_expand;
	ScrollWidget scrollPanel;

	ref array<ref LBAdminMenuMainButton> buttons = new array<ref LBAdminMenuMainButton>();
	ref array<ref LBAdminMenuPresetButton> presetButtons = new array<ref LBAdminMenuPresetButton>();
	ref TStringArray registeredFrames = new TStringArray();
	int panelWidth;
	bool wasLeftDown = false;
	bool wasInit = false;
	bool lookingAround = false, selectingESPEntries = false;
	int selectX, selectY;

	vector dragRotationAdd = Vector(0, 0, 0), dragPositionAdd = Vector(0, 0, 0);
	vector lastDragPos, lastDragNormal;
	EntityAI draggingItem = null;
	int dragESPEntry = -1;
	vector originalESPPosition = vector.Zero, originalESPOrientation = vector.Zero;
	vector dragM4[4];
	bool wasLeftControlDown = false;

	void ~LBAdminMenuMain() {
		LBLogger.Info("Admin Menu was Destroyed", "AdminTools");
	}

	void LBAdminMenuMain() {
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateSlow, 1000, true);
	}

	override bool SavePersistentState() {
		return true;
	}

	override string GetLayoutName() {
		return "LBAdminMenuMain";
	}

	void UpdateSlow() {
		foreach (LBAdminMenuMainButton button : buttons) {
			button.UpdateSlow();
		}
	}

	void SetExpandState(bool expanded) {
		btn_collapse.Show(expanded);
		btn_expand.Show(!expanded);
		float width, height;
		scrollPanel.GetScreenSize(width, height);
		if (expanded) {
			scrollPanel.SetPos(-9 * LBWidgetUtils.heightScale, 0);
		} else {
			scrollPanel.SetPos(60 * LBWidgetUtils.heightScale - width, 0);
		}
		LBAdminMenuWindowConfig.Get.expanded = expanded;
		LBAdminMenuWindowConfig.Loader.Save();
	}

	// To register your own frames, overwrite this method and call super.RegisterFrames(); !!
	// Frames can be reordered by the user anyways, so there is no need to care about call order here
	// Do not try to register Frames anywhere else!
	void RegisterFrames() {
		RegisterFrame("LBAdminMenuFrame_PlayerInfo");
		RegisterFrame("LBAdminMenuFrame_PlayerMap");
		RegisterFrame("LBAdminMenuFrame_TeleportPos");
		RegisterFrame("LBAdminMenuFrame_PlayerList");
		RegisterFrame("LBAdminMenuFrame_Teams");
		RegisterFrame("LBAdminMenuFrame_ItemSpawner");
		RegisterFrame("LBAdminMenuFrame_PresetSpawner");
		RegisterFrame("LBAdminMenuFrame_WeatherManager");
		RegisterFrame("LBAdminMenuFrame_PlayerCheats");
		RegisterFrame("LBAdminMenuFrame_Freecam");
		RegisterFrame("LBAdminMenuFrame_ActiveModifiers");
		RegisterFrame("LBAdminMenuFrame_ESPSettings");
		RegisterFrame("LBAdminMenuFrame_ESPDragging");
		RegisterFrame("LBAdminMenuFrame_SelectedItems");
		RegisterFrame("LBAdminMenuFrame_Watchlist");
		RegisterFrame("LBAdminMenuFrame_ConstructionEditor");
		RegisterFrame("LBAdminMenuFrame_ItemList");
		RegisterFrame("LBAdminMenuFrame_Banmanager");
		RegisterFrame("LBAdminMenuFrame_ObjectBuilder");
		RegisterFrame("LBAdminMenuFrame_ObjectSelection");
		RegisterFrame("LBAdminMenuFrame_BuildingSets");
		RegisterFrame("LBAdminMenuFrame_ObjectBuilderControls");
		RegisterFrame("LBAdminMenuFrame_ServerMonitor");
		RegisterFrame("LBAdminMenuFrame_LookingAt");

		RegisterFrame("LBAdminMenuFrame_Settings"); // Should always be the last
	}

	void SortRegisteredFrames() { // Simple sorting algorithm. No need to optimize this
		bool sorted = false;
		while (!sorted) {
			sorted = true;
			for (int i = 1; i < registeredFrames.Count(); ++i) {
				string prev = registeredFrames.Get(i - 1);
				string now = registeredFrames.Get(i);
				int prevIndex = LBAdminMenuWindowConfig.Get.GetFrameOrder(prev);
				int nowIndex = LBAdminMenuWindowConfig.Get.GetFrameOrder(now);
				if (prevIndex > nowIndex) {
					registeredFrames.Set(i - 1, now);
					registeredFrames.Set(i, prev);
					sorted = false;
				}
			}
		}
	}

	override bool OnDrop(Widget w, int x, int y, Widget reciever) {
		if (super.OnDrop(w, x, y, reciever))
			return true;
		LBAdminMenuMainButton from = GetByButton(w);
		LBAdminMenuMainButton to = GetByButton(reciever);
		if (from && to) {
			float rX, rY, rW, rH;
			reciever.GetScreenPos(rX, rY);
			reciever.GetScreenSize(rW, rH);
			registeredFrames.RemoveItem(from.primaryFrameClassname);
			int toOrder = registeredFrames.Find(to.primaryFrameClassname);
			if (y > rY + rH / 2) // When dropped in the lower half, add it behind the other button.
				++toOrder;
			registeredFrames.InsertAt(from.primaryFrameClassname, toOrder);
			if (toOrder <= 0) {
				LBAdminMenuWindowConfig.Get.MoveFrameOrder(from.primaryFrameClassname, "");
			} else {
				LBAdminMenuWindowConfig.Get.MoveFrameOrder(from.primaryFrameClassname, registeredFrames.Get(toOrder - 1));
			}
			LBAdminMenuWindowConfig.Loader.Save();
			ApplyButtonSort();
			return true;
		}
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_expand) {
			SetExpandState(true);
			return true;
		} else if (w == btn_collapse) {
			SetExpandState(false);
			return true;
		}
		return false;
	}

	LBAdminMenuMainButton GetByButton(Widget btn) {
		foreach (LBAdminMenuMainButton button : buttons) {
			if (btn == button.btn) {
				return button;
			}
		}
		return null;
	}

	override void OnInit() {
		wasInit = true;
		registeredFrames.Clear();
		RegisterFrames();
		SortRegisteredFrames();
		InitButtons();
		float width, height;
		list_main.GetScreenSize(width, height);
		panelWidth = width;
		foreach (LBAdminMenuMainButton button : buttons) {
			button.OnInit();
		}
		g_Game.GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(UpdateLB);
		for (int i = 8; i >= 0; --i) {
			presetButtons.Insert(new LBAdminMenuPresetButton(panel_layouts, i));
		}
		UpdateButtonsColor();
		SetExpandState(LBAdminMenuWindowConfig.Get.expanded);
	}

	void UpdateFrameColors() {
		foreach (LBAdminMenuMainButton button : buttons) {
			button.SetBackgroundColor();
			button.UpdateFrameColor();
		}
		UpdateButtonsColor();
	}

	void UpdateButtonsColor() {
		list_main.SetColor(LBAdminMenuSettings.Get.GetFrameColor());
	}

	override string GetTooltip(Widget w) {
		if (!LBAdminMenuWindowConfig.Get.expanded) {
			foreach (LBAdminMenuMainButton button : buttons) {
				if (w == button.btn) {
					return button.GetPrimaryFrame().GetButtonText();
				}
			}
		}
		return "";
	}

	override void OnHide() {
		super.OnHide();
		if (!buttons)
			return;
		foreach (LBAdminMenuMainButton button : buttons) {
			button.OnHide();
		}
		SetFocus(null);
		LBESPManager.HighlightWidgetsIn(0, 0, 0, 0);
	}

	override void OnShow() {
		super.OnShow();
		if (!wasInit) {
			foreach (LBAdminMenuMainButton button : buttons) {
				button.OnShow();
			}
		}
		wasInit = false;
	}

	void ForceHide() {
		foreach (LBAdminMenuMainButton button : buttons) {
			button.ForceHide();
		}
	}

	bool IsLeftDownAny() {
		foreach (LBAdminMenuMainButton button : buttons) {
			if (button.IsLeftDownAny())
				return true;
		}
		return false;
	}

	void CancelResize() {
		foreach (LBAdminMenuMainButton button : buttons) {
			button.CancelResize();
		}
	}

	bool GetCursorWorldPos(out vector hitPos, out vector hitNormal, Object ignore = null) {
		vector dir;
		if (!g_Game.GetUIManager().IsCursorVisible())
			dir = g_Game.GetCurrentCameraDirection();
		else
			dir = g_Game.GetWorldDirectionFromScreen(Vector(LBAdminMenuFrame.mouseX, LBAdminMenuFrame.mouseY, 0));
		vector camPos = g_Game.GetCurrentCameraPosition();
		vector endPos = camPos + dir * 1000;
		float frac;
		Object hitObj;
		PhxInteractionLayers layers = PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.VEHICLE_NOTERRAIN | PhxInteractionLayers.BUILDING | PhxInteractionLayers.CHARACTER | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.FIREGEOM | PhxInteractionLayers.DOOR | PhxInteractionLayers.WATERLAYER | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.FENCE | PhxInteractionLayers.AI;
		if (DayZPhysics.RayCastBullet(camPos, endPos, layers, ignore, hitObj, hitPos, hitNormal, frac)) {
			hitPos[1] = hitPos[1] + GetItemAboveGround(ignore);
			return true;
		}
		return false;
	}

	void RequestESPTeleport() {
		if (draggingItem && LBESPManager.enabled) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.ESP_MOVE_ITEM, new Param3<EntityAI, vector, vector>(draggingItem, draggingItem.GetPosition(), draggingItem.GetOrientation()), true);
		} else if (draggingItem) {
			draggingItem.SetPosition(originalESPPosition);
			draggingItem.SetOrientation(originalESPOrientation);
		}
	}

	int GetDraggedItemIndex() {
		if (!draggingItem)
			return -1;
		if (dragESPEntry < 0 || dragESPEntry >= LBESPManager.allItems.Count())
			return LBESPManager.allItems.Find(draggingItem);
		EntityAI item = LBESPManager.allItems.Get(dragESPEntry);
		if (item == draggingItem)
			return dragESPEntry;
		return LBESPManager.allItems.Find(draggingItem);
	}

	void CancelESPDragging() {
		if (!draggingItem)
			return;
		if (draggingItem) {
			draggingItem.SetPosition(originalESPPosition);
			draggingItem.SetOrientation(originalESPOrientation);
		}
		dragESPEntry = -1;
		draggingItem = null;
		dragRotationAdd = Vector(0, 0, 0);
	}

	void FinishDragging() {
		if (GetMouseState(MouseState.RIGHT) & MB_PRESSED_MASK)
			CancelESPDragging();
		else
			RequestESPTeleport();
		dragESPEntry = -1;
		draggingItem = null;
		dragRotationAdd = Vector(0, 0, 0);
	}

	void HandleStartDragging(int possibleESPItem) {
		dragESPEntry = possibleESPItem;
		if (dragESPEntry >= 0) {
			draggingItem = LBESPManager.allItems.Get(dragESPEntry);
			if (draggingItem && !draggingItem.GetParent()) {
				Math3D.MatrixIdentity4(dragM4);
				if (draggingItem) {
					originalESPPosition = draggingItem.GetPosition();
					originalESPOrientation = draggingItem.GetOrientation();
					vector camOrient = g_Game.GetCurrentCameraDirection().VectorToAngles();
					vector rotationFlags = GetItemRotationFlags(draggingItem);
					dragRotationAdd = Vector(-camOrient[0] - rotationFlags[0], 0, 0);
					dragPositionAdd = Vector(0, 0, 0);
					lastDragPos = vector.Zero;
					wasLeftControlDown = false;
				}
				if (LBAdminMenuSettings.Get.espDragCamera) {
					GetMousePos(selectX, selectY);
					Mission mission = g_Game.GetMission();
					mission.RemoveActiveInputExcludes({"menu", "movement", "aiming"}, true);
					lookingAround = true;
					SetFocus(null);
				}
			} else {
				dragESPEntry = -1;
				draggingItem = null;
				dragRotationAdd = Vector(0, 0, 0);
				dragPositionAdd = Vector(0, 0, 0);
			}
		}
	}

	void HandleLeftMouseButtonUp() {
		if (draggingItem) {
			FinishDragging();
		}
		if (lookingAround && IsVisible()) {
			Mission mission = g_Game.GetMission();
			mission.AddActiveInputExcludes({"movement", "aiming", "menu"});
			g_Game.GetUIManager().ShowUICursor(!DisableCursor());
		} else if (selectingESPEntries) {
			panel_selection.Show(false);
		}
		selectingESPEntries = false;
		lookingAround = false;
	}

	void HandleMouseDragging(int possibleESPItem) {
		if (!LBAdminMenuFrame.cursorWidgetCache || LBAdminMenuFrame.cursorWidgetCache.GetName() == "HudPanel" || WorkspaceWidget.Cast(LBAdminMenuFrame.cursorWidgetCache.GetParent())) {
			if (g_Game.LBIsKeyDown(KeyCode.KC_LSHIFT)) {
				panel_selection.Show(true);
				GetMousePos(selectX, selectY);
				selectingESPEntries = true;
				panel_selection.SetScreenPos(selectX, selectY);
			} else if (g_Game.IsAppActive()) {
				GetMousePos(selectX, selectY);
				Mission mission = g_Game.GetMission();
				mission.RemoveActiveInputExcludes({"menu", "movement", "aiming"}, true);
				lookingAround = true;
				SetFocus(null);
			}
		} else if (CanDragESP()) {
			HandleStartDragging(possibleESPItem);
		}
		if (GetMouseState(MouseState.RIGHT) & MB_PRESSED_MASK)
			CancelESPDragging();
	}

	void HandleLookingAround() {
		int movedX = LBAdminMenuFrame.mouseX - selectX;
		int movedY = LBAdminMenuFrame.mouseY - selectY;
		if (movedX * movedX + movedY * movedY > 100) {
			g_Game.GetUIManager().ShowUICursor(false);
		}
		if (GetUApi().GetInputByName("UALBMADMTeleportToCursor").LocalPress() || GetUApi().GetInputByName("UALBMADMTeleportToCursor").LocalHold()) {
			MissionGameplay mg = MissionGameplay.Cast(g_Game.GetMission());
			mg.TeleportToCursorLB();
		}
	}

	bool CanDragESP() {
		LBAdminMenuFrame_ESPDragging frame = LBAdminMenuFrame_ESPDragging.Cast(GetFrame("LBAdminMenuFrame_ESPDragging"));
		return frame && frame.IsEnabled() && g_Game.IsAppActive();
	}

	void UpdateLB(float timeslice) {
		if (!g_Game.IsAppActive() && GetDragWidget())
			CancelWidgetDragging();
		GetMousePos(LBAdminMenuFrame.mouseX, LBAdminMenuFrame.mouseY);
		bool otherMenu = false;
		if (g_Game.GetUIManager().GetMenu() && g_Game.GetUIManager().GetMenu() != this)
			otherMenu = true;

		bool leftDown = false;
		if (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK && !wasLeftDown) {
			leftDown = true;
		}
		LBAdminMenuFrame.cursorWidgetCache = GetWidgetUnderCursor();
		bool dragging = false;
		foreach (LBAdminMenuMainButton button : buttons) {
			dragging = button.Update(timeslice, otherMenu, leftDown) | dragging;
		}
		if (IsVisible()) {
			int possibleESPItem = LBESPManager.GetWidgetIndex(LBAdminMenuFrame.cursorWidgetCache);
			if (possibleESPItem >= 0 && !draggingItem && LBESPManager.highlightedItems.Count() == 0) {
				LBESPManager.highlightedEntity = LBESPManager.allItems.Get(possibleESPItem);
				LBESPManager.HighlightWidget(LBAdminMenuFrame.cursorWidgetCache);
			}
			if (leftDown && !dragging) {
				HandleMouseDragging(possibleESPItem);
			} else if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
				HandleLeftMouseButtonUp();
			} else if (GetMouseState(MouseState.RIGHT) & MB_PRESSED_MASK)
				CancelESPDragging();
			if (leftDown && !draggingItem && possibleESPItem >= 0) {
				PlayerBase targetPlayer = PlayerBase.Cast(LBESPManager.allItems.Get(possibleESPItem));
				if (targetPlayer && targetPlayer.GetIdentity()) {
					LBAdminMenuFrame_PlayerList playerList = LBAdminMenuFrame_PlayerList.Cast(GetFrame("LBAdminMenuFrame_PlayerList"));
					playerList.InvokePlayerSelected(targetPlayer.GetIdentity());
				}
			}
		}
		if (g_Game.LBIsKeyDown(KeyCode.KC_LCONTROL) && g_Game.LBIsKeyPressed(KeyCode.KC_C)) {
			if (LBESPManager.highlightedItems.Count() == 0 && LBESPManager.highlightedEntity && LBESPManager.lastHighlighted) {
				string itemname = LBESPManager.highlightedEntity.GetType() + " (" + LBESPManager.highlightedEntity.GetDisplayName() + ") -> " + LBESPManager.highlightedEntity.GetPosition() + " - " + LBESPManager.highlightedEntity.GetOrientation();
				g_Game.CopyToClipboard(itemname);
				LBCopiedPopup.ShowAtWidget(LBESPManager.lastHighlighted);
				GetUApi().SupressNextFrame(true);
			}
		}
		if (selectingESPEntries) {
			UpdatePanelSelection();
		} else if (lookingAround) {
			HandleLookingAround();
		}

		if (draggingItem) {
			HandleItemDragging();
		}
		LBESPManager.SetHighlighting(!selectingESPEntries && !lookingAround);
		LBAdminMenuFrame.cursorWidgetCache = null;

		if (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)
			wasLeftDown = true;
		else
			wasLeftDown = false;

		foreach (LBAdminMenuPresetButton button2 : presetButtons) {
			button2.Update();
		}
		if (g_Game.LBIsKeyPressed(KeyCode.KC_DELETE)) {
			if (LBESPManager.highlightedItems.Count() > 0 && LBAdmins.Get().HasPermission("admin.delete.items")) {
				LBWarningPopup.Get().Show("#lb_delete", "Are you sure you want to delete " + LBESPManager.highlightedItems.Count() + " items?", ScriptCaller.Create(ConfirmDelete), null, true);
			}
		}
	}

	void HandleItemDragging() {
		float dragRotationChange = GetUApi().GetInputByName("UAPrevAction").LocalValue() - GetUApi().GetInputByName("UANextAction").LocalValue();
		float step = 4;
		if (g_Game.LBIsKeyDown(KeyCode.KC_LSHIFT))
			step = 10;
		else if (g_Game.LBIsKeyDown(KeyCode.KC_LCONTROL))
			step = 0.5;
		dragRotationAdd[0] = dragRotationAdd[0] + dragRotationChange * step;
		int index = GetDraggedItemIndex();
		if (!LBESPManager.itemWidgets.Get(index).IsVisible())
			LBESPManager.itemWidgets.Get(index).Show(true);
		vector targetPos, targetNormal;
		if (draggingItem && LBESPManager.enabled) {
			bool gotWorldPos = GetCursorWorldPos(targetPos, targetNormal, draggingItem);
			vector cameraDir = g_Game.GetCurrentCameraDirection();
			if (g_Game.LBIsKeyDown(KeyCode.KC_LCONTROL) && lastDragPos != vector.Zero) {
				wasLeftControlDown = true;
				vector camPos = g_Game.GetCurrentCameraPosition();
				vector camAngles = cameraDir.VectorToAngles();
				// a = c * sin(alpha)
				float angle = camAngles[1];
				if (angle > 180)
					angle -= 360;
				float distance = vector.Distance(Vector(camPos[0], 0, camPos[2]), Vector(lastDragPos[0], 0, lastDragPos[2]));
				float a = distance * Math.Tan(angle * Math.DEG2RAD);
				float yDiff = a + camPos[1] - lastDragPos[1];
				dragPositionAdd[1] = yDiff;
			} else if (gotWorldPos) {
				lastDragPos = targetPos;
				lastDragNormal = targetNormal;
			}
			draggingItem.SetPosition(lastDragPos + dragPositionAdd);
			if (LBAdminMenuSettings.Get.espToSurface) {
				vector camDir = cameraDir.VectorToAngles();
				vector rotationFlags = GetItemRotationFlags(draggingItem);
				rotationFlags[0] = rotationFlags[0] + camDir[0];
				vector orient = LBAdminHologram.AlignToNormal(lastDragNormal, Vector(originalESPOrientation[0], 0, 0) + rotationFlags + dragRotationAdd);
				draggingItem.SetOrientation(orient);
			} else {
				camDir = cameraDir.VectorToAngles();
				draggingItem.SetOrientation(Vector(originalESPOrientation[0] + camDir[0], 0, 0) + dragRotationAdd);
			}
		}
		LBESPManager.HighlightWidget(LBESPManager.itemWidgets.Get(index));
	}

	static float GetItemAboveGround(Object item) {
		return item.GetBoundingCenter()[1] * item.GetScale();
	}

	vector GetItemRotationFlags(EntityAI item) {
		int flags = item.ConfigGetInt("rotationFlags");
		float yaw = 0.0, pitch = 0.0, roll = 0.0;
		if (flags & RF_FRONT)
			pitch -= 90;
		else if (flags & RF_BACK) {
			pitch -= 90;
		}
		if (flags & RF_TOP)
			yaw -= 90;
		if (flags & RF_BOTTOM)
			yaw -= 90;

		if (flags & RF_LEFT)
			roll -= 90;
		if (flags & RF_RIGHT)
			roll -= 90;
		return Vector(yaw, pitch, roll);
	}

	void ConfirmDelete() {
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(LBESPManager.highlightedItems);
		rpc.Send(null, LBAdminToolRPCs.DELETE_ITEMS_ENTITIES, true);
		LBESPManager.highlightedItems.Clear();
		LBESPManager.updatedHighlightedItems = true;
	}

	void UpdatePanelSelection() {
		int mouseX, mouseY;
		GetMousePos(mouseX, mouseY);
		int w = mouseX - selectX;
		int h = mouseY - selectY;
		int x = selectX;
		int y = selectY;
		if (w < 0) {
			x = mouseX;
			w = -w;
		}
		if (h < 0) {
			y = mouseY;
			h = -h;
		}
		panel_selection.SetScreenPos(x, y);
		panel_selection.SetSize(w, h);
		LBESPManager.HighlightWidgetsIn(x, y, w, h);
	}

	bool EnsureVisible(string classname) {
		foreach (LBAdminMenuMainButton button : buttons) {
			if (button.primaryFrameClassname == classname) {
				button.ShowFrame();
				return button.GetPrimaryFrame().IsVisible();
			}
		}
		return false;
	}

	LBAdminMenuFrame GetFrame(string classname) {
		foreach (LBAdminMenuMainButton button : buttons) {
			LBAdminMenuFrame frame = button.GetFrame(classname);
			if (frame)
				return frame;
		}
		return null;
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		foreach (LBAdminMenuMainButton button : buttons) {
			button.OnRPC(target, rpc_type, ctx);
		}
	}

	void RegisterFrame(string tn) {
		registeredFrames.Insert(tn);
	}

	void ApplyButtonSort() {
		foreach (LBAdminMenuMainButton button : buttons) {
			int index = registeredFrames.Find(button.primaryFrameClassname);
			if (button.mainWidget)
				button.mainWidget.SetSort(index);
		}
	}

	LBAdminMenuMainButton GetFrameButton(string frameClassname) {
		foreach (LBAdminMenuMainButton button : buttons) {
			if (button.GetFrame(frameClassname))
				return button;
		}
		return null;
	}

	void InitButtons() {
		int sort = 0;
		foreach (string entry : registeredFrames) {
			LBAdminMenuMainButton btn = LBAdminMenuMainButton.Init(this, entry, sort);
			if (btn) {
				buttons.Insert(btn);
				++sort;
			}
		}
	}

}