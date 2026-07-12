[DayZGame.RegisterLBAdminPage("LBAdminForbiddenItemsMenu", "LBmaster_AdminTools/logo/logo.paa", "Forbidden Items", "admin.change.forbidden_items")]
class LBAdminForbiddenItemsMenu : LBAdmin_Menu_Page {

	TextListboxWidget list_forbidden;
	ButtonWidget btn_add, btn_rem;

	override void OnShow() {
		linked.LoadLinkedVars();
	}

	override void RegisterAllLinkedVars() {
		linked.RegisterLinkedList("GetConfig().forbiddenItems", list_forbidden, btn_add, btn_rem, null).EnableItemSelector(false);
	}

	LBForbidenItemsList_ GetConfig() {
		return LBForbidenItemsList.Get;
	}

	override array<string> GetEditedConfigs() {
		return {"LBForbidenItemsList"};
	}

}