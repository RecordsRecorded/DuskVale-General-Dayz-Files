class MultiTerritoryIdentityCacheData
{
	ref map<string, string> Names = new map<string, string>;
}

class MultiTerritoryIdentityCache
{
	protected static const string DIR_PATH = "$profile:Multiplix";
	protected static const string FILE_PATH = DIR_PATH + "\\TerritoryIdentityCache.json";
	protected static ref MultiTerritoryIdentityCacheData s_Data;
	protected static bool s_Loaded = false;

	protected static void EnsureLoaded()
	{
		if (s_Loaded)
		{
			if (!s_Data)
			{
				s_Data = new MultiTerritoryIdentityCacheData;
			}

			if (!s_Data.Names)
			{
				s_Data.Names = new map<string, string>;
			}

			return;
		}

		s_Data = new MultiTerritoryIdentityCacheData;

		if (GetGame() && GetGame().IsServer() && FileExist(FILE_PATH))
		{
			JsonFileLoader<MultiTerritoryIdentityCacheData>.JsonLoadFile(FILE_PATH, s_Data);
		}

		if (!s_Data.Names)
		{
			s_Data.Names = new map<string, string>;
		}

		s_Loaded = true;
	}

	protected static void Save()
	{
		if (!GetGame() || !GetGame().IsServer())
		{
			return;
		}

		EnsureLoaded();

		if (!FileExist(DIR_PATH))
		{
			MakeDirectory(DIR_PATH);
		}

		JsonFileLoader<MultiTerritoryIdentityCacheData>.JsonSaveFile(FILE_PATH, s_Data);
	}

	static void RememberName(string guid, string displayName)
	{
		if (!guid || guid == "" || !displayName || displayName == "")
		{
			return;
		}

		EnsureLoaded();

		if (s_Data.Names.Contains(guid) && s_Data.Names.Get(guid) == displayName)
		{
			return;
		}

		s_Data.Names.Set(guid, displayName);
		Save();
	}

	static string GetName(string guid)
	{
		if (!guid || guid == "")
		{
			return "";
		}

		EnsureLoaded();

		if (s_Data.Names.Contains(guid))
		{
			return s_Data.Names.Get(guid);
		}

		return "";
	}

	static void BuildNameArrays(TStringArray guids, out TStringArray outGuids, out TStringArray outNames)
	{
		EnsureLoaded();

		outGuids = new TStringArray;
		outNames = new TStringArray;

		if (!guids)
		{
			return;
		}

		for (int i = 0; i < guids.Count(); i++)
		{
			string guid = guids.Get(i);
			string name = GetName(guid);
			if (name == "")
			{
				continue;
			}

			outGuids.Insert(guid);
			outNames.Insert(name);
		}
	}

	static void ReceiveNameArrays(TStringArray guids, TStringArray names)
	{
		EnsureLoaded();

		if (!guids || !names)
		{
			return;
		}

		int count = Math.Min(guids.Count(), names.Count());
		for (int i = 0; i < count; i++)
		{
			string guid = guids.Get(i);
			string name = names.Get(i);
			if (!guid || guid == "" || !name || name == "")
			{
				continue;
			}

			s_Data.Names.Set(guid, name);
		}
	}
}
