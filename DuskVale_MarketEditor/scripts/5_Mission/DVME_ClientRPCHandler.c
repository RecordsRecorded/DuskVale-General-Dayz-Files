// DuskVale Live Market Editor - client side state + RPC handler.

class DVMEItemRowClient
{
	string ClassName;
	int MinPriceThreshold;
	int MaxPriceThreshold;
	float SellPricePercent;
	int MinStockThreshold;
	int MaxStockThreshold;
	int QuantityPercent;
	int CurrentStock;
}

// Static state the admin page polls each frame
class DVMEClientState
{
	static ref TStringArray CategoryFiles = new TStringArray;
	static ref TStringArray CategoryNames = new TStringArray;
	static ref array<ref DVMEItemRowClient> Items = new array<ref DVMEItemRowClient>;
	static string ItemsCategory = "";
	static string Status = "";
	static int Revision = 0; // bumped on every change so the page knows to refresh

	static void Touch()
	{
		Revision++;
	}
}

[RegisterLBRPCHandler(LBConverter.TypenameToString(DVME_ClientRPCHandler), LBRPCHandlerType.CLIENT)]
class DVME_ClientRPCHandler : LB_RPCHandler
{
	void DVME_ClientRPCHandler()
	{
		RegisterRPC(DVMERPCs.SEND_CATEGORIES, ScriptCaller.Create(HandleSendCategories));
		RegisterRPC(DVMERPCs.SEND_ITEMS, ScriptCaller.Create(HandleSendItems));
		RegisterRPC(DVMERPCs.APPLY_ITEM, ScriptCaller.Create(HandleItemPatch));
		RegisterRPC(DVMERPCs.NOTIFY_RESULT, ScriptCaller.Create(HandleNotify));
	}

	override string GetNotificationTitle()
	{
		return "Market Editor";
	}

	void HandleSendCategories()
	{
		TStringArray files = new TStringArray;
		TStringArray names = new TStringArray;
		if (!ctx.Read(files) || !ctx.Read(names))
		{
			return;
		}

		DVMEClientState.CategoryFiles = files;
		DVMEClientState.CategoryNames = names;
		DVMEClientState.Touch();
	}

	void HandleSendItems()
	{
		string fileName;
		int count;
		if (!ctx.Read(fileName) || !ctx.Read(count))
		{
			return;
		}

		array<ref DVMEItemRowClient> items = new array<ref DVMEItemRowClient>;
		for (int i = 0; i < count; i++)
		{
			DVMEItemRowClient row = new DVMEItemRowClient;
			if (!ctx.Read(row.ClassName) || !ctx.Read(row.MinPriceThreshold) || !ctx.Read(row.MaxPriceThreshold) || !ctx.Read(row.SellPricePercent) || !ctx.Read(row.MinStockThreshold) || !ctx.Read(row.MaxStockThreshold) || !ctx.Read(row.QuantityPercent) || !ctx.Read(row.CurrentStock))
			{
				return;
			}

			items.Insert(row);
		}

		DVMEClientState.ItemsCategory = fileName;
		DVMEClientState.Items = items;
		DVMEClientState.Touch();
	}

	// Server broadcast after an edit: update the locally cached Expansion
	// market item so open/next menus show the new prices without reconnect.
	void HandleItemPatch()
	{
		string className;
		int minPrice;
		int maxPrice;
		float sellPercent;
		int minStock;
		int maxStock;
		int quantityPercent;

		if (!ctx.Read(className) || !ctx.Read(minPrice) || !ctx.Read(maxPrice) || !ctx.Read(sellPercent) || !ctx.Read(minStock) || !ctx.Read(maxStock) || !ctx.Read(quantityPercent))
		{
			return;
		}

		className.ToLower();

		ExpansionMarketItem item = ExpansionMarketCategory.GetGlobalItem(className);
		if (item)
		{
			item.MinPriceThreshold = minPrice;
			item.MaxPriceThreshold = maxPrice;
			item.SellPricePercent = sellPercent;
			item.MinStockThreshold = minStock;
			item.MaxStockThreshold = maxStock;
			item.QuantityPercent = quantityPercent;
			item.SanityCheckAndRepair();
		}

		// Keep the editor page rows in sync too
		foreach (DVMEItemRowClient row : DVMEClientState.Items)
		{
			string rowName = row.ClassName;
			rowName.ToLower();
			if (rowName == className)
			{
				row.MinPriceThreshold = minPrice;
				row.MaxPriceThreshold = maxPrice;
				row.SellPricePercent = sellPercent;
				row.MinStockThreshold = minStock;
				row.MaxStockThreshold = maxStock;
				row.QuantityPercent = quantityPercent;
				DVMEClientState.Touch();
				break;
			}
		}
	}

	void HandleNotify()
	{
		string message;
		if (!ctx.Read(message))
		{
			return;
		}

		DVMEClientState.Status = message;
		DVMEClientState.Touch();
	}
}
