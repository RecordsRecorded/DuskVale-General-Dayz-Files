class LBAdminMenuFrame_ObjectSelection : LBAdminMenuFrame {

	override string GetSharedButton() {
		return "LBAdminMenuFrame_ObjectBuilder";
	}

	override string GetTitle() {
		return "#lb_adm_title_placed_objects";
	}

	override void GetMinSize(out int width, out int height) {
		width = 60;
		height = 0;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 600;
		height = 200;
	}

	override string GetInfoString() {
		return "#lb_adm_desc_placed_objects";
	}

	TextListboxWidget list_objects;

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (g_Game.LBIsKeyPressed(KeyCode.KC_DELETE)) {
			DeleteObject(list_objects.GetSelectedRow());
		} else if (GetUApi().GetInputByName("UALBMADMDuplicate").LocalPress()) {
			DuplicateObject();
		}
	}

	void DuplicateObject() {
		int row = list_objects.GetSelectedRow();
		LBAdminMenuFrame_ObjectBuilder builder = LBAdminMenuFrame_ObjectBuilder.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectBuilder"));
		if (builder && builder.visible && builder.currentBuildingSet && row >= 0 && row < builder.currentBuildingSet.objects.Count()) {
			builder.DuplicateBuilding(builder.currentBuildingSet.objects.Get(row));
		}
	}

	void DeleteObject(int row) {
		LBAdminMenuFrame_ObjectBuilder builder = LBAdminMenuFrame_ObjectBuilder.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectBuilder"));
		if (builder && builder.visible && builder.currentBuildingSet && row >= 0 && row < builder.currentBuildingSet.objects.Count()) {
			builder.RemoveItem(row);
			list_objects.RemoveRow(row);
			LBAdminMenuFrame_BuildingSets sets = LBAdminMenuFrame_BuildingSets.Cast(parent.GetFrame("LBAdminMenuFrame_BuildingSets"));
			sets.SetSavedChanges(false);
		}
	}

	void LoadBuildings(LBObjectBuilderSet buildings) {
		list_objects.ClearItems();
		if (buildings) {
			foreach (LBObjectBuilderObject obj : buildings.objects) {
				AddBuilderObject(obj);
			}
		}
	}

	void AddBuilderObject(LBObjectBuilderObject obj, bool scrollTo = false) {
		int row = list_objects.AddItem(obj.classname, obj, 0);
		UpdateObjectColor(obj, row);
		if (scrollTo) {
			list_objects.SelectRow(row);
			list_objects.EnsureVisible(row);
		}
	}

	void UpdateObjectColor(LBObjectBuilderObject obj, int row) {
		if (!LBInherit.Get().GetEntry(obj.classname))
			list_objects.SetItemColor(row, 0, ARGB(255, 255, 0, 0));
		else if (obj.IsEnabled())
			list_objects.SetItemColor(row, 0, ARGB(255, 0, 255, 0));
		else
			list_objects.SetItemColor(row, 0, ARGB(255, 255, 255, 0));
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button) {
		if (super.OnDoubleClick(w, x, y, button))
			return true;
		if (w == list_objects) {
			int row;
			if (!LBWidgetUtils.CheckSelectedInRange(list_objects, row))
				return true;
			LBAdminMenuFrame_ObjectBuilder builder = LBAdminMenuFrame_ObjectBuilder.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectBuilder"));
			if (builder.currentBuildingSet && row >= 0 && row < builder.currentBuildingSet.objects.Count()) {
				LBObjectBuilderObject obj = builder.currentBuildingSet.objects.Get(row);
				obj.SetEnabled(!obj.IsEnabled());
				UpdateObjectColor(obj, row);
				LBAdminMenuFrame_BuildingSets sets = LBAdminMenuFrame_BuildingSets.Cast(parent.GetFrame("LBAdminMenuFrame_BuildingSets"));
				sets.SetSavedChanges(false);
			}

		}
		return false;
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column,	int oldRow, int oldColumn) {
		if (super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn))
			return true;
		if (w == list_objects) {
			LBAdminMenuFrame_PlayerMap map_ = LBAdminMenuFrame_PlayerMap.Cast(parent.GetFrame("LBAdminMenuFrame_PlayerMap"));
			LBAdminMenuFrame_ObjectBuilder builder = LBAdminMenuFrame_ObjectBuilder.Cast(parent.GetFrame("LBAdminMenuFrame_ObjectBuilder"));
			if (builder && builder.visible && builder.currentBuildingSet && row >= 0 && row < builder.currentBuildingSet.objects.Count()) {
				if (map_ && map_.visible)
					map_.ZoomToPos(builder.currentBuildingSet.objects.Get(row).position);
				builder.ColorDragWidget(row);
			}
			return true;
		}
		return false;
	}

}