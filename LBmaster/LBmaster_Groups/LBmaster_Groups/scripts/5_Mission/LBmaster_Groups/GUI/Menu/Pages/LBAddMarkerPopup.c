class LBAddMarkerPopup {

	TextWidget addTitle;
	ImageWidget image_Icon;
	SliderWidget sliderR, sliderG, sliderB, sliderA;
	SliderWidget radiusSliderR, radiusSliderG, radiusSliderB, radiusSliderA, sliderMarkerRadius;
	EditBoxWidget txtMarkerRadius;
	CheckBoxWidget chckbxRadiusStriked, chckbxShowPlayerNametags;
	ButtonWidget btn_add_marker, btn_add_cancel, btn_add_delete, btn_clear_name, btn_teleportto;
	EditBoxWidget input_marker_name;
	XComboBoxWidget comboBoxImage, comboBoxVisibility;
	Widget markerAdminPanel, radiusColorPreview;
	vector addPosition;

	ref array<ref LBMarkerType> availableTypes = new array<ref LBMarkerType>();

	bool edit_was_hidden = false;
	LBMarker edit_marker;
	ref LBMarker temp_marker;

	Widget addPopup;
	LBGroupUI parent;

	void Init(LBGroupUI parentUI) {
		parent = parentUI;
		addPopup = LBLayoutManager.Get().CreateLayout("Map Marker Add Popup", parent.layoutRoot);
		Hide();

		ConnectClassWidgetVariables(this, addPopup, {"addPopup"});
		btn_teleportto.Show(false);
		foreach (string str : LBMarkerSettingsConfig.Get.availableIcons) {
			string displayname = str;
			int index = displayname.LastIndexOf("\\");
			int index2 = displayname.LastIndexOf("/");
			if (index < index2)
				index = index2;
			if (index > 0) {
				displayname = displayname.Substring(index + 1, displayname.Length() - index - 1);
			}
			index = displayname.LastIndexOf(".");
			if (index != -1) {
				displayname = displayname.Substring(0, index);
			}
			if (displayname.Length() > 0) {
				string first = displayname[0] + "";
				first.ToUpper();
				displayname[0] = first;
			}
			comboBoxImage.AddItem(displayname);
		}
		comboBoxImage.SetCurrentItem(0);
		UpdateIconImage();
		FillAvailableMarkerTypes();
		UpdateMarkerRadiusText();
	}

	void ~LBAddMarkerPopup() {
		if (!g_Game)
			return;
		if (edit_marker) {
			edit_marker.hidden = edit_was_hidden;
			edit_marker.UpdateMarkerSlow();
			edit_marker = null;
		}
		if (parent && parent.mapMarkerManager) {
			parent.mapMarkerManager.UpdateSlowVisibility();
			if (temp_marker)
				parent.mapMarkerManager.RemoveMarker(temp_marker);
		}
		temp_marker = null;
	}

	void Hide() {
		if (addPopup)
			addPopup.Show(false);
		if (edit_marker) {
			edit_marker.hidden = edit_was_hidden;
			edit_marker.UpdateMarkerSlow();
			edit_marker = null;
		}
		parent.mapMarkerManager.UpdateSlowVisibility();
		if (temp_marker)
			parent.mapMarkerManager.RemoveMarker(temp_marker);
		temp_marker = null;
	}

	void OnHide() {
	}

	void OnUpdateFrame() {
		SetIconColor();
		SetRadiusColor();
	}

	void OnUpdateSlow() {

	}

	void UpdateShowRadiusCreation() {
		int visibility = comboBoxVisibility.GetCurrentItem();
		LBMarkerType type = availableTypes.Get(visibility);
		ShowRadiusCreation(type == LBMarkerType.SERVER_STATIC || type == LBMarkerType.SERVER_DYNAMIC);
	}

	void ShowRadiusCreation(bool show) {
		markerAdminPanel.Show(show);
		int height = LBWidgetUtils.HeightToPixel(270);
		if (show)
			height = LBWidgetUtils.HeightToPixel(365);
		float widthOld, heightOld;
		addPopup.GetSize(widthOld, heightOld);
		addPopup.SetSize(widthOld, height);
	}

	void UpdateMarkerRadiusText() {
		int radius = (int) sliderMarkerRadius.GetCurrent();
		txtMarkerRadius.SetText("" + radius);
	}

	void SetIconColor() {
		image_Icon.SetColor(ARGB(sliderA.GetCurrent(), sliderR.GetCurrent(), sliderG.GetCurrent(), sliderB.GetCurrent()));
	}

	void SetRadiusColor() {
		radiusColorPreview.SetColor(ARGB((int) radiusSliderA.GetCurrent(), (int) radiusSliderR.GetCurrent(), (int) radiusSliderG.GetCurrent(), (int) radiusSliderB.GetCurrent()));
	}

	void OnGroupChanged() {
		FillAvailableMarkerTypes();
	}

	void CreateTempMarker() {
		if (temp_marker)
			parent.mapMarkerManager.RemoveMarker(temp_marker);
		parent.mapMarkerManager.UpdateSlowVisibility();
		temp_marker = new LBMarker();
		vector pos = Vector(addPosition[0], addPosition[1], addPosition[2]);
		pos[1] = g_Game.SurfaceY(pos[0], pos[2]);
		if (pos[1] < 1)
			pos[1] = 1;
		temp_marker.SetupMarker(-1, "", "", pos);
		temp_marker.showGPS = false;
		temp_marker.showMap = true;
		temp_marker.show3D = false;
		UpdateTempMarker();
		parent.mapMarkerManager.AddMarker(temp_marker);
	}

	void UpdateTempMarker() {
		if (!temp_marker)
			return;

		string name = input_marker_name.GetText();
		int iconIndex = comboBoxImage.GetCurrentItem();
		string icon = LBMarkerSettingsConfig.Get.availableIcons.Get(iconIndex);
		temp_marker.SetIcon(icon);
		temp_marker.SetName(name);
		int colorA = sliderA.GetCurrent();
		int colorR = sliderR.GetCurrent();
		int colorG = sliderG.GetCurrent();
		int colorB = sliderB.GetCurrent();
		temp_marker.colorA = colorA;
		temp_marker.colorR = colorR;
		temp_marker.colorG = colorG;
		temp_marker.colorB = colorB;
		LBLogger.Debug("Add Marker Radius ? " + markerAdminPanel.IsVisible(), "AdvancedGroups");
		if (markerAdminPanel.IsVisible()) {
			temp_marker.SetRadius(sliderMarkerRadius.GetCurrent(), (int) radiusSliderA.GetCurrent(), (int) radiusSliderR.GetCurrent(), (int) radiusSliderG.GetCurrent(), (int) radiusSliderB.GetCurrent(), chckbxRadiusStriked.IsChecked(), false);
			temp_marker.showAllPlayerNametags = chckbxShowPlayerNametags.IsChecked();
		} else {
			temp_marker.showAllPlayerNametags = false;
			temp_marker.SetRadius(0.0, false);
		}
	}

	void AddMarkerButtonClicked() {
		int visibility = comboBoxVisibility.GetCurrentItem();
		LBMarkerType type = availableTypes.Get(visibility);
		string name = input_marker_name.GetText();
		int iconIndex = comboBoxImage.GetCurrentItem();
		string icon = LBMarkerSettingsConfig.Get.availableIcons.Get(iconIndex);
		int colorA = sliderA.GetCurrent();
		int colorR = sliderR.GetCurrent();
		int colorG = sliderG.GetCurrent();
		int colorB = sliderB.GetCurrent();
		if (temp_marker)
			parent.mapMarkerManager.RemoveMarker(temp_marker);
		parent.mapMarkerManager.UpdateSlowVisibility();
		temp_marker = null;
		if (edit_marker) {
			if (type != edit_marker.type) {
				vector pos = edit_marker.position;
				RequestMarkerDelete(edit_marker);
				AddMarker(pos, type, name, icon, colorR, colorG, colorB, colorA);
			} else {
				edit_marker.SetColorARGBGlobal(colorA, colorR, colorG, colorB);
				edit_marker.SetIconGlobal(icon);
				edit_marker.SetNameGlobal(name);
				if (markerAdminPanel.IsVisible()) {
					edit_marker.SetRadius(sliderMarkerRadius.GetCurrent(), (int) radiusSliderA.GetCurrent(), (int) radiusSliderR.GetCurrent(), (int) radiusSliderG.GetCurrent(), (int) radiusSliderB.GetCurrent(), chckbxRadiusStriked.IsChecked(), false, edit_marker.circleLayer);
					edit_marker.showAllPlayerNametags = chckbxShowPlayerNametags.IsChecked();
				} else {
					edit_marker.SetRadius(0.0, false);
					edit_marker.showAllPlayerNametags = false;
				}
				if (edit_marker.type == LBMarkerType.SERVER_STATIC || edit_marker.type == LBMarkerType.SERVER_DYNAMIC) {
					LBStaticMarkerManagerClient.Get().RequestGlobalMarkerUpdate(edit_marker.uid);
				} else if (type == LBMarkerType.PRIVATE_MARKER) {
					LBPrivateMarkerManager.Loader.Save();
				}
				Hide();
			}
			parent.UpdateMarkerListLater();
		} else {
			AddMarker(addPosition, type, name, icon, colorR, colorG, colorB, colorA);
		}
	}

	void AddMarker(vector pos, LBMarkerType type, string name, string icon, int colorR, int colorG, int colorB, int colorA) {

		string printname = name + "";
		printname.Replace("%", "");
#ifdef DIAG
		LBLogger.Info("AddMarker: " + pos + " " + type + " " + printname + " " + icon, "AdvancedGroups");
#endif
		LBMarker marker = new LBMarker();
		pos[1] = g_Game.SurfaceY(pos[0], pos[2]);
		if (pos[1] < 1)
			pos[1] = 1;
		marker.SetupMarker(type, name, icon, pos);
		marker.colorA = colorA;
		marker.colorR = colorR;
		marker.colorG = colorG;
		marker.colorB = colorB;
		LBLogger.Debug("Add Marker Radius ? " + markerAdminPanel.IsVisible(), "AdvancedGroups");
		if (markerAdminPanel.IsVisible()) {
			marker.SetRadius(sliderMarkerRadius.GetCurrent(), (int) radiusSliderA.GetCurrent(), (int) radiusSliderR.GetCurrent(), (int) radiusSliderG.GetCurrent(), (int) radiusSliderB.GetCurrent(), chckbxRadiusStriked.IsChecked(), false);
			marker.showAllPlayerNametags = chckbxShowPlayerNametags.IsChecked();
		} else {
			marker.showAllPlayerNametags = false;
			marker.SetRadius(0.0, false);
		}
		if (type == LBMarkerType.GROUP_MARKER) {
			PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
			if (pb) {
				LBGroup grp = pb.GetLBGroup();
				if (grp) {
					LBLogger.Debug("Adding Group Marker", "AdvancedGroups");
					grp.AddMarker(marker);
				}
			}
		} else if (type == LBMarkerType.PRIVATE_MARKER) {
			LBLogger.Debug("Adding Private Marker", "AdvancedGroups");
			LBPrivateMarkerManager.Get.AddMarker(marker);
		} else if (type == LBMarkerType.SERVER_STATIC || type == LBMarkerType.SERVER_DYNAMIC) {
			LBLogger.Debug("Adding Server Marker", "AdvancedGroups");
			LBStaticMarkerManagerClient.Get().RequestGlobalMarkerAdd(marker);
		}
		Hide();
	}

	void UpdateIconImage() {
		int selected = comboBoxImage.GetCurrentItem();
		if (selected < 0)
			return;
		string image = LBMarkerSettingsConfig.Get.availableIcons.Get(selected);
		image_Icon.LoadImageFile(0, image);
	}

	void ShowPopup(int x, int y, bool deleteMode, LBMarker marker) {
		FillAvailableMarkerTypes();
		vector pos = Vector(x, y, 0);
		addPosition = parent.mapWidget.ScreenToMap(pos);
		if (LBFOWConfig.Get.CannotInteractWith(addPosition, !LBFOWConfig.Get.canAddMarkers)) {
			return;
		}

		addPopup.Show(true);
#ifdef DIAG
		LBLogger.Debug("Showing Add Popup " + x + " " + y + " MapPos: " + addPosition + " Delete: " + deleteMode, "AdvancedGroups");
#endif
		btn_add_delete.Show(deleteMode);
		btn_teleportto.Show(LBAdmins.Get().HasPermissionActive("groups.teleport.map"));
		edit_marker = marker;
		if (deleteMode) {
			addPosition = edit_marker.position;
			edit_was_hidden = edit_marker.hidden;
			edit_marker.hidden = true;
			if (addTitle)
				addTitle.SetText("#lb_edit_marker");
			int index = LBMarkerSettingsConfig.Get.availableIcons.Find(marker.icon);
			if (index == -1)
				index = 0;
			comboBoxImage.SetCurrentItem(index);
			int typeIndex = availableTypes.Find(marker.type);
			if (typeIndex == -1)
				typeIndex = 0;
			comboBoxVisibility.SetCurrentItem(typeIndex);
			sliderA.SetCurrent(marker.colorA);
			sliderR.SetCurrent(marker.colorR);
			sliderG.SetCurrent(marker.colorG);
			sliderB.SetCurrent(marker.colorB);
			input_marker_name.SetText(marker.name);
			radiusSliderA.SetCurrent(marker.circleColorA);
			radiusSliderR.SetCurrent(marker.circleColorR);
			radiusSliderG.SetCurrent(marker.circleColorG);
			radiusSliderB.SetCurrent(marker.circleColorB);
			sliderMarkerRadius.SetCurrent(marker.circleRadius);
			chckbxRadiusStriked.SetChecked(marker.circleStriked);
			chckbxShowPlayerNametags.SetChecked(marker.showAllPlayerNametags);
			UpdateMarkerRadiusText();
			UpdateIconImage();
			SetIconColor();
		} else {
			if (addTitle)
				addTitle.SetText("#lb_addmarker");
		}
		UpdateShowRadiusCreation();
		CreateTempMarker();
	}

	void OnShow() {
		FillAvailableMarkerTypes();
		btn_teleportto.Show(LBAdmins.Get().HasPermissionActive("groups.teleport.map"));
	}

	bool OnClick(Widget w) {
		if (w == btn_add_marker) {
			AddMarkerButtonClicked();
			return true;
		} else if (w == btn_add_cancel) {
			Hide();
			return true;
		} else if (w == btn_add_delete) {
			RequestMarkerDelete(edit_marker);
			Hide();
			return true;
		} else if (w == btn_clear_name) {
			input_marker_name.SetText("");
			UpdateTempMarker();
			return true;
		} else if (w == btn_teleportto) {
			ScriptRPC rpc = new ScriptRPC();
			if (edit_marker) {
				rpc.Write(edit_marker.position);
			} else {
				rpc.Write(addPosition);
			}
			Hide();
			rpc.Send(null, LBGroupRPCs.ADMIN_TELEPORT_ON_MAP, true);
			//LBMenuManager.Get().CloseAllMenus();
			parent.CloseMe();
		}
		return false;
	}

	bool OnChange(Widget w) {
		if (w == comboBoxVisibility) {
			UpdateShowRadiusCreation();
			UpdateTempMarker();
			return true;
		} else if (w == comboBoxImage) {
			UpdateIconImage();
			UpdateTempMarker();
			return true;
		} else if (w == sliderMarkerRadius) {
			UpdateMarkerRadiusText();
			UpdateTempMarker();
			return true;
		} else if (w == txtMarkerRadius) {
			sliderMarkerRadius.SetCurrent(txtMarkerRadius.GetText().ToInt());
			UpdateMarkerRadiusText();
			UpdateTempMarker();
			return true;
		}
		UpdateTempMarker();
		return false;
	}

	void FillAvailableMarkerTypes() {
		comboBoxVisibility.ClearAll();
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		availableTypes.Clear();
		if (pb && pb.GetLBGroup()) {
			comboBoxVisibility.AddItem("#lb_group");
			availableTypes.Insert(LBMarkerType.GROUP_MARKER);
		}
		comboBoxVisibility.AddItem("#lb_marker_private");
		availableTypes.Insert(LBMarkerType.PRIVATE_MARKER);
		if (LBAdmins.Get().IsActive()) {
			if (LBAdmins.Get().HasPermission("groups.change.tempmarker")) {
				comboBoxVisibility.AddItem("#lb_marker_global_temp");
				availableTypes.Insert(LBMarkerType.SERVER_DYNAMIC);
			}
			if (LBAdmins.Get().HasPermission("groups.change.permmarker")) {
				comboBoxVisibility.AddItem("#lb_marker_global_perm");
				availableTypes.Insert(LBMarkerType.SERVER_STATIC);
			}
		}
		if (comboBoxVisibility.GetNumItems() <= comboBoxVisibility.GetCurrentItem())
			comboBoxVisibility.SetCurrentItem(0);
		UpdateShowRadiusCreation();
	}

	void DeleteMarkerUnderMouse() {
		int x, y;
		GetMousePos(x, y);
		LBLogger.Info("DeleteMarkerUnderMouse. Pos: " + x + "," + y, "AdvancedGroups");
		vector mousePos = Vector(x + 10, y + 10, 0);
		vector mapPos = parent.mapWidget.ScreenToMap(mousePos);
#ifdef DIAG
		LBLogger.Debug("MapPos: " + mapPos, "AdvancedGroups");
#endif
		LBMarker marker = FindMarkerInRadius(mapPos);
		RequestMarkerDelete(marker);
	}

	void RequestMarkerDelete(LBMarker marker) {
		if (marker) {
			string printname = marker.name + "";
			printname.Replace("%", "");
			LBLogger.Info("Removing Marker: " + printname + " " + marker.icon, "AdvancedGroups");
			if (marker.type == LBMarkerType.PRIVATE_MARKER) {
				LBPrivateMarkerManager.Get.RemoveMarker(marker);
			} else if (marker.type == LBMarkerType.GROUP_MARKER) {
				PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
				if (!pb || !pb.GetLBGroup())
					return;
				LBGroup grp = pb.GetLBGroup();
				grp.RemoveMarker(marker);
			} else if (marker.type == LBMarkerType.SERVER_STATIC || marker.type == LBMarkerType.SERVER_DYNAMIC) {
				LBStaticMarkerManagerClient.Get().RequestGlobalMarkerRemove(marker.uid);
			}
		}
	}

	LBMarker FindMarkerInRadius(vector center, float radius = 500) {
		PlayerBase pb = PlayerBase.Cast(g_Game.GetPlayer());
		if (!pb)
			return null;
		LBGroup grp = pb.GetLBGroup();
		LBMarker bestMarker = null;
		float bestDist = radius + 1;
		if (grp) {
			foreach (LBMarker marker : grp.markers) {
				if (!LBMarkerVisibilityManager.Get.IsMapVisible(marker))
					continue;
				float dist = Math.Sqrt((marker.position[0] - center[0]) * (marker.position[0] - center[0]) + (marker.position[2] - center[2]) * (marker.position[2] - center[2]));
				if (dist < bestDist) {
					LBLogger.Verbose("New Best Marker: " + marker + " OLD: " + bestMarker + " Dist: " + dist + " OLD: " + bestDist, "AdvancedGroups");
					bestMarker = marker;
					bestDist = dist;
				}
			}
		}
		foreach (LBMarker marker2 : LBPrivateMarkerManager.Get.privateMarkers) {
			if (!LBMarkerVisibilityManager.Get.IsMapVisible(marker2))
				continue;
			dist = Math.Sqrt((marker2.position[0] - center[0]) * (marker2.position[0] - center[0]) + (marker2.position[2] - center[2]) * (marker2.position[2] - center[2]));
			if (dist < bestDist) {
				LBLogger.Verbose("New Best Marker: " + marker2 + " OLD: " + bestMarker + " Dist: " + dist + " OLD: " + bestDist, "AdvancedGroups");
				bestMarker = marker2;
				bestDist = dist;
			}
		}
		if (LBAdmins.Get().IsActive()) {
			if (availableTypes.Find(LBMarkerType.SERVER_STATIC) != -1) {
				foreach (LBServerMarker serverMark2 : LBStaticMarkerManagerClient.Get().staticMarkers) {
					if (!LBMarkerVisibilityManager.Get.IsMapVisible(serverMark2))
						continue;
					if (serverMark2.type != LBMarkerType.SERVER_STATIC)
						continue;
					dist = Math.Sqrt((serverMark2.position[0] - center[0]) * (serverMark2.position[0] - center[0]) + (serverMark2.position[2] - center[2]) * (serverMark2.position[2] - center[2]));
					if (dist < bestDist) {
						LBLogger.Verbose("New Best Marker: " + serverMark2 + " OLD: " + bestMarker + " Dist: " + dist + " OLD: " + bestDist, "AdvancedGroups");
						bestMarker = serverMark2;
						bestDist = dist;
					}
				}
			}
			if (availableTypes.Find(LBMarkerType.SERVER_DYNAMIC) != -1) {
				foreach (LBServerMarker serverMark1 : LBStaticMarkerManagerClient.Get().staticMarkers) {
					if (!LBMarkerVisibilityManager.Get.IsMapVisible(serverMark1))
						continue;
					if (serverMark1.type != LBMarkerType.SERVER_DYNAMIC)
						continue;
					dist = Math.Sqrt((serverMark1.position[0] - center[0]) * (serverMark1.position[0] - center[0]) + (serverMark1.position[2] - center[2]) * (serverMark1.position[2] - center[2]));
					if (dist < bestDist) {
						LBLogger.Verbose("New Best Marker: " + serverMark1 + " OLD: " + bestMarker + " Dist: " + dist + " OLD: " + bestDist, "AdvancedGroups");
						bestMarker = serverMark1;
						bestDist = dist;
					}
				}
			}
		}
		LBLogger.Verbose("Best Marker: " + bestMarker + " Dist: " + bestDist, "AdvancedGroups");
		return bestMarker;
	}

}