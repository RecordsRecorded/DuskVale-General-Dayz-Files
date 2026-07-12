typedef Param3<string, ref vector, int> LBWidgetPosition;

class LBPositionManager : LBConfigLoader<LBPositionManager_> {

	[SetPriority(LBConfigPriority.LOW)]
	override void InitVars() {
		InitVarsInternal("LBGroup", "PositionManager.json");
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}

class LBPositionManager_ : LBConfigBase {

	static const int CURRENT_VERSION = 0;

	ref array<ref LBWidgetPosition> positions = new array<ref LBWidgetPosition>();
	string changedPosition = "";

	static ref ScriptInvoker Event_OnPositionChange = new ScriptInvoker();

	override bool OnLoad() {
		if (positions == null || positions.Count() == 0) {
			positions = new array<ref LBWidgetPosition>();
			LoadDefault();
			InvokeOnChanged();
			return true;
		}
		LoadDefault();
		InvokeOnChanged();
		return false;
	}

	void ResetAll() {
		positions.Clear();
		LoadDefault();
		InvokeOnChanged();
	}

	override bool JsonLoadVar(string path, bool forceValid, out bool overwriteTest) {
		return LBJsonLoader<array<ref LBWidgetPosition>>.JsonLoadFile(path, positions, forceValid);
	}

	override void JsonSaveVar(string path, out bool overwriteTest) {
		LBJsonLoader<array<ref LBWidgetPosition>>.JsonSaveFile(path, positions);
	}

	override void LoadDefault() {
		LBLogger.Debug("Getting default Layout Positions", "AdvancedGroups");
		GetPosition("PlayerList", Vector(0.02, 0.02, 0), 0);
		GetPosition("Minimap", Vector(0.97, 0.83, 0), 8);
#ifndef LB_DISABLE_CHAT
		GetPosition("Chat", Vector(0.05, 0.85, 0), 6);
#endif
	}

	bool IsAtDefaultPos(string posStr) {
		LBWidgetPosition pos = null;
		foreach (LBWidgetPosition all : positions) {
			if (all.param1 == posStr) {
				pos = all;
				break;
			}
		}
		if (!pos)
			return true;
		if (posStr == "Chat") {
			return vector.Distance(Vector(0.05, 0.85, 0), pos.param2) < 0.01 && pos.param3 == 6;
		}
		return true;
	}

	static void InvokeOnChanged() {
		Event_OnPositionChange.Invoke();
	}

	vector GetPosition(string locStr, vector defaultLoc = vector.Zero, int defaultIndex = 0, bool insert = true) {
		LBLogger.Debug("Position Manager: " + locStr + " " + defaultLoc + " " + defaultIndex + " " + insert, "AdvancedGroups");
		if (!insert)
			return vector.Zero;
		if (changedPosition == locStr) {
			SetPosition(locStr, defaultLoc);
			SetIndex(locStr, defaultIndex);
			changedPosition = "";
		}
		foreach (LBWidgetPosition pos : positions) {
			if (pos && pos.param1 == locStr)
				return pos.param2;
		}
		LBLogger.Debug("Inserting New pos", "AdvancedGroups");
		positions.Insert(new LBWidgetPosition(locStr, defaultLoc, defaultIndex));
		return defaultLoc;
	}

	int GetIndex(string posStr) {
		foreach (LBWidgetPosition pos : positions) {
			if (pos && pos.param1 == posStr)
				return pos.param3;
		}
		return 0;
	}

	void SetPosition(string posStr, vector pos, bool add = true) {
		foreach (LBWidgetPosition poss : positions) {
			if (poss && poss.param1 == posStr) {
				poss.param2 = pos;
				return;
			}
		}
		if (add)
			positions.Insert(new LBWidgetPosition(posStr, pos, 0));
	}

	void SetIndex(string posStr, int index, bool add = true) {
		foreach (LBWidgetPosition poss : positions) {
			if (poss && poss.param1 == posStr) {
				poss.param3 = index;
				return;
			}
		}
		if (add)
			positions.Insert(new LBWidgetPosition(posStr, vector.Zero, index));
	}

	void ResetPositionToDefault(string posStr) {
		changedPosition = posStr;
		LoadDefault();
	}

	TStringArray GetPositionStrings() {
		TStringArray arr = new TStringArray();
		foreach (LBWidgetPosition pos : positions) {
			arr.Insert(pos.param1);
		}
		return arr;
	}

}