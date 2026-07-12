modded class Weapon_Base {

	static vector last_Cam_dir, lb_current_cam;

	override void EEFired(int muzzleType, int mode, string ammoType) {
		super.EEFired(muzzleType, mode, ammoType);
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (player) {
#ifndef SERVER
			last_Cam_dir = g_Game.GetCurrentCameraDirection();
			if (player.IsControlledPlayer())
				player.LBOnUnlimitedAmmo();
			LBBulletTrajectoryCheck(player, ammoType);
#else
			player.LBOnUnlimitedAmmo();
#endif
		}
	}

	override bool JamCheck(int muzzleIndex) {
		if (super.JamCheck(muzzleIndex)) {
			PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
			return !player || !player.LBIsModifierActive(LBPlayerModifier.NO_JAM);
		}
		return false;
	}

#ifndef SERVER

	override void OnFire(int muzzle_index) {
		super.OnFire(muzzle_index);
		vector nowCam = g_Game.GetCurrentCameraDirection();
		if (nowCam != last_Cam_dir) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.BULLET_CAM, new Param3<vector, vector, vector>(lb_current_cam, last_Cam_dir, nowCam), true);
		} else if (nowCam != lb_current_cam) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.BULLET_CAM, new Param3<vector, vector, vector>(lb_current_cam, last_Cam_dir, nowCam), true);
		}
	}

	void LBBulletTrajectoryCheck(PlayerBase player, string ammo) {
		vector konec_hlavne_position = ModelToWorld(GetMemoryPointPos("konec hlavne"));
		vector usti_hlavne_position = ModelToWorld(GetMemoryPointPos("usti hlavne"));
		vector dir = usti_hlavne_position - konec_hlavne_position;

		if (!FindBullets(player, konec_hlavne_position, ammo, usti_hlavne_position) && !FindBullets(player, usti_hlavne_position, ammo, usti_hlavne_position)) {}
	}

	bool FindBullets(PlayerBase player, vector pos, string ammo, vector end) {
		array<Object> objects = new array<Object>();
		array<CargoBase> cargos = new array<CargoBase>();
		g_Game.GetObjectsAtPosition3D(pos, 0.5, objects, cargos);
		foreach (Object obj : objects) {
			if (!obj)
				continue;
			string name = obj.GetType();
			if (name == "")
				name = obj.GetDebugNameNative();
			if (name != ammo)
				continue;
			vector playerSpeed = player.GetSpeed();
			vector normalizedSpeed = obj.GetSpeed() - playerSpeed;
			float speed = normalizedSpeed.Length();
			if (speed < 0.1)
				return true;

			vector camDir = g_Game.GetCurrentCameraDirection();
			vector camPos = g_Game.GetCurrentCameraPosition();

			int stance = player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_ERECT) * DayZPlayerConstants.STANCEMASK_ERECT;
			stance |= player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH) * DayZPlayerConstants.STANCEMASK_CROUCH;
			stance |= player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE) * DayZPlayerConstants.STANCEMASK_PRONE;
			stance |= player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDERECT) * DayZPlayerConstants.STANCEMASK_RAISEDERECT;
			stance |= player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDCROUCH) * DayZPlayerConstants.STANCEMASK_RAISEDCROUCH;
			stance |= player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDPRONE) * DayZPlayerConstants.STANCEMASK_RAISEDPRONE;

			g_Game.RPCSingleParam(null, LBAdminToolRPCs.BULLET_TRAJECTORY, new Param10<string, Weapon_Base, vector, int, vector, vector, vector, vector, vector, vector>(ammo, this, player.GetPosition(), stance, pos, end, obj.GetPosition(), normalizedSpeed, camPos, camDir), true);
			return true;
		}
		return false;
	}

#endif
}