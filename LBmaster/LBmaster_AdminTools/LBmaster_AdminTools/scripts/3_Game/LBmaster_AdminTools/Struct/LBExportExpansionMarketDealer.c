class LBExportExpansionMarketDealer {

	int m_Version = 12;
	string DisplayName;
	string Icon = "Deliver";
	string Color = "FBFCFEFF";
	bool IsExchange = false;
	float InitStockPercent = 75.0;
	ref array<ref LBExportExpansionMarketItem> Items = new array<ref LBExportExpansionMarketItem>();

	void LBExportExpansionMarketDealer(string name) {
		this.DisplayName = name;
	}

	void AddItem(string classname) {
		Items.Insert(new LBExportExpansionMarketItem(classname));
	}

}