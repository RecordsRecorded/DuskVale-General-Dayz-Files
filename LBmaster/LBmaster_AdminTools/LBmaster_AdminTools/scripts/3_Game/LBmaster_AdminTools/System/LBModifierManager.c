class LBModifierManager {

	static ref LBModifierManager g_LBModifierManager;

	static LBModifierManager Get() {
		if (!g_LBModifierManager)
			g_LBModifierManager = new LBModifierManager();
		return g_LBModifierManager;
	}

	ref array<ref LBModifierInfo> infos = new array<ref LBModifierInfo>();
	ref map<int, LBModifierInfo> cache = new map<int, LBModifierInfo>();

	void LBModifierManager() {
		Register(new LBModifierInfo(LBPlayerModifier.FREEZE, "#lb_adm_frozen", false, true, "admin.set.freeze", "admin.set.freeze", "admin.set.freeze_multi", "#lb_adm_frozen_desc", "#lb_adm_desc_freeze", "#lb_adm_unfreeze", "#lb_adm_freeze", "btn_freeze"));
		Register(new LBModifierInfo(LBPlayerModifier.INVISIBLE, "#lb_adm_invisible", true, true, "admin.cheat.invisible", "admin.set.invisible", "admin.set.invisible_multi", "#lb_adm_invisible_desc", "#lb_adm_desc_invisible", "#lb_adm_set_visible", "#lb_adm_set_invisible", "btn_invis"));
		Register(new LBModifierInfo(LBPlayerModifier.GODMODE, "#lb_adm_godmode", true, false, "admin.cheat.godmode", "admin.set.godmode", "admin.set.godmode_multi", "#lb_adm_godmode_desc", "#lb_adm_desc_godmode", "#lb_adm_disable_godmode", "#lb_adm_enable_godmode", "btn_god"));
		Register(new LBModifierInfo(LBPlayerModifier.UNLIMITED_AMMO, "#lb_adm_unlimited_ammo", true, false, "admin.cheat.unlimitedammo", "admin.set.unlimited_ammo", "admin.set.unlimited_ammo_multi", "#lb_adm_unlimited_ammo_desc", "#lb_adm_desc_unlimited_ammo", "#lb_adm_disable_unlimited_ammo", "#lb_adm_enable_unlimited_ammo", "btn_ammo"));
		Register(new LBModifierInfo(LBPlayerModifier.VEHICLE_GODMODE, "#lb_adm_vehicle_godmode", true, false, "admin.cheat.godmode_vehicle", "admin.set.vehicle_godmode", "admin.set.vehicle_godmode_multi", "#lb_adm_vehicle_godmode_desc", "#lb_adm_desc_vehicle_godmode", "#lb_adm_disable_vehicle_godmode", "#lb_adm_enable_vehicle_godmode", "btn_god_veh"));
		Register(new LBModifierInfo(LBPlayerModifier.PRISTINE_HANDS, "#lb_adm_pristine_hands", true, false, "admin.cheat.pristinehands", "admin.set.pristine_hands", "admin.set.pristine_hands_multi", "#lb_adm_pristine_hands_desc", "#lb_adm_desc_pristine_hands", "#lb_adm_disable_pristine_hands", "#lb_adm_enable_pristine_hands", "btn_prist_hands"));
		Register(new LBModifierInfo(LBPlayerModifier.UNLIMITED_STAMINA, "#lb_adm_unlimited_stamina", true, false, "admin.cheat.unlimited_stamina", "admin.set.unlimited_stamina", "admin.set.unlimited_stamina_multi", "#lb_adm_unlimited_stamina_desc", "#lb_adm_desc_unlimited_stamina", "#lb_adm_disable_unlimited_stamina", "#lb_adm_enable_unlimited_stamina", "btn_stamina"));
		Register(new LBModifierInfo(LBPlayerModifier.ACTION_SPEEDHACK, "#lb_adm_action_speedhack", true, false, "admin.cheat.action_speedhack", "admin.set.action_speedhack", "admin.set.action_speedhack_multi", "#lb_adm_action_speedhack_desc", "#lb_adm_desc_action_speedhack", "#lb_adm_disable_action_speedhack", "#lb_adm_enable_action_speedhack", "btn_action_speed"));
		Register(new LBModifierInfo(LBPlayerModifier.IGNORED_BY_AI, "#lb_adm_ai_ignores_player", true, false, "admin.cheat.ignore_ai", "admin.set.ignore_ai", "admin.set.ignore_ai_multi", "#lb_adm_ai_ignores_player_desc", "#lb_adm_desc_ai_ignores_player", "#lb_adm_disable_ignored_by_ai", "#lb_adm_enable_ignored_by_ai", "btn_ignore_ai"));
		Register(new LBModifierInfo(LBPlayerModifier.SILENT, "#lb_adm_silent", true, true, "admin.cheat.silent", "admin.set.silent", "admin.set.silent_multi", "#lb_adm_silent_desc", "#lb_adm_desc_silent_desc", "#lb_adm_enable_sounds", "#lb_adm_disable_sound", "btn_silent"));
		Register(new LBModifierInfo(LBPlayerModifier.NO_JAM, "#lb_adm_no_jam", true, false, "admin.cheat.no_jam", "admin.set.no_jam", "admin.set.no_jam_multi", "#lb_adm_no_jam_desc", "#lb_adm_desc_no_jam", "#lb_adm_enable_no_jam", "#lb_adm_disable_no_jam", "btn_no_jam"));
		Register(new LBModifierInfo(LBPlayerModifier.INVINCIBLE, "#lb_adm_invincible", true, false, "admin.cheat.invincible", "admin.set.invincible", "admin.set.invincible_multi", "#lb_adm_invincible_desc", "#lb_adm_desc_invincible", "#lb_adm_enable_invincible", "#lb_adm_disable_invincible", "btn_invincible"));
		Register(new LBModifierInfo(LBPlayerModifier.INVULNERABLE, "#lb_adm_invulnerable", true, false, "admin.cheat.invulnerable", "admin.set.invulnerable", "admin.set.invulnerable_multi", "#lb_adm_invulnerable_desc", "#lb_adm_desc_invulnerable", "#lb_adm_enable_invulnerable", "#lb_adm_disable_invulnerable", "btn_invulnerable"));
	}

	void Register(LBModifierInfo info) {
		infos.Insert(info);
		cache.Insert(info.id, info);
	}
	
	LBModifierInfo Get(int id) {
		return cache.Get(id);
	}

}