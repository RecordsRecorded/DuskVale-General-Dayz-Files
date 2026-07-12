modded class Hologram
{
	override EntityAI PlaceEntity( EntityAI entity_for_placing )
	{
		if (entity_for_placing.IsInherited(REV_KitBase))
		{
			return entity_for_placing;
		}
		return super.PlaceEntity(entity_for_placing);
	}
};

class REV_TerritoryComputer_Hologram : ItemBase 
{
	const string HOLOGRAM_COLOR = "#(argb,8,8,3)color(1,1,1,0.5,co)";
	const string HOLOGRAM_RVMAT = "ElysianTerritoriesPlus\\Models\\Computer\\data\\TerritoryComputer_Placing.rvmat";

	void REV_TerritoryComputer_Hologram()
	{
		SetObjectTexture(0, HOLOGRAM_COLOR);
		SetSynchDirty();
	}
};