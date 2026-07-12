modded class BoatHud {

	override void LBSetVehicle(EntityAI vehicle) {
		m_CurrentVehicle = BoatScript.Cast(vehicle);

		m_VehiclePanel.Show(true);

		m_EngineHealthLevel = -1;
		m_FuelLevel = -1;

		if (m_CurrentVehicle.HasEngine())
			m_HasEngine = true;

		float gearNeutral = m_CurrentVehicle.GetNeutralGear();
		m_VehicleGearTable.Set(gearNeutral - 2, "");
		m_VehicleGearTable.Set(gearNeutral - 1, "R");
		m_VehicleGearTable.Set(gearNeutral, "N");
		m_VehicleGearTable.Set(gearNeutral + 1, "F");
		m_VehicleGearTable.Set(gearNeutral + 2, "");

		m_EngagedGear = -10; // force initial update
	}

}