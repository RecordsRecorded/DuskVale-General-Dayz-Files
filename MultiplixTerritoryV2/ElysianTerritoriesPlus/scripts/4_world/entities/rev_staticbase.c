class REV_StaticBase : DeployableContainer_Base
{
	void REV_FurnitureBase()
	{
		ProcessInvulnerabilityCheck(GetInvulnerabilityTypeString());
	}
	
	override string GetInvulnerabilityTypeString()
	{
		return "disableContainerDamage";
	}

	override void SetActions()
    {
        super.SetActions();
		AddAction(REV_ActionDismantleObject);
    }
	
	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
};