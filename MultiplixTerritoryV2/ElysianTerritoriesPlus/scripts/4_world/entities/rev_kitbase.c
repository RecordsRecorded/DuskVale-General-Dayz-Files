class REV_KitBase: WoodenCrate 
{
    override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation );

		if (!GetGame().IsServer())
		{
			return;
		}

		string furnitureKitName = this.ClassName();
		int kitNameLength = furnitureKitName.Length();
		int furnitureNameLength = kitNameLength - 3; // "kit".Length == 3
		string furnitureName = furnitureKitName.Substring(0,furnitureNameLength);
		auto furniture = GetGame().CreateObject(furnitureName, position, false);

		if (furniture)
		{
			furniture.SetOrientation(orientation);
			this.Delete();
		}
	}
};

class REV_TerritoryComputerKit: REV_KitBase
{
	override string GetSourceTextureName()
    {
        return "ElysianTerritoriesPlus\\Models\\Kit\\data\\TerritoryComputerKit_co.paa";
    }
};
