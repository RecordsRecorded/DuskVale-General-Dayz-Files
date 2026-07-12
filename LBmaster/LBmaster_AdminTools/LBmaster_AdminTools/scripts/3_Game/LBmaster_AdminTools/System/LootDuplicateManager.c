class LootDuplicateManager {

	static EntityAI itemToCopy;

	static void OnUpdateStatic(float timeslice) {
		if (GetUApi().GetInputByName("UALBMADMDuplicate").LocalPress()) {
			if (RequestDuplicate())
				GetUApi().SupressNextFrame(true);
		} else if (GetUApi().GetInputByName("UALBMADMItemCopy").LocalPress()) {
			SetItemToCopy();
			if (itemToCopy)
				GetUApi().SupressNextFrame(true);
		} else if (GetUApi().GetInputByName("UALBMADMItemPaste").LocalPress()) {
			RequestPaste();
			GetUApi().SupressNextFrame(true);
		}
	}

	static void SetItemToCopy() {
		itemToCopy = FindItemUnderCursor();
	}

	static void RequestPaste() {
	}

	static bool RequestDuplicate() {
		EntityAI item = FindItemUnderCursor();
		if (!item)
			return false;
		SendCopyRequest(item, null, InventoryLocationType.GROUND, -1, -1, -1);
		return true;
	}

	static void SendCopyRequest(EntityAI item, EntityAI targetItem, InventoryLocationType loc, int targetRow, int targetCol, int targetSlot) {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.DUPLICATE_ITEM, new Param6<EntityAI, EntityAI, InventoryLocationType, int, int, int>(item, targetItem, loc, targetRow, targetCol, targetSlot), true);
	}

	private static EntityAI FindItemUnderCursor() {
		if (!g_Game.GetUIManager() || !g_Game.GetUIManager().GetMenu() || g_Game.GetUIManager().GetMenu().GetID() != MENU_INVENTORY)
			return null;
		Widget w = GetWidgetUnderCursor();
		string hierarchy = "";
		ItemPreviewWidget preview = null;
		while (w) {
			if (!preview) {
				Widget found = null;
				for (int i = -1; i <= 8; ++i) {
					if (i == -1)
						found = w.FindAnyWidget("Render");
					else
						found = w.FindAnyWidget("Render" + i);
					if (found) {
						preview = ItemPreviewWidget.Cast(found);
						break;
					}
				}
			}
			hierarchy = hierarchy + w.ClassName() + "(" + w.GetName() + "," + w.GetUserID() + ") ";
			w = w.GetParent();
		}
		if (!preview || !LBWidgetUtils.IsUnderMouse(preview))
			return null;
		return preview.GetItem();
	}

}