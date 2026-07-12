class ActionInsertFloppy: ActionInteractBase
{	
	protected float FLAG_STEP_INSTANT = 1.0;

	void ActionInsertFloppy()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_Text = "Insert Floppydisk";
	}

	protected bool CanInsertFloppy(REV_TerritoryComputer computer)
	{
		if (!computer)
		{
			return false;
		}

		Territory_Floppydisk floppy = Territory_Floppydisk.Cast(computer.FindAttachmentBySlotName("Material_FPole_Flag"));
		return floppy && !floppy.IsLockedInSlot() && !computer.IsFloppyLocked() && computer.HasValidFloppy();
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (super.ActionCondition(player, target, item))
		{
			REV_TerritoryComputer computer = REV_TerritoryComputer.Cast(target.GetObject());
			PlayerBase thePlayer = PlayerBase.Cast(player);

			if (computer && thePlayer && thePlayer.GetIdentity())
			{
				computer.SyncTerritoryRateLimited();
				if (CanInsertFloppy(computer) && vector.Distance(computer.GetPosition(), thePlayer.GetPosition()) < UAMisc.MAX_DISTANCE_FROM_FLAG)
				{
					return true;
				}
			}
		}

		return false;
	}
	
	override void OnExecuteClient(ActionData action_data)
	{
		if (!action_data || !action_data.m_Target)
		{
			return;
		}

		REV_TerritoryComputer target = REV_TerritoryComputer.Cast( action_data.m_Target.GetObject() );
		bool wasTurnedOn = target && target.IsTurnedOn();

		super.OnExecuteClient(action_data);

		if (target)
		{
			target.ClientPreviewPowerOn();
			target.ClientPreviewFloppyInserted(wasTurnedOn);
			target.PlayInsertFloppySound();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(target.ClientPlayPowerOnSoundsFromFloppyInsert, 250, false);
		}
	}
	override void OnExecute(ActionData action_data)
	{
		REV_TerritoryComputer target = REV_TerritoryComputer.Cast( action_data.m_Target.GetObject() );
		bool shouldInsert = CanInsertFloppy(target);

		super.OnExecute(action_data);

		if (target && shouldInsert)
		{
			target.LockFloppydiskFromAction();
			target.ForceProtectionRefresh(false);
		}
	}
};
