class LBConsoleCommandTeleport : LBConsoleCommandBase {

	[LBConsoleCommandHandler.Get().Register("LBConsoleCommandTeleport")]
	override void Register() {
		prefix = "tpp";
		parameter.Insert("<x,y,z> OR <x,z> OR x,y,z");
	}

	override bool HasPermission() {
		return LBAdmins.Get().HasPermission("admin.teleport.map");
	}

	override bool IsValid() {
		vector pos;
		return parsedParameter.Count() > 0 && rawParameter.Length() > 5 && TryParsePos(rawParameter.Substring(4, rawParameter.Length() - 4).Trim(), pos);
	}

	override void Execute() {
		vector pos;
		if (rawParameter.Length() > 5 && TryParsePos(rawParameter.Substring(4, rawParameter.Length() - 4).Trim(), pos)) {
			g_Game.RPCSingleParam(null, LBAdminToolRPCs.TP_MAP, new Param2<vector, vector>(pos, vector.Zero), true);
		}
	}

	static bool TryParsePos(string input, out vector pos) {
		TStringArray parts = new TStringArray();
		input.Split(",", parts);
		if (parts.Count() == 1) {
			parts.Clear();
			input.Split("/", parts);
		}
		float x, y, z;
		if (parts.Count() == 3) {
			if (ParseFloat(parts[0], x) && ParseFloat(parts[1], y) && ParseFloat(parts[2], z)) {
				pos = Vector(x, y, z);
				return true;
			}
		} else if (parts.Count() == 2) {
			if (ParseFloat(parts[0], x) && ParseFloat(parts[1], z)) {
				y = g_Game.SurfaceY(x, z);
				pos = Vector(x, y, z);
				return true;
			}
		}
		return false;
	}

	static bool TryParseDir(string input, out vector dir) {
		TStringArray parts = new TStringArray();
		input.Split(",", parts);
		if (parts.Count() == 1) {
			input.Split("/", parts);
		}
		float x, y, z;
		if (parts.Count() == 3) {
			if (ParseFloat(parts[0], x) && ParseFloat(parts[1], y) && ParseFloat(parts[2], z)) {
				dir = Vector(x, y, z);
				return true;
			}
		} else if (parts.Count() == 2) {
			if (ParseFloat(parts[0], x) && ParseFloat(parts[2], z)) {
				dir = Vector(x, 0, z);
				return true;
			}
		} else if (parts.Count() == 1) {
			if (ParseFloat(parts[0], x)) {
				dir = Vector(x, 0, 0);
				return true;
			}
		}
		return false;
	}

	static bool ParseFloat(string str, out float val) {
		str = str.Trim();
		if (str.IndexOf("<") != -1)
			str.Replace("<", "");
		if (str.IndexOf(">") != -1)
			str.Replace(">", "");
		if (str.IndexOf(" ") != -1)
			str.Replace(" ", "");
		float f = str.ToFloat();
		int start = (int) f;
		if (str.IndexOf("" + start) != 0 && str.IndexOf("-" + start) != 0)
			return false;
		val = f;
		return true;
	}

}