// Expose the protected trader zone list so the editor can update stock in
// every zone. Modded class only adds an accessor, no behavior changes.
modded class ExpansionMarketSettings
{
	array<ref ExpansionMarketTraderZone> DVME_GetTraderZones()
	{
		return m_TraderZones;
	}
}
