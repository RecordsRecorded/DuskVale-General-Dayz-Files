modded class ModItemRegisterCallbacks {

	override void RegisterRestrained(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior) {
		super.RegisterRestrained(pType, pBehavior);
		pType.AddItemInHandsProfileIK("LBAdminHandcuffs", "dz/anims/workspaces/player/player_main/player_main_1h_restrained.asi", pBehavior, "dz/anims/anm/player/ik/gear/handcuffslocked.anm");

	}

}