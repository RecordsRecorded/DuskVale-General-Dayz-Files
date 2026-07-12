// DuskVale Live Market Editor - server side.
//
// Applies edits to the in-memory Expansion Market data, saves the JSON files
// in the profile Market/ folder (and mission traderzones/ for stock), and
// broadcasts patches so connected clients see new prices without a reconnect
// (Expansion only re-syncs stock after the first open of a trader; prices are
// cached client-side for the session, so we patch the cached items directly).
[RegisterLBRPCHandler(LBConverter.TypenameToString(DVME_ServerRPCHandler), LBRPCHandlerType.SERVER)]
class DVME_ServerRPCHandler : LB_RPCHandler
{
	void DVME_ServerRPCHandler()
	{
		RegisterRPC(DVMERPCs.REQUEST_CATEGORIES, ScriptCaller.Create(HandleRequestCategories));
		RegisterRPC(DVMERPCs.REQUEST_ITEMS, ScriptCaller.Create(HandleRequestItems));
		RegisterRPC(DVMERPCs.APPLY_ITEM, ScriptCaller.Create(HandleApplyItem));
		RegisterRPC(DVMERPCs.SET_STOCK, ScriptCaller.Create(HandleSetStock));
		RegisterRPC(DVMERPCs.RELOAD_CATEGORY, ScriptCaller.Create(HandleReloadCategory));
	}

	override string GetNotificationTitle()
	{
		return "Market Editor";
	}

	protected bool CheckAccess()
	{
		if (!HasPermission(DVMEConst.PERMISSION))
		{
			SendErrorNotification("You do not have the '" + DVMEConst.PERMISSION + "' permission.");
			return false;
		}

		return true;
	}

	protected void NotifyPage(string message)
	{
		rpc.Reset();
		rpc.Write(message);
		Send(DVMERPCs.NOTIFY_RESULT);
	}

	// The live trader zone list inside ExpansionMarketSettings is protected and
	// the class cannot be modded (its own Load() passes 'this' to JsonFileLoader,
	// which does not compile against a modded type). So we enumerate the zone
	// files on disk and resolve each to its live instance by position.
	protected array<ExpansionMarketTraderZone> GetLiveZones()
	{
		array<ExpansionMarketTraderZone> zones = new array<ExpansionMarketTraderZone>;
		ExpansionMarketSettings market = GetExpansionSettings().GetMarket();

		array<string> files = ExpansionStatic.FindFilesInLocation(EXPANSION_TRADER_ZONES_FOLDER, ".json", true);
		foreach (string fileName : files)
		{
			fileName = fileName.Substring(0, fileName.Length() - 5);

			ExpansionMarketTraderZone fileZone = ExpansionMarketTraderZone.Load(fileName);
			if (!fileZone)
			{
				continue;
			}

			ExpansionMarketTraderZone liveZone = market.GetTraderZoneByPosition(fileZone.Position);
			if (liveZone && zones.Find(liveZone) == -1)
			{
				zones.Insert(liveZone);
			}
		}

		return zones;
	}

	void HandleRequestCategories()
	{
		if (!CheckAccess())
		{
			return;
		}

		ExpansionMarketSettings market = GetExpansionSettings().GetMarket();
		TStringArray fileNames = new TStringArray;
		TStringArray displayNames = new TStringArray;

		map<int, ref ExpansionMarketCategory> categories = market.GetCategories();
		foreach (int id, ExpansionMarketCategory category : categories)
		{
			if (!category)
			{
				continue;
			}

			fileNames.Insert(category.m_FileName);
			displayNames.Insert(category.DisplayName);
		}

		rpc.Write(fileNames);
		rpc.Write(displayNames);
		Send(DVMERPCs.SEND_CATEGORIES);
	}

	void HandleRequestItems()
	{
		if (!CheckAccess())
		{
			return;
		}

		string fileName;
		if (!ctx.Read(fileName))
		{
			return;
		}

		ExpansionMarketSettings market = GetExpansionSettings().GetMarket();
		ExpansionMarketCategory category = market.GetCategory(fileName);
		if (!category)
		{
			NotifyPage("Category '" + fileName + "' not found.");
			return;
		}

		array<ExpansionMarketTraderZone> zones = GetLiveZones();
		ExpansionMarketTraderZone zone;
		if (zones.Count() > 0)
		{
			zone = zones.Get(0);
		}

		rpc.Write(fileName);
		rpc.Write(category.Items.Count());

		foreach (ExpansionMarketItem item : category.Items)
		{
			int currentStock = -1;
			if (zone && zone.ItemExists(item.ClassName))
			{
				currentStock = zone.GetStock(item.ClassName, true);
			}

			rpc.Write(item.ClassName);
			rpc.Write(item.MinPriceThreshold);
			rpc.Write(item.MaxPriceThreshold);
			rpc.Write(item.SellPricePercent);
			rpc.Write(item.MinStockThreshold);
			rpc.Write(item.MaxStockThreshold);
			rpc.Write(item.QuantityPercent);
			rpc.Write(currentStock);
		}

		Send(DVMERPCs.SEND_ITEMS);
	}

	protected bool ApplyValuesToItem(ExpansionMarketItem item, int minPrice, int maxPrice, float sellPercent, int minStock, int maxStock, int quantityPercent)
	{
		if (!item)
		{
			return false;
		}

		item.MinPriceThreshold = minPrice;
		item.MaxPriceThreshold = maxPrice;
		item.SellPricePercent = sellPercent;
		item.MinStockThreshold = minStock;
		item.MaxStockThreshold = maxStock;
		item.QuantityPercent = quantityPercent;

		// Re-runs the same clamping + bfloat16 sell percent packing Expansion
		// does on load, so netsync uses exactly these values.
		item.SanityCheckAndRepair();

		return true;
	}

	protected void BroadcastItemPatch(ExpansionMarketItem item)
	{
		rpc.Reset();
		rpc.Write(item.ClassName);
		rpc.Write(item.MinPriceThreshold);
		rpc.Write(item.MaxPriceThreshold);
		rpc.Write(item.SellPricePercent);
		rpc.Write(item.MinStockThreshold);
		rpc.Write(item.MaxStockThreshold);
		rpc.Write(item.QuantityPercent);
		SendAll(DVMERPCs.APPLY_ITEM);
	}

	void HandleApplyItem()
	{
		if (!CheckAccess())
		{
			return;
		}

		string fileName;
		string className;
		int minPrice;
		int maxPrice;
		float sellPercent;
		int minStock;
		int maxStock;
		int quantityPercent;

		if (!ctx.Read(fileName) || !ctx.Read(className) || !ctx.Read(minPrice) || !ctx.Read(maxPrice) || !ctx.Read(sellPercent) || !ctx.Read(minStock) || !ctx.Read(maxStock) || !ctx.Read(quantityPercent))
		{
			return;
		}

		className.ToLower();

		ExpansionMarketItem item = ExpansionMarketCategory.GetGlobalItem(className);
		if (!item)
		{
			NotifyPage("Item '" + className + "' not found in market data.");
			return;
		}

		ApplyValuesToItem(item, minPrice, maxPrice, sellPercent, minStock, maxStock, quantityPercent);

		ExpansionMarketSettings market = GetExpansionSettings().GetMarket();
		ExpansionMarketCategory category = market.GetCategory(fileName);
		if (category)
		{
			category.Save();
		}

		BroadcastItemPatch(item);
		LBLogger.Info("[DVME] " + GetSenderLog() + " edited market item " + className + " in " + fileName + " (buy " + item.MinPriceThreshold + "-" + item.MaxPriceThreshold + ", sell% " + item.SellPricePercent + ", stock " + item.MinStockThreshold + "-" + item.MaxStockThreshold + ")", "AdminTools");
		NotifyPage("Saved '" + className + "' and synced all clients.");
	}

	void HandleSetStock()
	{
		if (!CheckAccess())
		{
			return;
		}

		string className;
		int stock;
		if (!ctx.Read(className) || !ctx.Read(stock))
		{
			return;
		}

		className.ToLower();

		array<ExpansionMarketTraderZone> zones = GetLiveZones();
		if (zones.Count() == 0)
		{
			NotifyPage("No trader zones loaded.");
			return;
		}

		int updated = 0;
		foreach (ExpansionMarketTraderZone zone : zones)
		{
			if (!zone || !zone.ItemExists(className))
			{
				continue;
			}

			zone.SetStock(className, stock);
			zone.Save();
			updated++;
		}

		LBLogger.Info("[DVME] " + GetSenderLog() + " set stock of " + className + " to " + stock + " in " + updated + " zone(s)", "AdminTools");
		NotifyPage("Stock of '" + className + "' set to " + stock + " in " + updated + " zone(s). Clients see it on next menu open.");
	}

	void HandleReloadCategory()
	{
		if (!CheckAccess())
		{
			return;
		}

		string fileName;
		if (!ctx.Read(fileName))
		{
			return;
		}

		ExpansionMarketCategory fresh = ExpansionMarketCategory.Load(fileName);
		if (!fresh)
		{
			NotifyPage("Could not read '" + fileName + ".json' from the Market folder.");
			return;
		}

		int updated = 0;
		int missing = 0;

		foreach (ExpansionMarketItem freshItem : fresh.Items)
		{
			string className = freshItem.ClassName;
			className.ToLower();

			ExpansionMarketItem item = ExpansionMarketCategory.GetGlobalItem(className);
			if (!item)
			{
				// Item exists in the file but not in memory - needs a restart to add
				missing++;
				continue;
			}

			ApplyValuesToItem(item, freshItem.MinPriceThreshold, freshItem.MaxPriceThreshold, freshItem.SellPricePercent, freshItem.MinStockThreshold, freshItem.MaxStockThreshold, freshItem.QuantityPercent);
			BroadcastItemPatch(item);
			updated++;
		}

		LBLogger.Info("[DVME] " + GetSenderLog() + " reloaded market category " + fileName + " from disk (" + updated + " items updated, " + missing + " new items skipped)", "AdminTools");

		string message = "Reloaded '" + fileName + "' from disk: " + updated + " items updated live.";
		if (missing > 0)
		{
			message += " " + missing + " item(s) are new in the file and need a server restart.";
		}

		NotifyPage(message);
	}
}
