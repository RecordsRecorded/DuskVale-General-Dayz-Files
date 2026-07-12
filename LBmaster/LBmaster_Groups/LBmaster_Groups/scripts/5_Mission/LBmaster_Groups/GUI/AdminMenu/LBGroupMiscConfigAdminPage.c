[DayZGame.RegisterLBAdminPage("LBGroupMiscConfigAdminPage", "LBmaster_Groups/logo/logo.paa", "Misc Settings", "groups.change.miscconfig")]
class LBGroupMiscConfigAdminPage : LBAdmin_Menu_Page {

	TextListboxWidget list_legend, list_compass_items, list_gps_items, list_gps_vehicles, list_map_items;
	CheckBoxWidget chckbx_enable_legend, chckbx_compass, chckbx_compass_item, chckbx_gps, chckbx_gps_heading, chckbx_gps_speed, chckbx_gps_coords, chckbx_gps_item;
	CheckBoxWidget chckbx_gps_item_veh, chckbx_map_item, chckbx_disable_map_menu;
	ButtonWidget btn_add_legend, btn_del_legend, btn_legend_icon, btn_change_color_icon, btn_change_color_text, btn_add_compass, btn_del_compass, btn_add_gps;
	ButtonWidget btn_del_gps, btn_add_vehicle, btn_del_vehicle, btn_add_map, btn_del_map, btn_not_found_browse;
	Widget panel_icon_color, panel_text_color;
	ImageWidget legend_icon;
	EditBoxWidget edit_legend_title, edit_legend_icon, edit_legend_text, edit_map_not_found_txt, edit_map_not_found_img;

	override void OnPageShown() {
		super.OnPageShown();
		linked.LoadLinkedVars();
	}

	override array<string> GetEditedConfigs() {
		return {"LBGroupMiscConfig"};
	}

	override void RegisterAllLinkedVars() {
		// Legend
		linked.RegisterLinkedVar("GetConfig().enableMapLegend", chckbx_enable_legend);
		linked.RegisterLinkedVar("GetConfig().mapLegendTitle", edit_legend_title);
		linked.RegisterLinkedList("GetConfig().mapLegend", list_legend, btn_add_legend, btn_del_legend).SetListManager(new LBListManager<ref MapLegendItem>());
		linked.RegisterLinkedVar("GetConfig().mapLegend[list_legend].iconPath", edit_legend_icon).EnableIconOutput(legend_icon).EnableFileSelector({".paa", ".edds"}, btn_legend_icon).SetReloadTrigger(list_legend);
		linked.RegisterLinkedVar("GetConfig().mapLegend[list_legend].name", edit_legend_text);
		linked.RegisterLinkedVar("GetConfig().mapLegend[list_legend].iconColor", btn_change_color_icon).EnableColorPicker(true, "A", "R", "G", "B", panel_icon_color);
		linked.RegisterLinkedVar("GetConfig().mapLegend[list_legend].nameColor", btn_change_color_text).EnableColorPicker(true, "A", "R", "G", "B", panel_text_color);
		// Items
		TStringSet itemSet = LBInherit.Get().GetAllChildren({"Inventory_Base"}, false, true, true);
		linked.RegisterLinkedVar("GetConfig().enableCompassHud", chckbx_compass);
		linked.RegisterLinkedVar("GetConfig().compassRequireItem", chckbx_compass_item);
		linked.RegisterLinkedList("GetConfig().compassItems", list_compass_items, btn_add_compass, btn_del_compass, null).EnableItemSelector(true, "Inventory_Base").EnableInListSearch(itemSet);
		linked.RegisterLinkedVar("GetConfig().enableGPS", chckbx_gps);
		linked.RegisterLinkedVar("GetConfig().gpsDisplayAngle", chckbx_gps_heading);
		linked.RegisterLinkedVar("GetConfig().gpsDisplaySpeed", chckbx_gps_speed);
		linked.RegisterLinkedVar("GetConfig().gpsDisplayCoords", chckbx_gps_coords);
		linked.RegisterLinkedVar("GetConfig().gpsRequireItem", chckbx_gps_item);
		linked.RegisterLinkedList("GetConfig().gpsItems", list_gps_items, btn_add_gps, btn_del_gps, null).EnableItemSelector(true, "Inventory_Base").EnableInListSearch(itemSet);
		linked.RegisterLinkedVar("GetConfig().gpsOnlyInVehicles", chckbx_gps_item_veh);
		TStringSet itemVehicles = LBInherit.Get().GetAllChildren({"Transport"}, false, true, true);
		linked.RegisterLinkedList("GetConfig().vehiclesWithGPS", list_gps_vehicles, btn_add_vehicle, btn_del_vehicle, null).EnableItemSelector(false, "Transport").EnableInListSearch(itemVehicles);
		linked.RegisterLinkedVar("GetConfig().mapRequireItem", chckbx_map_item);
		linked.RegisterLinkedList("GetConfig().mapItems", list_map_items, btn_add_map, btn_del_map, null).EnableItemSelector(true, "Inventory_Base").EnableInListSearch(itemSet);
		linked.RegisterLinkedVar("GetConfig().mapNotFoundText", edit_map_not_found_txt);
		linked.RegisterLinkedVar("GetConfig().mapNotFoundImage", edit_map_not_found_img).EnableFileSelector({".paa", ".edds"}, btn_not_found_browse);
		linked.RegisterLinkedVar("GetConfig().disableMapMenu", chckbx_disable_map_menu);

	}

	LBGroupMiscConfig_ GetConfig() {
		return LBGroupMiscConfig.Get;
	}
}