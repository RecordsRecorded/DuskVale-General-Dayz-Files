class REV_ActionDismantleObjectCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class REV_ActionDismantleObject: ActionContinuousBase
{
	void REV_ActionDismantleObject()
	{
		m_CallbackClass = REV_ActionDismantleObjectCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Dismantle";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{	
		if (!item)
			return false;

		string itemType = item.GetType();
		if (itemType != "Screwdriver")
			return false;

		auto targetObject = target.GetObject();
		if (!targetObject)
			return false;

		if (targetObject.IsInherited(REV_StaticBase))
		{
			if (!HasDismantlePermission(player, targetObject))
				return false;

			auto furniture = REV_StaticBase.Cast(targetObject);
			auto furnitureInventory = furniture.GetInventory();
			
			auto hasNoAttachments = furnitureInventory.AttachmentCount() == 0;
			auto cargo = furnitureInventory.GetCargo();
			auto hasNoItemsInCargo = true;

			if (cargo)
				hasNoItemsInCargo = cargo.GetItemCount() == 0;
			
			return hasNoAttachments && hasNoItemsInCargo;
		}

		if (targetObject.IsInherited(TerritoryFlag))
		{
			if (!HasDismantlePermission(player, targetObject))
				return false;

			auto territoryFlag = TerritoryFlag.Cast(targetObject);
			if (territoryFlag.IsInherited(REV_TerritoryComputer))
			{
				auto computer = REV_TerritoryComputer.Cast(targetObject);
				return computer.CanDismantle();
			}
		}
       
		return false;
	}

	protected bool HasDismantlePermission(PlayerBase player, Object targetObject)
	{
		if (!player || !targetObject || !player.GetIdentity())
		{
			return false;
		}

		return TerritoryFlag.HasTerritoryPermAtPos(player.GetIdentity().GetId(), TerritoryPerm.DISMANTLE, targetObject.GetPosition());
	}

	override void OnFinishProgressServer( ActionData action_data ) 
	{
		auto targetObject = action_data.m_Target.GetObject();
		if (!targetObject || !HasDismantlePermission(action_data.m_Player, targetObject))
		{
			return;
		}

		string furnitureName = targetObject.ClassName();
        vector positionToSpawn = action_data.m_Player.GetPosition();
        targetObject.Delete();
        
        if (furnitureName == "REV_TerritoryComputer")
        {
            GetGame().CreateObject("REV_TerritoryComputerKit", positionToSpawn);
        }
        else
        {
            string furnitureKitNameToSpawn = furnitureName + "Kit";
            GetGame().CreateObject(furnitureKitNameToSpawn, positionToSpawn);
        }
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
};
