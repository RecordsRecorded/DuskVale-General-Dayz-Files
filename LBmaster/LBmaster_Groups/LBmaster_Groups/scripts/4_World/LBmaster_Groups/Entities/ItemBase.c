modded class ItemBase {

	void OnInventoryUpdateGPS() {
		LB_Elevator_World elevator = GetElevatorWorld();
		if (elevator) {
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(elevator.OnItemInInventoryChanged);
		}
	}

	override void OnInventoryExit(Man player) {
		super.OnInventoryExit(player);
		if (g_Game.IsClient())
			OnInventoryUpdateGPS();
	}
	override void OnInventoryEnter(Man player) {
		super.OnInventoryEnter(player);
		if (g_Game.IsClient())
			OnInventoryUpdateGPS();
	}

}