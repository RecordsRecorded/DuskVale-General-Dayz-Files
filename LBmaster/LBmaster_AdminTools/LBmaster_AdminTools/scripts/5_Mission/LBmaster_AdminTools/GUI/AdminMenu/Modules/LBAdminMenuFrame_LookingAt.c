class LBAdminMenuFrame_LookingAt : LBAdminMenuFrame {

	override string GetTitle() {
		return "#lb_adm_title_looking_at_info";
	}

	override string GetButtonIcon() {
		return "LBmaster_AdminTools/gui/icons/looking_at.paa";
	}

	override void GetMinSize(out int width, out int height) {
		width = 300;
		height = 20;
	}

	override void GetInitialSize(out int width, out int height) {
		width = 1000;
		height = 40;
	}

	override TStringArray GetRequiredPermissionsOne() {
		return {"admin.view.target_object_info"};
	}

	override string GetInfoString() {
		return "#lb_adm_desc_looking_at_info";
	}

	static const ref TStringArray list_default_pos_formats = {"<x, y, z>", "<x y z>", "x, y, z", "x y z", "custom"};
	static const ref TStringArray list_default_dir_formats = {"<yaw, pitch, roll>", "yaw, pitch, roll", "yaw pitch roll", "yaw;yaw pitch", "yaw, pitch", "custom"};

	TextWidget txt_item, txt_pos, txt_dir, txt_lifetime;
	CheckBoxWidget chckbx_copy_item, chckbx_copy_pos, chckbx_copy_dir;
	XComboBoxWidget combo_pos_format, combo_dir_format;
	EditBoxWidget edit_dir_custom_format, edit_pos_custom_format;
	Widget panel_dir_format, panel_pos_format;

	Object lastObj;
	int lastLifetimeRequest = 0;
	string type;
	vector pos, dir;

	override void Show(bool show) {
		super.Show(show);
		combo_dir_format.ClearAll();
		foreach (string dirFormat : list_default_dir_formats)
			combo_dir_format.AddItem(dirFormat);
		combo_pos_format.ClearAll();
		foreach (string posFormat : list_default_pos_formats)
			combo_pos_format.AddItem(posFormat);
		UpdateCheckboxes();
	}

	override void OnRPC(Object target, int rpc_type, ParamsReadContext ctx) {
		if (rpc_type == LBAdminToolRPCs.OBJECT_LIFETIME_INFO) {
			Param5<int, int, int, int, int> info;
			if (!ctx.Read(info) || lastLifetimeRequest != info.param1)
				return;
			string max = LBDate.Init(info.param3).ToDiffString();
			string maxDB = LBDate.Init(info.param4).ToDiffString();
			string remain = LBDate.Init(info.param5).ToDiffString();
			if (info.param2 == 0) {
				txt_lifetime.SetText("Not in types.xml. Max: " + max + " Remain: " + remain);
			} else if (info.param2 == -1) {
				txt_lifetime.SetText("Client side Object.");
			} else {
				float percent = 0.0;
				if (info.param3 > 0)
					percent = ((float) info.param5) / info.param3;
				txt_lifetime.SetText("Found in types.xml. Max: " + max + " Remain: " + remain + " (" + ((int)(percent * 100)) + "%) Types Lifetime:" + maxDB);
			}
		}
	}

	void UpdateCheckboxes() {
		chckbx_copy_item.SetChecked(LBAdminMenuSettings.Get.lookingAtCopyItemname);
		chckbx_copy_pos.SetChecked(LBAdminMenuSettings.Get.lookingAtCopyPos);
		chckbx_copy_dir.SetChecked(LBAdminMenuSettings.Get.lookingAtCopyDir);
		combo_pos_format.SetCurrentItem(LBAdminMenuSettings.Get.lookAtPositionFormat);
		combo_dir_format.SetCurrentItem(LBAdminMenuSettings.Get.lookAtDirectionFormat);
		UpdateCustomFormatVisibility();
	}

	void UpdateCustomFormatVisibility() {
		if (combo_dir_format.GetCurrentItem() == combo_dir_format.GetNumItems() - 1) {
			panel_dir_format.Show(true);
			edit_dir_custom_format.SetText(LBAdminMenuSettings.Get.lookAtDirectionCustomFormat);
		} else
			panel_dir_format.Show(false);

		if (combo_pos_format.GetCurrentItem() == combo_pos_format.GetNumItems() - 1) {
			panel_pos_format.Show(true);
			edit_pos_custom_format.SetText(LBAdminMenuSettings.Get.lookAtPositionCustomFormat);
		} else
			panel_pos_format.Show(false);

	}

	override void Update(float timeslice, bool otherMenu, bool leftClicked) {
		super.Update(timeslice, otherMenu, leftClicked);
		if (IsVisible()) {
			UpdateItemInfo();
			if (GetUApi().GetInputByName("UALBMADMCopyTarget").LocalPress() && type != "NONE") {
				string text = "";
				if (chckbx_copy_item.IsChecked())
					text = type + " ";
				if (chckbx_copy_pos.IsChecked())
					text = text + GetPositionFormatted() + " ";
				if (chckbx_copy_dir.IsChecked())
					text = text + GetDirectionFormatted() + " ";
				text = text.Trim();
				if (text != "") {
					g_Game.CopyToClipboard(text);
					LBCopiedPopup.ShowAtCursor();
				}
				GetUApi().SupressNextFrame(true);
			}
		}
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (super.OnClick(w, x, y, button))
			return true;
		if (w == chckbx_copy_item) {
			LBAdminMenuSettings.Get.lookingAtCopyItemname = chckbx_copy_item.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == chckbx_copy_pos) {
			LBAdminMenuSettings.Get.lookingAtCopyPos = chckbx_copy_pos.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == chckbx_copy_dir) {
			LBAdminMenuSettings.Get.lookingAtCopyDir = chckbx_copy_dir.IsChecked();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == combo_pos_format) {
			LBAdminMenuSettings.Get.lookAtPositionFormat = combo_pos_format.GetCurrentItem();
			LBAdminMenuSettings.Loader.Save();
			UpdateCustomFormatVisibility();
			return true;
		} else if (w == combo_dir_format) {
			LBAdminMenuSettings.Get.lookAtDirectionFormat = combo_dir_format.GetCurrentItem();
			LBAdminMenuSettings.Loader.Save();
			UpdateCustomFormatVisibility();
			return true;
		}
		return false;
	}

	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (super.OnChange(w, x, y, finished))
			return true;
		if (w == edit_pos_custom_format) {
			LBAdminMenuSettings.Get.lookAtPositionCustomFormat = edit_pos_custom_format.GetText();
			LBAdminMenuSettings.Loader.Save();
			return true;
		} else if (w == edit_dir_custom_format) {
			LBAdminMenuSettings.Get.lookAtDirectionCustomFormat = edit_dir_custom_format.GetText();
			LBAdminMenuSettings.Loader.Save();
			return true;
		}
		return false;
	}

	void RequestLifetimeInfo(Object obj) {
		int scope = obj.ConfigGetInt("scope");
		EntityAI ent = EntityAI.Cast(obj);
		if (scope != 2 || !ent) {
			txt_lifetime.SetText("Cannot set lifetime");
			return;
		}
		txt_lifetime.SetText("Waiting for info...");
		++lastLifetimeRequest;
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.OBJECT_LIFETIME_INFO, new Param2<EntityAI, int>(ent, lastLifetimeRequest), true);
	}

	string GetPositionFormatted() {
		int index = combo_pos_format.GetCurrentItem();
		string format = list_default_pos_formats.Get(index);
		if (index == combo_pos_format.GetNumItems() - 1)
			format = LBAdminMenuSettings.Get.lookAtPositionCustomFormat + "";
		LBStringTools.SafeReplace(format, "x", "" + pos[0]);
		LBStringTools.SafeReplace(format, "y", "" + pos[1]);
		LBStringTools.SafeReplace(format, "z", "" + pos[2]);
		return format;
	}

	string GetDirectionFormatted() {
		int index = combo_dir_format.GetCurrentItem();
		string format = list_default_dir_formats.Get(index);
		if (index == combo_dir_format.GetNumItems() - 1)
			format = LBAdminMenuSettings.Get.lookAtDirectionCustomFormat + "";
		LBStringTools.SafeReplace(format, "yaw", "" + dir[0]);
		LBStringTools.SafeReplace(format, "y", "" + dir[0]);
		LBStringTools.SafeReplace(format, "pitch", "" + dir[1]);
		LBStringTools.SafeReplace(format, "p", "" + dir[1]);
		LBStringTools.SafeReplace(format, "roll", "" + dir[2]);
		LBStringTools.SafeReplace(format, "r", "" + dir[2]);
		return format;
	}

	void UpdateItemInfo() {
		dir = g_Game.GetCurrentCameraDirection();
		vector camPos = g_Game.GetCurrentCameraPosition();
		vector endPos = camPos + dir * 1000;
		array<ref RaycastRVResult> results = new array<ref RaycastRVResult>();
		RaycastRVParams params = new RaycastRVParams(camPos, endPos, g_Game.GetPlayer());
		params.type = ObjIntersectIFire;
		if (DayZPhysics.RaycastRVProxy(params, results) && results.Count() > 0 && results.Get(0).obj) {
			RaycastRVResult result = results.Get(0);
			Object hitObj = result.obj;
			if (!hitObj)
				hitObj = result.parent;
			type = hitObj.GetType();
			pos = hitObj.GetPosition();
			dir = hitObj.GetOrientation();
			if (type == "") {
				type = hitObj.GetDebugNameNative();
				int index = type.IndexOf(": ");
				if (index != -1)
					type = type.Substring(index + 2, type.Length() - index - 2);
				if (type.IndexOf("NOID") == 0 && result.parent) {
					Object hitParent = Object.Cast(result.parent);
					if (hitParent) {
						type = hitParent.GetType() + " -> Proxy: " + type.Substring(5, type.Length() - 5);
						pos = hitParent.GetPosition();
						dir = hitParent.GetOrientation();
					}
				}
			}
			txt_item.SetText("#lb_adm_looking_at_item: " + type);
			txt_pos.SetText("#lb_adm_looking_at_pos: " + GetPositionFormatted());
			txt_dir.SetText("#lb_adm_looking_at_dir: " + GetDirectionFormatted());
			if (lastObj != hitObj) {
				RequestLifetimeInfo(hitObj);
			}
			lastObj = hitObj;
		} else {
			lastObj = null;
			if (results.Count() > 0) {
				result = results.Get(0);
				type = "#lb_adm_ground";
				if (result.surface) {
					type = "#lb_adm_ground: " + result.surface.GetEntryName();
				}
				pos = result.pos;
				txt_item.SetText("#lb_adm_looking_at_item: " + type);
				txt_pos.SetText("#lb_adm_looking_at_pos: " + GetPositionFormatted());
				txt_dir.SetText("");
			} else {
				type = "#lb_adm_none";
				pos = vector.Zero;
				dir = vector.Zero;
				txt_item.SetText("#lb_adm_none");
				txt_pos.SetText("");
				txt_dir.SetText("");
			}
		}
	}

}