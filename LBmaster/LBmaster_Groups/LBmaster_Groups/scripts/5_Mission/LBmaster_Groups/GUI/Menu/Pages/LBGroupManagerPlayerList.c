typedef Param4<LBGroupMember, Widget, TextWidget, int> GroupManagerPlayerListEntry;

class LBGroupManagerPlayerList : ScriptedWidgetEventHandler {

	LBGroupManagePage parent;
	Widget mainWidget;
	ref array<ref GroupManagerPlayerListEntry> playerEntries = new array<ref GroupManagerPlayerListEntry>();
	int selected = -1;
	int currentSubgroupIndex = -1;
	int currentHeight = 2;

	void LBGroupManagerPlayerList(Widget mainWidget_, LBGroupManagePage parent_) {
		mainWidget = mainWidget_;
		parent = parent_;
		mainWidget.SetHandler(this);
	}

	int GetItemCount() {
		return playerEntries.Count();
	}

	int GetSelectedRow() {
		return selected;
	}

	void SetSelected(int index) {
		if (selected != -1 && selected < playerEntries.Count())
			playerEntries.Get(selected).param2.SetColor(ARGB(0, 0, 0, 0));
		selected = index;
		if (selected >= 0) {
			playerEntries.Get(selected).param2.SetColor(ARGB(120, 255, 0, 0));
			EnsureSelectedVisible();
		}
		parent.UpdateGroupButtons();
	}

	void ClearEntries() {
		LBWidgetUtils.DeleteAllChildren(mainWidget);
		playerEntries.Clear();
		currentHeight = 0;
		currentSubgroupIndex = -1;
	}

	void AddEntry(string groupName, int textColor = -1) {
		Widget entryWidget = LBLayoutManager.Get().CreateLayout("GroupOverviewPlayerEntry", mainWidget);
		entryWidget.SetPos(0, currentHeight);
		float width, height;
		entryWidget.GetScreenSize(width, height);
		currentHeight += height;
		TextWidget txt = TextWidget.Cast(entryWidget.FindAnyWidget("txt"));
		txt.SetText(groupName);
		txt.SetColor(textColor);
		currentSubgroupIndex++;
		playerEntries.Insert(new GroupManagerPlayerListEntry(null, entryWidget, txt, currentSubgroupIndex));
		if (playerEntries.Count() - 1 == selected)
			SetSelected(selected);
	}

	void SetEntry(string groupName, int index, int color) {
		if (index >= playerEntries.Count()) {
			AddEntry(groupName, color);
			return;
		}
		GroupManagerPlayerListEntry param = playerEntries.Get(index);
		bool wasSubgroup = false;
		if (param.param1 == null)
			wasSubgroup = true;
		param.param1 = null;
		param.param3.SetText(groupName);
		param.param3.SetColor(color);
		if (!wasSubgroup) {
			currentSubgroupIndex++;
			for (int i = index; i < playerEntries.Count(); i++) {
				playerEntries.Get(i).param4++;
			}
		}
	}

	void RemoveLastEntries(int start) {
		for (int i = start; i < playerEntries.Count();) {
			GroupManagerPlayerListEntry param = playerEntries.Get(i);
			playerEntries.Remove(i);
			if (!param.param1)
				currentSubgroupIndex--;
			float width, height;
			param.param2.GetSize(width, height);
			currentHeight -= height;
			param.param2.Unlink();
		}
		if (selected >= playerEntries.Count()) {
			SetSelected(playerEntries.Count() - 1);
		}
	}

	void SetEntry(LBGroupMember memberMarker, int index) {
		if (index >= playerEntries.Count()) {
			AddEntry(memberMarker);
			return;
		}

		GroupManagerPlayerListEntry param = playerEntries.Get(index);

		LBGroupPermission perm = LBGroupPermissions.Get.FindPermissionGroupByUID(memberMarker.permissionGroup);
		string groupname = "#lb_ag_unknown";
		if (!perm) {
			LBLogger.Fatal("Could not find Permission group " + memberMarker.permissionGroup + " of " + memberMarker.name, "AdvancedGroups");
		} else {
			groupname = perm.permName;
		}
		string prefix = "    ";
		if (!LBGroupSettingsConfig.Get.enableSubGroups)
			prefix = "";
		string displayname = prefix + memberMarker.name + " (" + groupname + ")";

		bool wasSubgroup = false;
		if (param.param1 == null)
			wasSubgroup = true;
		param.param1 = memberMarker;
		param.param3.SetText(displayname);
		param.param3.SetColor(memberMarker.GetColorARGB());
		if (wasSubgroup) {
			currentSubgroupIndex--;
			for (int i = index; i < playerEntries.Count(); i++) {
				playerEntries.Get(i).param4--;
			}
		}
	}

	void EnsureSelectedVisible() {
		int displayedHeight = 0;
		float width, height;
		mainWidget.GetScreenSize(width, height);
		displayedHeight = height / 10;
		float x, y;
		mainWidget.GetPos(x, y);

		int visibleFrom = - y;
		int visibleTo = displayedHeight - y;

		float selectedWidth, selectedHeight;
		playerEntries.Get(selected).param2.GetScreenSize(selectedWidth, selectedHeight);
		float selectedX, selectedY;
		playerEntries.Get(selected).param2.GetPos(selectedX, selectedY);

		int selectedFrom = selectedY;
		int selectedTo = selectedY + selectedHeight;

		if (selectedFrom + selectedHeight > visibleTo) {
			mainWidget.SetPos(0, - selectedTo + displayedHeight);
		} else if (selectedTo - selectedHeight < visibleFrom) {
			mainWidget.SetPos(0, - selectedFrom);
		}
	}

	void AddEntry(LBGroupMember memberMarker) {
		Widget entryWidget = LBLayoutManager.Get().CreateLayout("GroupOverviewPlayerEntry", mainWidget);
		entryWidget.SetPos(0, currentHeight);
		float width, height;
		entryWidget.GetScreenSize(width, height);
		currentHeight += height;
		TextWidget txt = TextWidget.Cast(entryWidget.FindAnyWidget("txt"));

		LBGroupPermission perm = LBGroupPermissions.Get.FindPermissionGroupByUID(memberMarker.permissionGroup);
		string groupname = perm.permName;
		string prefix = "    ";
		if (!LBGroupSettingsConfig.Get.enableSubGroups)
			prefix = "";
		string displayname = prefix + memberMarker.name + " (" + groupname + ")";

		txt.SetText(displayname);
		txt.SetColor(memberMarker.GetColorARGB());
		playerEntries.Insert(new GroupManagerPlayerListEntry(memberMarker, entryWidget, txt, currentSubgroupIndex));
		if (playerEntries.Count() - 1 == selected)
			SetSelected(selected);
	}

	int GetSubgroup(int row) {
		return playerEntries.Get(row).param4;
	}

	LBGroupMember GetItemData(int row) {
		return playerEntries.Get(row).param1;
	}

	override bool OnDrag(Widget w, int x, int y) {
		int index = 0;
		GroupManagerPlayerListEntry foundParam;
		if (GetParamWithWidget(w, foundParam) != -1) {
			if (!foundParam.param1 || !LBGroupSettingsConfig.Get.enableSubGroups) {
				CancelWidgetDragging();
			} else {
				foundParam.param2.SetColor(ARGB(144, 91, 91, 255));
			}
			return true;
		}
		return false;
	}

	int GetParamWithWidget(Widget w, out GroupManagerPlayerListEntry foundParam) {
		int index = 0;
		foreach (GroupManagerPlayerListEntry param : playerEntries) {
			if (w == param.param2 || w == param.param3) {
				foundParam = param;
				return index;
			}
			index++;
		}
		return -1;
	}

	int GetParamAt(int x, int y, out GroupManagerPlayerListEntry foundParam) {
		int index = 0;
		foreach (GroupManagerPlayerListEntry param : playerEntries) {
			float posX, posY, width, height;
			param.param2.GetScreenPos(posX, posY);
			param.param2.GetScreenSize(width, height);
			if (x >= posX && x < posX + width && y >= posY && y < posY + height) {
				foundParam = param;
				LBLogger.Verbose("Get Param at: " + x + " - " + y + " Found at " + index + ": " + param.param1 + " " + param.param2 + " " + param.param3 + " " + param.param4, "AdvancedGroups");
				return index;
			}
			index++;
		}
		LBLogger.Verbose("Get Param at: " + x + " - " + y + " did not find anything.", "AdvancedGroups");
		return -1;
	}

	override bool OnDrop(Widget w, int x, int y, Widget reciever) {
		Widget underMouse = GetWidgetUnderCursor();
		GroupManagerPlayerListEntry foundDragged;
		GroupManagerPlayerListEntry foundRecieved;
		if (GetParamWithWidget(w, foundDragged) != -1) {
			foundDragged.param2.SetColor(ARGB(0, 0, 0, 0));
			if (GetParamAt(x, y, foundRecieved) != -1) {
				if (foundRecieved.param4 != foundDragged.param4 && foundDragged.param1) {
					if (foundDragged.param1.steamid == LBAdmins.Get().GetMySteamid()) {
						parent.JoinSubGroup(foundRecieved.param4);
					} else {
						parent.MovePlayerToSubgroup(foundDragged.param1.uid, foundRecieved.param4);
					}
				}
			}
		}
		SetSelected(selected);
		return false;
	}

	Widget lastClicked;
	int lastClickedTime;
	int lastFinishedClickTime;

	override bool OnMouseButtonDown(Widget w, int x, int y, int button) {
		GroupManagerPlayerListEntry found;
		if (GetParamAt(x, y, found) != -1) {
			lastClicked = found.param2;
			lastClickedTime = g_Game.GetTime();
			return true;
		}
		return false;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button) {
		GroupManagerPlayerListEntry found;
		if (GetParamAt(x, y, found) != -1) {
			if (found.param2 == lastClicked && g_Game.GetTime() - lastClickedTime < 300) {
				lastClickedTime = 0;
				if (g_Game.GetTime() - lastFinishedClickTime < 300) {
					lastFinishedClickTime = 0;
					OnDoubleClick(lastClicked, x, y, button);
				} else {
					lastFinishedClickTime = g_Game.GetTime();
					OnClick(lastClicked, x, y, button);
				}
			}
			return true;
		}
		return false;
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		GroupManagerPlayerListEntry found;
		if (GetParamAt(x, y, found) != -1) {
			parent.JoinSubGroup(found.param4);
			return true;
		}
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		GroupManagerPlayerListEntry found;
		int index = GetParamWithWidget(w, found);
		if (index != -1)
			SetSelected(index);
		return false;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel) {
		int nextSelect = GetSelectedRow() - wheel;
		if (nextSelect >= 0 && nextSelect < GetItemCount())
			SetSelected(nextSelect);
		return true;
	}

	void OnSizeChange() {
		array<ref GroupManagerPlayerListEntry> playerEntriesCopy = playerEntries;
		ClearEntries();
	}

}