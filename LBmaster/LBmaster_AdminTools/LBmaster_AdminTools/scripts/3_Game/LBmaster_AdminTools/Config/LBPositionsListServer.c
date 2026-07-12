class LBPositionsListServer : LBConfigLoader<LBPositionsListServer_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "teleportpositions.json", LBConfigType.CONFIG, true, "admin.view.teleport_positions", "admin.view.teleport_positions"); // (medium)
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

}
// Should not be edited manually, but only ingame through the admin menu
class LBPositionsListServer_ : LBPositionsListBase {

	static const int CURRENT_VERSION = 1;
	ref array<ref LB_TeleportPos> positions = new array<ref LB_TeleportPos>(); // List of all positions saved on the server

	override array<ref LB_TeleportPos> GetPositions() {
		return positions;
	}

	override void RemovePosition(LB_TeleportPos pos) {
		positions.RemoveItem(pos);
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.POSITION_REMOVE, new Param1<ref LB_TeleportPos>(pos), true);
	}

	override void AddPosition(LB_TeleportPos pos) {
		positions.Insert(pos);
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.POSITION_ADD, new Param1<ref LB_TeleportPos>(pos), true);
	}

	override void ChangePosition(LB_TeleportPos pos, LB_TeleportPos old) {
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.POSITION_UPDATE, new Param2<ref LB_TeleportPos, ref LB_TeleportPos>(pos, old), true);
	}

}