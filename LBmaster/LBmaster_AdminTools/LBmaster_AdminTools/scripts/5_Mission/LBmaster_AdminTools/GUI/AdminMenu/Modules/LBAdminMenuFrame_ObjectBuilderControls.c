class LBAdminMenuFrame_ObjectBuilderControls : LBAdminMenuFrame {

	override string GetSharedButton() {
		return "LBAdminMenuFrame_ObjectBuilder";
	}

	override string GetTitle() {
		return "#lb_adm_title_object_builder_controls";
	}

	override void GetMinSize(out int width, out int height) {
		width = 200;
		height = 60;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 600;
		height = 60;
	}

	override string GetInfoString() {
		return "#lb_adm_desc_object_builder_controls";
	}

	ButtonWidget btn_snap_ground, btn_snap_object, btn_camera_rotation;

	override void Show(bool show) {
		super.Show(show);
		LoadSetting(btn_snap_ground, LBAdminMenuSettings.Get.objB_snap_ground);
		LoadSetting(btn_snap_object, LBAdminMenuSettings.Get.objB_snap_object);
		LoadSetting(btn_camera_rotation, LBAdminMenuSettings.Get.objB_camera_rotation);
	}

	override string GetTooltip(Widget w) {
		if (w == btn_snap_ground)
			return "#lb_adm_snap_to_surface";
		if (w == btn_snap_object)
			return "#lb_adm_snap_to_object";
		if (w == btn_camera_rotation)
			return "#lb_adm_camera_rotation";
		return super.GetTooltip(w);
	}

	void LoadSetting(ButtonWidget btn, bool enabled) {
		if (enabled)
			btn.GetParent().SetColor(ARGB(255, 0, 255, 0));
		else
			btn.GetParent().SetColor(ARGB(255, 255, 0, 0));
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_snap_ground) {
			LBAdminMenuSettings.Get.objB_snap_ground = !LBAdminMenuSettings.Get.objB_snap_ground;
			LoadSetting(btn_snap_ground, LBAdminMenuSettings.Get.objB_snap_ground);
		} else if (w == btn_snap_object) {
			LBAdminMenuSettings.Get.objB_snap_object = !LBAdminMenuSettings.Get.objB_snap_object;
			LoadSetting(btn_snap_object, LBAdminMenuSettings.Get.objB_snap_object);
		} else if (w == btn_camera_rotation) {
			LBAdminMenuSettings.Get.objB_camera_rotation = !LBAdminMenuSettings.Get.objB_camera_rotation;
			LoadSetting(btn_camera_rotation, LBAdminMenuSettings.Get.objB_camera_rotation);
		} else {
			return false;
		}
		LBAdminMenuSettings.Loader.Save();
		return false;
	}

}