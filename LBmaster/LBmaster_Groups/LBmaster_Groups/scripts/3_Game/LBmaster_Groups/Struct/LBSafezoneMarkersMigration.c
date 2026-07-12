class LBSafezoneMarkersMigration {

	string playerTagPrefix = "#lb_ag_marker_player "; // Prefix for normal players (is not displayed when name obfuscation is enabled)
	ref LBColorConfig playerTagColor = LBColorConfig.Init(255, 51, 153, 255); // Color of the nametag for players
	string adminTagPrefix = "#lb_ag_marker_admin "; // Prefix for admins, which have the `"groups.nametags.admin"` permission set
	ref LBColorConfig adminTagColor = LBColorConfig.Init(255, 255, 0, 0); // Color of the Admin nametag

}