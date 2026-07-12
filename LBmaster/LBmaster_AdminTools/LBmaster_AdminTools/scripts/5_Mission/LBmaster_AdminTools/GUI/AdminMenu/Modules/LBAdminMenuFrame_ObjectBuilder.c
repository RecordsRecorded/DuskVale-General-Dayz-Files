class LBAdminMenuFrame_ObjectBuilder : LBAdminMenuFrame {

	ref LBObjectBuilderSet currentBuildingSet;

	Object spawnedObject;

	override string GetTitle() {
		return "#lb_adm_title_object_builder";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/crane.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 60;
		height = 0;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 600;
		height = 200;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.use.objectbuilder"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_object_builder";
	}

	ItemPreviewWidget item_preview;
	TextWidget txt_info_itemname;
	XComboBoxWidget combo_list;
	EditBoxWidget edit_search, edit_x, edit_y, edit_z, edit_yaw, edit_pitch, edit_roll, edit_scale;
	TextListboxWidget list_items;
	Widget itemPreviewFrame, previewBackground, panel_objectInfo;
	ButtonWidget btn_delete, btn_close_info;

	ref array<ref Param4<Object, int, int, vector>> hiddenObjects = new array<ref Param4<Object, int, int, vector >> ();
	ref array<ref Param2<LBObjectBuilderObject, Widget>> localObjectWidgets = new array<ref Param2<LBObjectBuilderObject, Widget >> ();

	ref array<ref Param4<LBObjectBuilderObject, vector, vector, float>> undoSteps = new array<ref Param4<LBObjectBuilderObject, vector, vector, float >> ();
	ref array<ref Param4<LBObjectBuilderObject, vector, vector, float>> redoSteps = new array<ref Param4<LBObjectBuilderObject, vector, vector, float >> ();

	Object draggingObject = null, selectedObject = null;
	int forceDragging = false;
	float draggingObjectHeight = 0;
	vector dragRotationAdd = Vector(0, 0, 0), dragOriginalOrientation;
	Object infoObject = null;
	Widget lastColoredDragWidget = null;
	int infoObjectIndex = -1;

	bool rightUp = true;

	void ~LBAdminMenuFrame_ObjectBuilder() {
		if (g_Game && localObjectWidgets) {
			foreach (Param2<LBObjectBuilderObject, Widget> widget : localObjectWidgets) {
				if (widget && widget.param2)
					widget.param2.Unlink();
			}
		}
	}

	override void Show(bool show) {
		super.Show(show);
		LoadAvailableItems();
		SetFocus(edit_search);
	}

	override void OnHide() {
		super.OnHide();
		DeleteSpawnedObject();
		FinishInfoObjectEdit();
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		UpdateDragWidgets();
		if (IsVisible()) {
			UpdateItemPreviewPosition();
			if (leftClicked && forceDragging > 0) {
				--forceDragging;
			}
			if (selectedObject) {
				if (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) {
					if (!g_Game.GetUIManager().IsCursorVisible()) {
						draggingObject = selectedObject;
						selectedObject = null;
					}
				} else {
					selectedObject = null;
				}
			}
			if (leftClicked && LBAdminMenuFrame.cursorWidgetCache) {
				int index = GetDragWidgetIndex(LBAdminMenuFrame.cursorWidgetCache);
				if (index != -1) {
					LBObjectBuilderObject builderObj = localObjectWidgets.Get(index).param1;
					selectedObject = builderObj.object;
					dragOriginalOrientation = selectedObject.GetOrientation();
					vector camOrient = g_Game.GetCurrentCameraDirection().VectorToAngles();
					if (LBAdminMenuSettings.Get.objB_camera_rotation)
						dragRotationAdd = Vector(-camOrient[0], 0, 0);
					else
						dragRotationAdd = Vector(0, 0, 0);
					LBAdminMenuFrame_ObjectSelection selection = LBAdminMenuFrame_ObjectSelection.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectSelection"));
					selection.list_objects.SelectRow(index);
					selection.list_objects.EnsureVisible(index);
				}
			} else if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) && draggingObject && !forceDragging) {
				FinishDragging();
			} else if ((GetMouseState(MouseState.RIGHT) & MB_PRESSED_MASK)) {
				if (draggingObject) {
					if (forceDragging) {
						selection = LBAdminMenuFrame_ObjectSelection.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectSelection"));
						selection.DeleteObject(currentBuildingSet.objects.Count() - 1);
					} else {
						LBObjectBuilderObject obj = GetBuilderObject(draggingObject);
						if (obj) {
							draggingObject.SetPosition(obj.position);
							draggingObject.SetOrientation(obj.orientation);
							draggingObject.SetScale(obj.scale);
						}
					}
					draggingObject = null;
					dragRotationAdd = Vector(0, 0, 0);
					forceDragging = 0;
				} else if (rightUp && LBAdminMenuFrame.cursorWidgetCache) {
					infoObjectIndex = GetDragWidgetIndex(LBAdminMenuFrame.cursorWidgetCache);
					rightUp = false;
					if (infoObjectIndex != -1) {
						builderObj = localObjectWidgets.Get(infoObjectIndex).param1;
						SetObjectInfo(builderObj.object);
					}
				}
			} else {
				rightUp = true;
			}
			if (leftClicked && !LBWidgetUtils.IsChild(panel_objectInfo, LBAdminMenuFrame.cursorWidgetCache)) {
				//FinishInfoObjectEdit();
			}

			if (draggingObject) {
				float dragRotationChange = GetUApi().GetInputByName("UAPrevAction").LocalValue() - GetUApi().GetInputByName("UANextAction").LocalValue();
				dragRotationAdd[0] = dragRotationAdd[0] + dragRotationChange * 5;
				vector pos, dir;
				if (parent.GetCursorWorldPos(pos, dir, draggingObject)) {
					float scale = draggingObject.GetScale();
					camOrient = g_Game.GetCurrentCameraDirection().VectorToAngles();
					draggingObject.SetPosition(pos);
					vector orient = dragRotationAdd;
					orient[0] = orient[0] + dragOriginalOrientation[0];
					if (LBAdminMenuSettings.Get.objB_camera_rotation)
						orient[0] = orient[0] + camOrient[0];
					if (LBAdminMenuSettings.Get.objB_snap_ground) {
						dir = g_Game.SurfaceGetNormal(pos[0], pos[2]);
						orient = LBAdminHologram.AlignToNormal(dir, orient);
					} else if (LBAdminMenuSettings.Get.objB_snap_object) {
						orient = LBAdminHologram.AlignToNormal(dir, orient);
					}
					draggingObject.SetOrientation(orient);
					draggingObject.SetScale(scale);
				}
			}
			if (GetUApi().GetInputByName("UALBMADMUndo").LocalPress())
				UndoStep();
			else if (GetUApi().GetInputByName("UALBMADMRedo").LocalPress())
				RedoStep();
		}
	}

	void RemoveItem(int row) {
		currentBuildingSet.objects.Get(row).Despawn();
		currentBuildingSet.objects.RemoveOrdered(row);
		Param2<LBObjectBuilderObject, Widget> param = localObjectWidgets.Get(row);
		if (param.param2)
			param.param2.Unlink();
		localObjectWidgets.RemoveOrdered(row);
	}

	void ColorDragWidget(int index) {
		if (lastColoredDragWidget) {
			lastColoredDragWidget.SetColor(ARGB(250, 200, 200, 200));
		}
		if (index >= 0 && index < localObjectWidgets.Count()) {
			Widget w = localObjectWidgets.Get(index).param2;
			lastColoredDragWidget = w;
			if (!w)
				return;
			w.SetColor(ARGB(250, 0, 255, 0));
		}
	}

	void SetObjectInfo(Object obj) {
		forceDragging = 0;
		FinishDragging();
		FinishInfoObjectEdit();
		if (!obj)
			return;
		infoObject = obj;
		txt_info_itemname.SetText(obj.GetType());
		panel_objectInfo.Show(true);
		panel_objectInfo.SetScreenPos(LBAdminMenuFrame.mouseX, LBAdminMenuFrame.mouseY);
		vector pos = infoObject.GetPosition();
		vector dir = infoObject.GetOrientation();
		edit_x.SetText("" + pos[0]);
		edit_y.SetText("" + pos[1]);
		edit_z.SetText("" + pos[2]);
		edit_yaw.SetText("" + dir[0]);
		edit_pitch.SetText("" + dir[1]);
		edit_roll.SetText("" + dir[2]);
		edit_scale.SetText("" + infoObject.GetScale());
	}

	void AddUndoStep(LBObjectBuilderObject obj, bool clearRedo) {
		undoSteps.Insert(new Param4<LBObjectBuilderObject, vector, vector, float>(obj, obj.position, obj.orientation, obj.scale));
		if (undoSteps.Count() > 100)
			undoSteps.RemoveOrdered(0);
		if (clearRedo)
			redoSteps.Clear();
	}

	void AddRedoStep(LBObjectBuilderObject obj) {
		redoSteps.Insert(new Param4<LBObjectBuilderObject, vector, vector, float>(obj, obj.position, obj.orientation, obj.scale));
		if (redoSteps.Count() > 100)
			redoSteps.RemoveOrdered(0);
	}

	void RedoStep() {
		if (redoSteps.Count() <= 0)
			return;
		Param4<LBObjectBuilderObject, vector, vector, float> step = redoSteps.Get(redoSteps.Count() - 1);
		redoSteps.RemoveOrdered(redoSteps.Count() - 1);
		if (!step.param1)
			return;
		AddUndoStep(step.param1, false);
		step.param1.position = step.param2;
		step.param1.orientation = step.param3;
		step.param1.scale = step.param4;
		if (step.param1.object) {
			step.param1.object.SetPosition(step.param1.position);
			step.param1.object.SetOrientation(step.param1.orientation);
			step.param1.object.SetScale(step.param1.scale);
		}
	}

	void UndoStep() {
		if (undoSteps.Count() <= 0)
			return;
		Param4<LBObjectBuilderObject, vector, vector, float> step = undoSteps.Get(undoSteps.Count() - 1);
		undoSteps.RemoveOrdered(undoSteps.Count() - 1);
		if (!step.param1)
			return;
		AddRedoStep(step.param1);
		step.param1.position = step.param2;
		step.param1.orientation = step.param3;
		step.param1.scale = step.param4;
		if (step.param1.object) {
			step.param1.object.SetPosition(step.param1.position);
			step.param1.object.SetOrientation(step.param1.orientation);
			step.param1.object.SetScale(step.param1.scale);
		}
	}

	void FinishInfoObjectEdit() {
		infoObjectIndex = -1;
		panel_objectInfo.Show(false);
		if (!infoObject)
			return;
		LBObjectBuilderObject obj = GetBuilderObject(infoObject);
		if (obj) {
			AddUndoStep(obj, true);
			obj.position = infoObject.GetPosition();
			obj.orientation = infoObject.GetOrientation();
			obj.scale = infoObject.GetScale();
			LBAdminMenuFrame_BuildingSets sets = LBAdminMenuFrame_BuildingSets.Cast(parent.GetFrame("LBAdminMenuFrame_BuildingSets"));
			sets.SetSavedChanges(false);
		}
		infoObject = null;
	}

	void FinishDragging() {
		if (draggingObject && currentBuildingSet) {
			LBObjectBuilderObject obj = GetBuilderObject(draggingObject);
			if (obj) {
				AddUndoStep(obj, true);
				obj.position = draggingObject.GetPosition();
				obj.orientation = draggingObject.GetOrientation();
				LBAdminMenuFrame_BuildingSets sets = LBAdminMenuFrame_BuildingSets.Cast(parent.GetFrame("LBAdminMenuFrame_BuildingSets"));
				sets.SetSavedChanges(false);
			}
			draggingObject = null;
			dragRotationAdd = Vector(0, 0, 0);
		}
	}

	int GetDragWidgetIndex(Widget widget) {
		for (int i = 0; i < localObjectWidgets.Count(); ++i) {
			if (localObjectWidgets.Get(i).param2 == widget)
				return i;
		}
		return -1;
	}

	void UpdateItemPreviewPosition() {
		if (!IsMouseOver() && LBAdminMenuFrame.cursorWidgetCache != itemPreviewFrame)
			itemPreviewFrame.Show(false);
		else if (itemPreviewFrame.IsVisible()) {
			float previewX, previewY, previewW, previewH;
			itemPreviewFrame.GetScreenSize(previewW, previewH);
			itemPreviewFrame.GetScreenPos(previewX, previewY);
			int posY = LBAdminMenuFrame.mouseY - previewH / 2;
			if (posY + previewH > LBWidgetUtils.screenHeight)
				posY = LBWidgetUtils.screenHeight - previewH;
			else if (posY < 0)
				posY = 0;
			itemPreviewFrame.SetScreenPos(previewX, posY);
		}
	}

	void CreateDragWidgets() {
		DestroyDragWidgets();
		if (!currentBuildingSet)
			return;
		foreach (LBObjectBuilderObject obj : currentBuildingSet.objects) {
			CreateDragWidget(obj);

		}
		UpdateDragWidgets();
	}

	void CreateDragWidget(LBObjectBuilderObject obj) {
		if (obj.object) {
			Widget main = LBLayoutManager.Get().CreateLayout("ObjectBuilderDragIcon");
			localObjectWidgets.Insert(new Param2<LBObjectBuilderObject, Widget>(obj, main));
		} else {
			localObjectWidgets.Insert(new Param2<LBObjectBuilderObject, Widget>(null, null));
		}
	}

	void UpdateDragWidgets() {
		foreach (Param2<LBObjectBuilderObject, Widget> param : localObjectWidgets) {
			if (param.param1 && param.param1.object) {
				vector worldPos = param.param1.object.GetPosition();
				vector screenPos = g_Game.GetScreenPos(worldPos);
				if (screenPos[2] < 0 || screenPos[0] == 0 || screenPos[1] == 0 || screenPos[0] == LBWidgetUtils.screenWidth - 1 || screenPos[1] == LBWidgetUtils.screenHeight - 1) {
					param.param2.Show(false);
					continue;
				}
				param.param2.SetScreenPos(screenPos[0], screenPos[1]);
				param.param2.Show(true);
			} else if (param.param2) {
				param.param2.Show(false);
			}
		}
	}

	void DestroyDragWidgets() {
		foreach (Param2<LBObjectBuilderObject, Widget> param : localObjectWidgets) {
			if (param.param2)
				param.param2.Unlink();
		}
		localObjectWidgets.Clear();
	}

	void ShowHiddenObjects() {
		foreach (Param4<Object, int, int, vector> object : hiddenObjects) {
			if (object.param1) {
				object.param1.SetPosition(object.param4);
				object.param1.SetFlags(EntityFlags.VISIBLE, true);
				object.param1.Update();
			}
		}
		hiddenObjects.Clear();
	}

	void UpdateHiddenObjects() {
		foreach (Param4<Object, int, int, vector> object : hiddenObjects)
			UpdateObject(object);
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBAdminToolRPCs.LOAD_BUILDING_SET) {
			bool shouldExport;
			ctx.Read(shouldExport);
			if (shouldExport) {
				LBAdminMenuFrame_BuildingSets buildingSets = LBAdminMenuFrame_BuildingSets.Cast(parent.GetFrame("LBAdminMenuFrame_BuildingSets"));
				if (!buildingSets)
					return;
				LBObjectBuilderSet tempSet;
				ctx.Read(tempSet);

				ObjectSpawnerJson json = new ObjectSpawnerJson();
				json.Objects = new array<ref ITEM_SpawnerObject>();
				int count = 0;
				foreach (LBObjectBuilderObject obj : tempSet.objects) {
					if (!obj.IsEnabled())
						continue;
					++count;
					ITEM_SpawnerObject item = new ITEM_SpawnerObject();
					item.name = obj.classname;
					item.pos = {obj.position[0], obj.position[1], obj.position[2]};
					item.ypr = {obj.orientation[0], obj.orientation[1], obj.orientation[2]};
					item.scale = obj.scale;
					item.enableCEPersistency = false;
					json.Objects.Insert(item);
				}
				JsonSerializer serial = new JsonSerializer();
				string output;
				serial.WriteToString(json, true, output);
				g_Game.CopyToClipboard(output);
				LBLoadingIcon.Hide();
				NotificationSystem.AddNotificationExtended(10.0, "Object Builder", "Successfully exported " + count + " of " + tempSet.objects.Count() + " Objects to Clipboard", LBIconConfig.Get.info);
				return;
			}

			if (currentBuildingSet)
				currentBuildingSet.Despawn();
			array<int> objectNetworkIds;
			ShowHiddenObjects();
			ctx.Read(currentBuildingSet);
			ctx.Read(objectNetworkIds);
			for (int i = 1; i < objectNetworkIds.Count(); i += 2) {
				Param4<Object, int, int, vector> param = new Param4<Object, int, int, vector>(null, objectNetworkIds.Get(i - 1), objectNetworkIds.Get(i), vector.Zero);
				hiddenObjects.Insert(param);
			}
			LBLoadingIcon.Hide();
			LoadCurrentBuildingSet();
		}
	}

	void UpdateObject(Param4<Object, int, int, vector> param) {
		if (param.param1)
			return;
		if (!param.param1)
			param.param1 = g_Game.GetObjectByNetworkId(param.param2, param.param3);
		if (param.param1 && param.param4 == vector.Zero) {
			param.param1.ClearFlags(EntityFlags.VISIBLE, true);
			param.param4 = param.param1.GetPosition();
			param.param1.SetPosition(vector.Zero);
		}
	}

	override void UpdateSlow() {
		super.UpdateSlow();
		UpdateHiddenObjects();
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column,	int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_items) {
			LoadSelectedObjectPreview();
			return true;
		}
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == combo_list) {
			LoadAvailableItems();
			return true;
		} else if (button == MouseState.MIDDLE && infoObject) {
			LBObjectBuilderObject obj = GetBuilderObject(infoObject);
			if (w == edit_x)
				edit_x.SetText("" + obj.position[0]);
			else if (w == edit_y)
				edit_y.SetText("" + obj.position[1]);
			else if (w == edit_z)
				edit_z.SetText("" + obj.position[2]);
			else if (w == edit_yaw)
				edit_yaw.SetText("" + obj.orientation[0]);
			else if (w == edit_pitch)
				edit_pitch.SetText("" + obj.orientation[1]);
			else if (w == edit_roll)
				edit_roll.SetText("" + obj.orientation[2]);

			vector pos = Vector(edit_x.GetText().ToFloat(), edit_y.GetText().ToFloat(), edit_z.GetText().ToFloat());
			infoObject.SetPosition(pos);
			vector dir = Vector(edit_yaw.GetText().ToFloat(), edit_pitch.GetText().ToFloat(), edit_roll.GetText().ToFloat());
			infoObject.SetOrientation(dir);
		} else if (w == btn_close_info) {
			FinishInfoObjectEdit();
			return true;
		} else if (w == btn_delete) {
			LBAdminMenuFrame_ObjectSelection selection = LBAdminMenuFrame_ObjectSelection.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectSelection"));
			selection.DeleteObject(infoObjectIndex);
			FinishInfoObjectEdit();
		}
		return false;
	}

	LBObjectBuilderObject GetBuilderObject(Object obj) {
		if (!currentBuildingSet)
			return null;
		foreach (LBObjectBuilderObject builder : currentBuildingSet.objects) {
			if (builder.object == obj) {
				return builder;
			}
		}
		return null;
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == list_items && currentBuildingSet) {
			int index;
			if (!LBWidgetUtils.CheckSelectedInRange(list_items, index)) {
				return true;
			}
			string itemname;
			list_items.GetItemText(index, 0, itemname);
			if (!LBItemHelper.CanSpawnLocalPreview(itemname))
				return true;
			LBObjectBuilderObject obj = new LBObjectBuilderObject(itemname, vector.Zero, vector.Zero, 1.0, true);
			obj.Spawn("");
			currentBuildingSet.objects.Insert(obj);
			CreateDragWidget(obj);
			LBAdminMenuFrame_ObjectSelection selection = LBAdminMenuFrame_ObjectSelection.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectSelection"));
			selection.AddBuilderObject(obj, true);
			draggingObject = obj.object;
			forceDragging = 2;
			return true;
		}
		return false;
	}

	void DuplicateBuilding(LBObjectBuilderObject obj) {
		LBObjectBuilderObject copy = new LBObjectBuilderObject(obj.classname, obj.position, obj.orientation, obj.scale, true);
		obj.Spawn("");
		currentBuildingSet.objects.Insert(obj);
		CreateDragWidget(obj);
		LBAdminMenuFrame_ObjectSelection selection = LBAdminMenuFrame_ObjectSelection.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectSelection"));
		selection.AddBuilderObject(obj, true);
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_search) {
			LoadAvailableItems();
			return true;
		}
		EditBoxWidget edit = EditBoxWidget.Cast(w);
		if (edit && (edit == edit_x || edit == edit_y || edit == edit_z)) {
			vector pos = Vector(edit_x.GetText().ToFloat(), edit_y.GetText().ToFloat(), edit_z.GetText().ToFloat());
			infoObject.SetPosition(pos);
		} else if (edit && (edit == edit_yaw || edit == edit_pitch || edit == edit_roll)) {
			vector dir = Vector(edit_yaw.GetText().ToFloat(), edit_pitch.GetText().ToFloat(), edit_roll.GetText().ToFloat());
			infoObject.SetOrientation(dir);
		} else if (w == edit_scale) {
			infoObject.SetScale(Math.Clamp(edit_scale.GetText().ToFloat(), 0.001, 10));
		}
		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (super.OnMouseEnter(w, x, y))
			return true;
		if (w == list_items) {
			LoadSelectedObjectPreview();
			return true;
		}
		return false;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel) {
		if (super.OnMouseWheel(w, x, y, wheel))
			return true;
		EditBoxWidget edit = EditBoxWidget.Cast(w);
		float step = wheel;
		if (g_Game.LBIsKeyDown(KeyCode.KC_LSHIFT))
			step *= 5;
		else if (g_Game.LBIsKeyDown(KeyCode.KC_LCONTROL))
			step *= 0.2;
		if (edit && (edit == edit_x || edit == edit_y || edit == edit_z)) {
			float value = edit.GetText().ToFloat();
			value += step * 0.1;
			edit.SetText("" + value);
			vector pos = Vector(edit_x.GetText().ToFloat(), edit_y.GetText().ToFloat(), edit_z.GetText().ToFloat());
			infoObject.SetPosition(pos);
			infoObject.SetScale(edit_scale.GetText().ToFloat());
		} else if (edit && (edit == edit_yaw || edit == edit_pitch || edit == edit_roll)) {
			value = edit.GetText().ToFloat();
			value += step;
			edit.SetText("" + value);
			vector dir = Vector(edit_yaw.GetText().ToFloat(), edit_pitch.GetText().ToFloat(), edit_roll.GetText().ToFloat());
			infoObject.SetOrientation(dir);
			infoObject.SetScale(edit_scale.GetText().ToFloat());
		} else if (w == edit_scale) {
			value = edit_scale.GetText().ToFloat();
			value += step * 0.1;
			value = Math.Clamp(value, 0.001, 10);
			edit_scale.SetText("" + value);
			infoObject.SetScale(value);
		}
		return false;
	}

	void LoadSelectedObjectPreview() {
		DeleteSpawnedObject();
		int index;
		if (!LBWidgetUtils.CheckSelectedInRange(list_items, index)) {
			itemPreviewFrame.Show(false);
			return;
		}
		string itemname;
		list_items.GetItemText(index, 0, itemname);
		LBLogger.Info("Trying to spawn item: " + itemname, "AdminTools");
		spawnedObject = LBWidgetUtils.SpawnAndSetItemPreview(item_preview, itemname);

		itemPreviewFrame.Show(true);
		float leftRight = ((float) LBAdminMenuFrame.mouseX) / LBWidgetUtils.screenWidth;
		float frameX, frameY;
		frame.SetScreenPos(frameX, frameY);
		float previewW, previewH;
		itemPreviewFrame.GetScreenSize(previewW, previewH);
		int posX;
		if (leftRight < 0.5) { // Left part of the screen
			float frameW, frameH;
			frame.GetScreenSize(frameW, frameH);
			posX = frameX + frameW;
		} else {
			posX = frameX - previewW;
		}
		int posY = LBAdminMenuFrame.mouseY - previewH / 2;
		if (posY + previewH > LBWidgetUtils.screenHeight)
			posY = LBWidgetUtils.screenHeight - previewH;
		else if (posY < 0)
			posY = 0;
		itemPreviewFrame.SetScreenPos(posX, posY);
	}

	void DeleteSpawnedObject() {
		if (spawnedObject && g_Game)
			g_Game.ObjectDelete(spawnedObject);
	}

	void LoadCurrentBuildingSet() {
		UpdateHiddenObjects();
		LBAdminMenuFrame_ObjectSelection selection = LBAdminMenuFrame_ObjectSelection.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectSelection"));
		selection.LoadBuildings(currentBuildingSet);
		currentBuildingSet.Spawn();
		CreateDragWidgets();
		LBAdminMenuFrame_BuildingSets sets = LBAdminMenuFrame_BuildingSets.Cast(parent.GetFrame("LBAdminMenuFrame_BuildingSets"));
		sets.SetSelected(currentBuildingSet.name);
	}

	void LoadAvailableItems() {
		string search = edit_search.GetText();
		search.ToLower();
		TStringArray available = GetAvailableItems();
		int i = 0, a = 0;
		foreach (string objectName : available) {
			if (search == "" || objectName.IndexOf(search) != -1) {
				if (list_items.GetNumItems() <= a)
					list_items.AddItem(objectName, null, 0);
				else
					list_items.SetItem(a, objectName, null, 0);
				++a;
			}
		}
		for (int j = list_items.GetNumItems() - 1; j >= 0; --j) {
			if (j >= a) {
				list_items.RemoveRow(j);
			}
		}
		if (list_items.GetSelectedRow() >= a) {
			list_items.SelectRow(a - 1);
			list_items.EnsureVisible(a - 1);
		}
		LoadSelectedObjectPreview();
	}

	TStringArray GetAvailableItems() {
		TStringArray available = new TStringArray();
		if (combo_list.GetCurrentItem() == 0) {
			TStringSet items = LBInherit.Get().GetAllChildren(null, true, true, true, 1);
			available = LBArrayTools<string>.ToArray(items);
		} else if (combo_list.GetCurrentItem() == 1) {
			items = LBInherit.Get().GetAllChildren(null, true, true, true, 2);
			available = LBArrayTools<string>.ToArray(items);
		} else if (combo_list.GetCurrentItem() == 2) {

		} else if (combo_list.GetCurrentItem() == 3) {
			TStringSet built = new TStringSet();
			if (currentBuildingSet) {
				foreach (LBObjectBuilderObject obj : currentBuildingSet.objects) {
					string name = LBStringTools.ToLowerString(obj.classname);
					built.Insert(name);
				}
			}
			available = LBArrayTools<string>.ToArray(built);
		}
		return available;
	}

}