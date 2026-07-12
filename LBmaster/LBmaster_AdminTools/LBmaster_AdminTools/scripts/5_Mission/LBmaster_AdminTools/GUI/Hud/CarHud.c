modded class CarHud {

	override void LBSetVehicle(EntityAI vehicle) {
		m_CurrentVehicle = CarScript.Cast(vehicle);
		if (!m_CurrentVehicle)
			return;

		m_VehicleGearCount	= m_CurrentVehicle.GetGearCount();

		float rpm_value_red = m_CurrentVehicle.EngineGetRPMRedline() / m_CurrentVehicle.EngineGetRPMMax();
		m_VehicleRPMDial.SetMaskProgress(rpm_value_red);
		m_VehicleRPMRedline.SetMaskProgress(1 - rpm_value_red);

		m_VehiclePanel.Show(true);
	}

}