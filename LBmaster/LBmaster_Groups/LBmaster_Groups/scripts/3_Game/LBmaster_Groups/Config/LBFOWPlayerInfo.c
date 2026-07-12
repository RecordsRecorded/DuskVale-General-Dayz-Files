class LBFOWPlayerInfo : LBConfigLoaderPlayer<LBFOWPlayerInfo_> {

	override void InitVars() {
		InitVarsInternal("LBGroup/FOW", "[STEAMID].json", LBConfigType.DATA, true, "groups.change.fow"); // (do not edit)
	}

	override string GetRequiredVersion() {
		return "Advanced_Groups";
	}

	override bool KeepCached() {
		return true;
	}

}
// This file contains information about which sectors a player already discovered
class LBFOWPlayerInfo_ : LBConfigBasePlayer {

	ref array<int> fowSectorInfo = new array<int>(); // List of sectors in a binary format. Starting at sector 0,0 row wise and bit encoded
	int sectorCount; // Last sector count (only used to sync with client and overwritten on load)
	float sectorSize; // Last sector size based on the world size (only used to sync with client and overwritten on load)
	int discoveryRadius; // Last discovery Radius (only used to sync with client and overwritten on load)

	static ref LBFOWPlayerInfo_ CLIENT_RECEIVED_INFO = null;

	[NonSerialized()]
	bool changed = false;
	[NonSerialized()]
	ref array<ref Param2<int, int>> guaranteedOffsets = new array<ref Param2<int, int >> ();
	[NonSerialized()]
	ref array<ref Param2<int, int>> possibleOffsets = new array<ref Param2<int, int >> ();

	override void LoadDefault() {
		int total = sectorCount * sectorCount;
		int minArraySize = (total + 31) / 32;
		while (fowSectorInfo.Count() < minArraySize)
			fowSectorInfo.Insert(0);
	}

#ifdef SERVER
	override bool OnLoad() {
		sectorCount = LBFOWConfig.Get.sectorCount;
		discoveryRadius = LBFOWConfig.Get.discoveryRadius;
		float worldSize = g_Game.GetWorld().GetWorldSize();
		sectorSize = worldSize / sectorCount;
		LoadDefault();
		return false;
	}
#endif

	[LBTestManager.StartTest(ScriptCaller.Create(TestCalculateOffsets))]
	static void TestCalculateOffsets() {
		array<ref Param2<int, int>> guaranteed = new array<ref Param2<int, int >> ();
		array<ref Param2<int, int>> possible = new array<ref Param2<int, int >> ();
		CalculateOffsets(guaranteed, possible, 0.0, 150);
		LBTest<int>.Assert(guaranteed.Count(), 1);
		LBTest<int>.Assert(possible.Count(), 0);

		guaranteed.Clear();
		possible.Clear();
		CalculateOffsets(guaranteed, possible, 1.0, 150);
		LBTest<int>.Assert(guaranteed.Count(), 1);
		LBTest<int>.Assert(possible.Count(), 8);

		guaranteed.Clear();
		possible.Clear();
		CalculateOffsets(guaranteed, possible, 100.0, 150);
		LBTest<int>.Assert(guaranteed.Count(), 1);
		LBTest<int>.Assert(possible.Count(), 8);

		guaranteed.Clear();
		possible.Clear();
		CalculateOffsets(guaranteed, possible, 150.0, 150);
		LBTest<int>.Assert(guaranteed.Count(), 5);
		LBTest<int>.Assert(possible.Count(), 4);

		guaranteed.Clear();
		possible.Clear();
		CalculateOffsets(guaranteed, possible, 151.0, 150);
		LBTest<int>.Assert(guaranteed.Count(), 5);
		LBTest<int>.Assert(possible.Count(), 16);

		guaranteed.Clear();
		possible.Clear();
		CalculateOffsets(guaranteed, possible, 170.0, 150);
		LBTest<int>.Assert(guaranteed.Count(), 5);
		LBTest<int>.Assert(possible.Count(), 16);

		guaranteed.Clear();
		possible.Clear();
		CalculateOffsets(guaranteed, possible, 212.0, 150);
		LBTest<int>.Assert(guaranteed.Count(), 5);
		LBTest<int>.Assert(possible.Count(), 16);

		guaranteed.Clear();
		possible.Clear();
		CalculateOffsets(guaranteed, possible, 213.0, 150);
		LBTest<int>.Assert(guaranteed.Count(), 9);
		LBTest<int>.Assert(possible.Count(), 16);
	}

	static void CalculateOffsets(array<ref Param2<int, int>> guaranteed, array<ref Param2<int, int >> possible, float radius, float size) {
		int radiusSectors = Math.Ceil(radius / size);
		float r2 = radius * radius;
		float extra = size * 1.41421356;
		float r2Plus = (radius + extra) * (radius + extra);
		for (int dy = -radiusSectors; dy <= radiusSectors; ++dy) {
			for (int dx = -radiusSectors; dx <= radiusSectors; ++dx) {
				float sqMinX = dx * size;
				float sqMinY = dy * size;
				if ((sqMinX * sqMinX + sqMinY * sqMinY) <= r2) {
					guaranteed.Insert(new Param2<int, int>(dx, dy));
					LBLogger.Verbose("Guaranteed: " + dx + "," + dy, "AdvancedGroups");
				} else if ((sqMinX * sqMinX + sqMinY * sqMinY) <= r2Plus) {
					possible.Insert(new Param2<int, int>(dx, dy));
					LBLogger.Verbose("Possible: " + dx + "," + dy, "AdvancedGroups");
				}
			}
		}
		LBLogger.Debug("Calculated " + guaranteed.Count() + " Guaranteed and " + possible.Count() + " possible offsets for a sector size of " + size + " and a radius of " + radius + " testing " + (radiusSectors * 2 + 1) + " sectors per row", "AdvancedGroups");
	}

	override void OnReceivedFromRPC(PlayerIdentity sender) {
		changed = true;
		CLIENT_RECEIVED_INFO = this;

		guaranteedOffsets = new array<ref Param2<int, int>>();
		possibleOffsets = new array<ref Param2<int, int>>();
		CalculateOffsets(guaranteedOffsets, possibleOffsets, discoveryRadius, sectorSize);
	}

	bool RevealSectorsAt(vector pos) {
		int x = pos[0] / sectorSize;
		int y = pos[2] / sectorSize;
		array<ref Param2<int, int>> newRevealed = new array<ref Param2<int, int >> ();
		foreach (Param2<int, int> guaranteed : guaranteedOffsets) {
			if (RevealSector(x + guaranteed.param1, y + guaranteed.param2))
				newRevealed.Insert(new Param2<int, int>(x + guaranteed.param1, y + guaranteed.param2));
		}
		float xOff = x * sectorSize;
		float yOff = y * sectorSize;
		float r2 = discoveryRadius * discoveryRadius;
		foreach (Param2<int, int> possible : possibleOffsets) {
			float closestX = Math.Clamp(pos[0], possible.param1 * sectorSize + xOff, possible.param1 * sectorSize + sectorSize + xOff);
			float closestY = Math.Clamp(pos[2], possible.param2 * sectorSize + yOff, possible.param2 * sectorSize + sectorSize + yOff);
			float dx2 = pos[0] - closestX;
			float dy2 = pos[2] - closestY;
			float distSq = dx2 * dx2 + dy2 * dy2;

			if (distSq <= r2) {
				if (RevealSector(x + possible.param1, y + possible.param2))
					newRevealed.Insert(new Param2<int, int>(x + possible.param1, y + possible.param2));
			}
		}
		if (newRevealed.Count() > 0) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(newRevealed);
			rpc.Send(null, LBGroupRPCs.FOW_DISCOVERED, true);
			return true;
		}
		return false;
	}

	bool IsPositionRevealed(vector pos) {
		int x = pos[0] / sectorSize;
		int y = pos[2] / sectorSize;
		return IsSectorRevealed(x, y);
	}

	bool IsSectorRevealed(int x, int y) {
		if (x < 0 || y < 0 || x >= sectorCount || y >= sectorCount)
			return true;
		int fullIndex = y * sectorCount + x;
		int index = fullIndex >> 5;
		int bit = fullIndex & 0x1F;
		return (fowSectorInfo.Get(index) & (1 << bit)) != 0;
	}

	bool RevealSector(int x, int y) {
		if (x < 0 || y < 0 || x >= sectorCount || y >= sectorCount)
			return false;
		int fullIndex = y * sectorCount + x;
		int index = fullIndex >> 5;
		int bit = fullIndex & 0x1F;
		int before = fowSectorInfo.Get(index);
		int mask = 1 << bit;
		int after = before | mask;
		if (after == before)
			return false;
		fowSectorInfo.Set(index, after);
		changed = true;
		return true;
	}

}