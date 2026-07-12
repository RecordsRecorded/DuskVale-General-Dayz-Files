class LBAdminMenuFrame_SelectedItems : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_esp_selected_items";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/selected_items.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 20;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 500;
		height = 400;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.cheat.esp", "admin.delete.items"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_esp_selected_items";
	}

	WrapSpacerWidget list_entries;
	ButtonWidget btn_delete;
	ScrollWidget scroll_items;

	ref array<ref Param4<Widget, ButtonWidget, ButtonWidget, EntityAI>> items = new array<ref Param4<Widget, ButtonWidget, ButtonWidget, EntityAI >> ();

	override void Show(bool show) {
		super.Show(show);
		LoadEntries();
	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (LBESPManager.updatedHighlightedItems) {
			LBESPManager.updatedHighlightedItems = false;
			LoadEntries();
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		bool found = false;
		foreach (Param4<Widget, ButtonWidget, ButtonWidget, EntityAI> param : items) {
			if (w == param.param2) {
				LBESPManager.RemoveHighlightedItemFromList(param.param4);
				found = true;
				break;
			} else if (w == param.param3) {
				g_Game.RPCSingleParam(null, LBAdminToolRPCs.DELETE_ITEM, new Param1<Object>(param.param4), true);
				LBESPManager.RemoveHighlightedItemFromList(param.param4);
				found = true;
				break;
			}
		}
		if (found) {
			LoadEntries();
			return true;
		}
		if (w == btn_delete) {
			if (LBESPManager.highlightedItems.Count() > 0) {
				LBWarningPopup.Get().Show("#lb_delete", LBTranslatedString("#lb_adm_confirm_delete_items", {"%count%", "" + LBESPManager.highlightedItems.Count()}).Get(), ScriptCaller.Create(ConfirmDelete), null, true);
			}
		}
		return false;
	}

	void ConfirmDelete() {
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(LBESPManager.highlightedItems);
		rpc.Send(null, LBAdminToolRPCs.DELETE_ITEMS_ENTITIES, true);
		LBESPManager.highlightedItems.Clear();
		LoadEntries();
	}

	void LoadEntries() {
		LBWidgetUtils.DeleteAllChildren(list_entries);
		items.Clear();
		if (LBESPManager.highlightedItems && LBESPManager.highlightedItems.Count() > 0) {
			foreach (EntityAI item : LBESPManager.highlightedItems) {
				AddItemEntry(item);
			}
		} else if (LBESPManager.highlightedEntity) {
			AddItemEntry(LBESPManager.highlightedEntity);
		}
		if (scroll_items.GetVScrollPos01() > 1.0) {
			scroll_items.VScrollToPos01(0);
			scroll_items.VScrollToPos01(1);
		}
	}

	void AddItemEntry(EntityAI item) {
		Widget root = LBLayoutManager.Get().CreateLayout("LBAdminItemListEntry", list_entries);
		ButtonWidget removeItem = ButtonWidget.Cast(root.FindAnyWidget("btn_remove"));
		ButtonWidget deleteItem = ButtonWidget.Cast(root.FindAnyWidget("btn_delete"));
		TextWidget name = TextWidget.Cast(root.FindAnyWidget("name"));
		name.SetText(item.GetDisplayName());
		items.Insert(new Param4<Widget, ButtonWidget, ButtonWidget, EntityAI>(root, removeItem, deleteItem, item));
		int index = LBESPManager.allItems.Find(item);
		if (index >= 0) {
			Widget itemParent = LBESPManager.itemWidgets.Get(index);
			TextWidget txt = TextWidget.Cast(itemParent.FindAnyWidget("txt"));
			name.SetColor(txt.GetColor() | 0xff000000);
		}
	}

}