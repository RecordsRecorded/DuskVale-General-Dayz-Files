// DuskVale Live Market Editor - shared definitions.

enum DVMERPCs
{
	REQUEST_CATEGORIES = 758122400, // client -> server: request market category file list
	SEND_CATEGORIES,                // server -> client: category names
	REQUEST_ITEMS,                  // client -> server: request items of one category
	SEND_ITEMS,                     // server -> client: item rows for one category
	APPLY_ITEM,                     // client -> server: apply edited values for one item (+ save + resync)
	RELOAD_CATEGORY,                // client -> server: re-read one category file from disk
	SET_STOCK,                      // client -> server: set current stock of an item in all trader zones
	NOTIFY_RESULT                   // server -> client: status message for the page
}

class DVMEConst
{
	// LBmaster admin permission required to see and use the editor page.
	// Add it to your admin permission group in LBmaster's Admins config
	// ("*" or "duskvale.*" also works).
	static const string PERMISSION = "duskvale.marketeditor";
}

// One editable market item row sent between server and client
class DVMEItemRow
{
	string ClassName;
	int MinPriceThreshold;
	int MaxPriceThreshold;
	float SellPricePercent;
	int MinStockThreshold;
	int MaxStockThreshold;
	int QuantityPercent;
	int CurrentStock; // stock in the first trader zone (-1 = no zone/static stock)
}
