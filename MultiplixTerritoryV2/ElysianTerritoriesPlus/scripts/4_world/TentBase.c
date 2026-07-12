modded class TentBase extends ItemBase
{
	override void EEOnCECreate() 
	{
		super.EEOnCECreate();
		this.SetLifetime(GetMultiTerritoriesConfig().TentCESpawnLifeTime);
	}
	
	void ResetTentLifeTime()
	{
		float OldLifeTime = GetLifetime();
		float MaxLifetime = GetLifetimeMax();
		this.SetLifetime(MaxLifetime);
		float NewLifeTime = GetLifetime();
		GetCEApi().RadiusLifetimeReset(GetPosition(), GetMultiTerritoriesConfig().TentRadius);
	}
	
	override void ToggleAnimation(string selection)
	{
		super.ToggleAnimation(selection);
		if (GetGame().IsServer() && GetMultiTerritoriesConfig().TentRadius >= 0)
		{
			ResetTentLifeTime();
		}
	}
};