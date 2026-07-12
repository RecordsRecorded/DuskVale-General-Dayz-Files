modded class BaseBuildingBase extends ItemBase
{
	protected void ApplyMultiTerritoriesBaseDamageSetting()
	{
		if (!GetGame() || !GetGame().IsServer())
		{
			return;
		}

		if (GetMultiTerritoriesConfig().disableBaseDamage() > 0)
		{
			SetAllowDamage(false);
		}
	}

	override void EEInit()
	{
		super.EEInit();
		ApplyMultiTerritoriesBaseDamageSetting();
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		ApplyMultiTerritoriesBaseDamageSetting();
	}
}
