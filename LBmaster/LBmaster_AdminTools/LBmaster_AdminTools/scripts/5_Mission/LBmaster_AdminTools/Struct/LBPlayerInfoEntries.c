class LBPlayerInfoEntries {

	ref array<ref LBPlayerInfoEntry> entries = new array<ref LBPlayerInfoEntry>();
	Widget btn_apply_all;

	void LBPlayerInfoEntries() {
		InitEntries();
	}

	void InitEntries() {
		AddEntry(new LBPlayerInfoHealth("", "", "#lb_adm_health_2"));
		AddEntry(new LBPlayerInfoHealth("", "Blood", "#lb_adm_blood"));
		AddEntry(new LBPlayerInfoHealth("", "Shock", "#lb_adm_shock"));
		AddEntry(new LBPlayerInfoStat(EPlayerStats_current.WATER, 5000, 0, "#lb_adm_water"));
		AddEntry(new LBPlayerInfoStat(EPlayerStats_current.ENERGY, 5000, 0, "#lb_adm_energy"));
		AddEntry(new LBPlayerInfoStat(EPlayerStats_current.HEATCOMFORT, 1, -1, "#lb_adm_heat_comfort", 0.05));
		AddEntry(new LBPlayerInfoStat(EPlayerStats_current.HEATBUFFER, 30, 0, "#lb_adm_heat_buffer", 0.25));
#ifdef DZ_Expansion_Hardline
		AddEntry(new LBPlayerInfoExpansionReputation());
#endif
	}

	void InitWidgets(Widget parent, Widget btn_apply_all_) {
		btn_apply_all = btn_apply_all_;
		foreach (LBPlayerInfoEntry entry : entries) {
			entry.InitWidget(parent, this);
		}
		UpdateApplyAll();
	}

	void UpdateApplyAll() {
		foreach (LBPlayerInfoEntry entry: entries) {
			if (entry.btn_apply.IsVisible()) {
				btn_apply_all.Show(true);
				return;
			}
		}
		btn_apply_all.Show(false);
	}

	void ApplyAll() {
		array<ref Param2<float, int>> list = new array<ref Param2<float, int >> ();
		Param2<PlayerIdentity, ref array<ref Param2<float, int>>> param = new Param2<PlayerIdentity, ref array<ref Param2<float, int >>> (target, list);
		foreach (LBPlayerInfoEntry entry: entries) {
			if (entry.btn_apply.IsVisible()) {
				list.Insert(new Param2<float, int>(entry.clientValue, entry.index));
				entry.btn_apply.Show(false);
			}
		}
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.SET_PLAYER_STAT, param, true);
		UpdateApplyAll();
	}

	void AddEntry(LBPlayerInfoEntry entry) {
		entry.SetIndex(entries.Count());
		entries.Insert(entry);
	}

	void WritePlayer(PlayerBase player, ParamsWriteContext ctx) {
		array<float> values = new array<float>();
		foreach (LBPlayerInfoEntry entry : entries) {
			values.Insert(entry.GetValueFromPlayer(player));
		}
		ctx.Write(values);
	}

	string GetParamName(int index) {
		if (entries.Count() <= index || index < 0)
			return "Unknown";
		return entries.Get(index).display;
	}

	float GetParamValue(PlayerBase player, int index) {
		if (entries.Count() <= index || index < 0)
			return 0;
		return entries.Get(index).GetValueFromPlayer(player);
	}

	bool ApplyPlayer(PlayerBase player, float value, int index) {
		if (entries.Count() <= index || index < 0)
			return false;
		entries.Get(index).SetPlayerValue(player, value);
		return true;
	}

	int index;
	Widget parent;
	PlayerIdentity target;

	bool ReadPlayer(ParamsReadContext ctx, Widget parent_, Widget btn_apply_all_, PlayerIdentity target_) {
		array<float> values;
		target = target_;
		if (!ctx.Read(values))
			return false;
		if (values.Count() != entries.Count())
			return false;
		InitWidgets(parent_, btn_apply_all_);
		for (int i = 0; i < values.Count(); ++i) {
			entries.Get(i).SetFromServer(values.Get(i));
		}
		return true;
	}

	void ResetApply() {
		foreach (LBPlayerInfoEntry entry : entries) {
			if (entry.btn_apply)
				entry.btn_apply.Show(false);
		}
	}

	void SetValue(float value, int index_) {
		array<ref Param2<float, int>> list = new array<ref Param2<float, int >> ();
		Param2<PlayerIdentity, ref array<ref Param2<float, int>>> param = new Param2<PlayerIdentity, ref array<ref Param2<float, int >>> (target, list);
		list.Insert(new Param2<float, int>(value, index_));
		g_Game.RPCSingleParam(null, LBAdminToolRPCs.SET_PLAYER_STAT, param, true);
	}

}