modded class IngameHud {

	bool hasSpectatorVehicleHud = false;

	override void ShowVehicleInfo() {
		if (!DayZPlayerCameraSpectator.currentSpecCamera || !DayZPlayerCameraSpectator.currentSpecCamera.targetObj) {
			super.ShowVehicleInfo();
			return;
		}
		Transport vehicle = Transport.Cast(DayZPlayerCameraSpectator.currentSpecCamera.targetObj.GetParent());
		if (!vehicle)
			return;

		VehicleHudBase vehHud = m_VehicleHudMap.Get(vehicle.GetVehicleType());
		if (vehHud) {
			vehHud.ShowVehicleInfo(DayZPlayerCameraSpectator.currentSpecCamera.targetObj);
			vehHud.LBSetVehicle(EntityAI.Cast(DayZPlayerCameraSpectator.currentSpecCamera.targetObj.GetParent()));
			m_ActiveVehicleHUD = vehHud;
			m_ActiveVehicleHUD.ShowPanel();
			ShowVehicleHud(true);
		}
	}

	override void Update(float timeslice) {
		super.Update(timeslice);
		if (!DayZPlayerCameraSpectator.currentSpecCamera) {
			if (hasSpectatorVehicleHud) {
				HideVehicleInfo();
				hasSpectatorVehicleHud = false;
				LBLogger.Info("Hiding vehicle hud after spectate", "AdminTools");
			}
			return;
		}

		if (m_ActiveVehicleHUD && (!DayZPlayerCameraSpectator.currentSpecCamera.targetObj || !DayZPlayerCameraSpectator.currentSpecCamera.targetObj.GetParent())) {
			HideVehicleInfo();
			hasSpectatorVehicleHud = false;
			LBLogger.Info("Hiding vehicle hud in spectate", "AdminTools");
		} else if (!m_ActiveVehicleHUD && DayZPlayerCameraSpectator.currentSpecCamera.targetObj && DayZPlayerCameraSpectator.currentSpecCamera.targetObj.GetParent()) {
			ShowVehicleInfo();
			hasSpectatorVehicleHud = true;
			LBLogger.Info("Showing vehicle hud in spectate", "AdminTools");
		}
	}

}