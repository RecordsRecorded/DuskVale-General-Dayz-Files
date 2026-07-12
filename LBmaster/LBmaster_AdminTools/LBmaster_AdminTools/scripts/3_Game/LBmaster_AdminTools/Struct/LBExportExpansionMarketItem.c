class LBExportExpansionMarketItem {

	string ClassName;
	int MaxPriceThreshold = 100;
	int MinPriceThreshold = 100;
	float SellPricePercent = -1.0;
	int MaxStockThreshold = 1;
	int MinStockThreshold = 1;
	int QuantityPercent = -1;
	ref TStringArray SpawnAttachments = new TStringArray();
	ref TStringArray Variants = new TStringArray();

	void LBExportExpansionMarketItem(string classname_) {
		this.ClassName = classname_;
	}

}