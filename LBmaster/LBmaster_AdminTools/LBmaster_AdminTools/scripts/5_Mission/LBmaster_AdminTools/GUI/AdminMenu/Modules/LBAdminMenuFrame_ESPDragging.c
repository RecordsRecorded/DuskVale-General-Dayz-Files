class LBAdminMenuFrame_ESPDragging : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_esp_drag_items";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/espdragging.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 200;
		height = 60;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 400;
		height = 60;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.drag.espitem"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_esp_drag_items";
	}

	CheckBoxWidget chckbx_enable, chckbx_surface, chckbx_drag_camera;

	bool IsEnabled() {
		return IsVisible() && chckbx_enable.IsChecked();
	}

	override void Show(bool show) {
		super.Show(show);
		chckbx_enable.SetChecked(LBAdminMenuSettings.Get.espEnableDrag);
		chckbx_surface.SetChecked(LBAdminMenuSettings.Get.espToSurface);
		chckbx_drag_camera.SetChecked(LBAdminMenuSettings.Get.espDragCamera);
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == chckbx_enable) {
			LBAdminMenuSettings.Get.espEnableDrag = chckbx_enable.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == chckbx_surface) {
			LBAdminMenuSettings.Get.espToSurface = chckbx_surface.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == chckbx_drag_camera) {
			LBAdminMenuSettings.Get.espDragCamera = chckbx_drag_camera.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			return true;
		}
		return false;
	}

}