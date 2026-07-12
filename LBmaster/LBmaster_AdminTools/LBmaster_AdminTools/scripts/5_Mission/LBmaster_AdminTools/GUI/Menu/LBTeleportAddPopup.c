class LBTeleportAddPopup : ScriptedWidgetEventHandler {

	Widget layoutRoot;
	TextWidget txt_title;
	ButtonWidget btn_apply, btn_cancel, btn_here;
	EditBoxWidget edit_name, edit_pos, edit_orient;

	ref LB_TeleportPos position;
	LBAdminMenuFrame_TeleportPos parent;
	bool addNew;

	void LBTeleportAddPopup(LB_TeleportPos pos, bool isNew, LBAdminMenuFrame_TeleportPos parent_) {
		this.parent = parent_;
		this.addNew = isNew;
		layoutRoot = LBLayoutManager.Get().CreateLayout("LBTeleportAddPopup");
		layoutRoot.SetHandler(this);
		ConnectClassWidgetVariables(this, layoutRoot, {"layoutRoot"});
		this.position = pos;
		if (isNew) {
			txt_title.SetText("#lb_adm_add_position");
		} else {
			txt_title.SetText("#lb_adm_edit_position");
			edit_name.SetText(pos.name);
			edit_pos.SetText(pos.pos.ToString());
		}
		edit_orient.SetText(pos.dir.ToString());
	}

	void ~LBTeleportAddPopup() {
		if (layoutRoot) {
			layoutRoot.Unlink();
			layoutRoot = null;
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == btn_cancel) {
			parent.CloseAddPopup();
			return true;
		} else if (w == btn_apply) {
			if (edit_name.GetText().Trim() == "") {
				NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", "#lb_adm_message_position_not_empty", LBIconConfig.Get.error);
				return true;
			}
			vector outPos, outDir;
			if (!LBConsoleCommandTeleport.TryParsePos(edit_pos.GetText(), outPos)) {
				NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", "#lb_adm_message_position_format", LBIconConfig.Get.error);
				return true;
			}
			if (!LBConsoleCommandTeleport.TryParseDir(edit_orient.GetText(), outDir)) {
				NotificationSystem.AddNotificationExtended(5.0, "#lb_adm_admin_tools", "#lb_adm_message_orientation_format", LBIconConfig.Get.error);
				return true;
			}
			position.pos = outPos;
			position.dir = outDir;
			position.name = edit_name.GetText();
			if (addNew)
				parent.OnPositionAdd(position);
			else
				parent.OnPositionEdit(position);
			parent.CloseAddPopup();
			return true;
		} else if (w == btn_here) {
			if (LBFreecamCamera.instance) {
				edit_pos.SetText(LBFreecamCamera.position.ToString());
				edit_orient.SetText(LBFreecamCamera.orientation.ToString());
			} else if (g_Game.GetPlayer()) {
				edit_pos.SetText(g_Game.GetPlayer().GetPosition().ToString());
				edit_orient.SetText(g_Game.GetPlayer().GetOrientation().ToString());
			}
			return true;
		}
		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (EditBoxWidget.Cast(w) || CheckBoxWidget.Cast(w) || MultilineEditBoxWidget.Cast(w)) {
			SetFocus(w);
		}
		return super.OnMouseEnter(w, x, y);
	}

}