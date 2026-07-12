// Bitmask of player symptoms we display in the menu's ALRT row. Server reads
// SymptomManager/PlayerStats and packs the active flags; client just unpacks.
enum MULTITERRITORY_VITAL_FLAGS
{
	VFLAG_BLEED      = 1,        // bleeding source count > 0
	VFLAG_PAIN       = 2,        // SYMPTOM_PAIN_LIGHT
	VFLAG_PAIN_HEAVY = 4,        // SYMPTOM_PAIN_HEAVY
	VFLAG_COUGH      = 8,        // SYMPTOM_COUGH
	VFLAG_VOMIT      = 16,
	VFLAG_FEVER      = 32,       // SYMPTOM_FEVERBLUR
	VFLAG_COLD       = 64,       // SYMPTOM_FREEZE or _RATTLE
	VFLAG_HOT        = 128,
	VFLAG_BLIND      = 256,
	VFLAG_UNCNS      = 512,      // SYMPTOM_UNCONSCIOUS
	VFLAG_INFCT      = 1024,     // SYMPTOM_HMP_SEVERE
	VFLAG_WET        = 2048,     // WET stat > 0
	VFLAG_DEAF       = 4096,
	VFLAG_SHAKE      = 8192      // SYMPTOM_HAND_SHIVER
}

class MultiplixTerritoryMenuState
{
	protected static Object s_TargetComputer;

	// Vitals cache populated by the MENU_VITALS_RESPONSE RPC handler.
	// Values are percentages 0-100; -1 means "not yet received".
	// Temperature is mapped from HEATCOMFORT (-1..+1) to 0..100 where 50 = neutral.
	static int s_VitalHpPct       = -1;
	static int s_VitalBlPct       = -1;
	static int s_VitalTmpPct      = -1;
	static int s_VitalWaPct       = -1;
	static int s_VitalEnPct       = -1;
	static int s_VitalSymptomBits = 0;
	static float s_VitalLastUpdate = 0; // GetGame().GetTime() of last receipt, for staleness UI

	static void OpenComputerMenu(Object computer)
	{
		if (!computer)
		{
			return;
		}

		s_TargetComputer = computer;
		// Wipe stale cache from a previous menu session.
		s_VitalHpPct       = -1;
		s_VitalBlPct       = -1;
		s_VitalTmpPct      = -1;
		s_VitalWaPct       = -1;
		s_VitalEnPct       = -1;
		s_VitalSymptomBits = 0;
		s_VitalLastUpdate  = 0;
		GetGame().GetUIManager().EnterScriptedMenu(MENU_MULTITERRITORY_COMPUTER, null);
	}

	static Object GetTargetComputer()
	{
		return s_TargetComputer;
	}

	static void Clear()
	{
		s_TargetComputer = null;
	}
}
