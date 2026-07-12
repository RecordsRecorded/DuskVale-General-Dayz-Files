#ifdef DZ_Expansion_Market
modded class ExpansionTraderNPCBase {

	string GetLBMarkerName() {
		ExpansionTraderObjectBase obj = GetTraderObject();
		if (!obj)
			return "";
		ExpansionMarketTrader market = obj.GetTraderMarket();
		if (!market)
			return "";
		return market.DisplayName;
	}

	override bool AddAutoMarker() {
		return true;
	}

}
#endif