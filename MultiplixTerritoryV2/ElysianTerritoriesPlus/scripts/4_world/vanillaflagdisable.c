class Flag_Territory extends TerritoryFlag
{
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}
}