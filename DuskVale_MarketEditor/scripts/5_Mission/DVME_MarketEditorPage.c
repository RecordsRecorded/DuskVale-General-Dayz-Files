// DuskVale Live Market Editor - LBmaster admin menu page.
//
// Appears in the LBmaster (i) admin menu (AdminTools category) for admins with
// the "duskvale.marketeditor" permission. Lists Expansion Market categories
// and items, and lets you edit buy price range, sell percent, stock thresholds
// and current zone stock. Changes apply live and are written to the JSON files.
[DayZGame.RegisterLBAdminPage("DVMEMarketEditorPage", "LBmaster_AdminTools/logo/logo.paa", "Market Editor", "duskvale.marketeditor")]
class DVMEMarketEditorPage : LBAdmin_Menu_Page
{
	TextListboxWidget list_categories, list_items;
	EditBoxWidget edit_search, edit_min_price, edit_max_price, edit_sell_percent, edit_min_stock, edit_max_stock, edit_quantity_percent, edit_stock;
	ButtonWidget btn_refresh, btn_apply, btn_set_stock, btn_reload_cat;
	TextWidget txt_item_name, txt_status;

	protected int m_LastRevision = -1;
	protected string m_SelectedCategory = "";
	protected string m_SelectedClass = "";
	protected ref TIntArray m_FilteredToItemIndex = new TIntArray;

	override string GetLayoutName()
	{
		return "AdminMenu_DVMEMarketEditorPage";
	}

	override void OnPageShown()
	{
		super.OnPageShown();

		m_LastRevision = -1;
		RequestCategories();
	}

	protected void RequestCategories()
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(true);
		rpc.Send(null, DVMERPCs.REQUEST_CATEGORIES, true);
		SetStatus("Requesting categories from server...");
	}

	protected void RequestItems(string fileName)
	{
		m_SelectedCategory = fileName;
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(fileName);
		rpc.Send(null, DVMERPCs.REQUEST_ITEMS, true);
		SetStatus("Requesting items of '" + fileName + "'...");
	}

	protected void SetStatus(string text)
	{
		if (txt_status)
		{
			txt_status.SetText(text);
		}
	}

	override void OnUpdateFrame()
	{
		if (m_LastRevision != DVMEClientState.Revision)
		{
			m_LastRevision = DVMEClientState.Revision;
			PopulateCategories();
			PopulateItems();
			SetStatus(DVMEClientState.Status);
		}
	}

	protected void PopulateCategories()
	{
		if (!list_categories)
		{
			return;
		}

		list_categories.ClearItems();
		for (int i = 0; i < DVMEClientState.CategoryFiles.Count(); i++)
		{
			string label = DVMEClientState.CategoryFiles.Get(i);
			if (i < DVMEClientState.CategoryNames.Count() && DVMEClientState.CategoryNames.Get(i) != "")
			{
				label += "  (" + DVMEClientState.CategoryNames.Get(i) + ")";
			}

			list_categories.AddItem(label, null, 0);
		}
	}

	protected void PopulateItems()
	{
		if (!list_items)
		{
			return;
		}

		string filter = "";
		if (edit_search)
		{
			filter = edit_search.GetText();
			filter.ToLower();
		}

		list_items.ClearItems();
		m_FilteredToItemIndex.Clear();

		for (int i = 0; i < DVMEClientState.Items.Count(); i++)
		{
			DVMEItemRowClient row = DVMEClientState.Items.Get(i);
			string nameLower = row.ClassName;
			nameLower.ToLower();

			if (filter != "" && !nameLower.Contains(filter))
			{
				continue;
			}

			string stockText = row.CurrentStock.ToString();
			if (row.CurrentStock < 0)
			{
				stockText = "-";
			}

			string label = row.ClassName + "  |  " + row.MinPriceThreshold + "-" + row.MaxPriceThreshold + "  |  " + row.SellPricePercent + "%  |  " + row.MinStockThreshold + "-" + row.MaxStockThreshold + " (" + stockText + ")";
			list_items.AddItem(label, null, 0);
			m_FilteredToItemIndex.Insert(i);
		}
	}

	protected DVMEItemRowClient GetSelectedItemRow()
	{
		if (!list_items)
		{
			return null;
		}

		int row = list_items.GetSelectedRow();
		if (row < 0 || row >= m_FilteredToItemIndex.Count())
		{
			return null;
		}

		int index = m_FilteredToItemIndex.Get(row);
		if (index < 0 || index >= DVMEClientState.Items.Count())
		{
			return null;
		}

		return DVMEClientState.Items.Get(index);
	}

	protected void LoadSelectionIntoEditor()
	{
		DVMEItemRowClient row = GetSelectedItemRow();
		if (!row)
		{
			return;
		}

		m_SelectedClass = row.ClassName;

		if (txt_item_name) txt_item_name.SetText(row.ClassName);
		if (edit_min_price) edit_min_price.SetText(row.MinPriceThreshold.ToString());
		if (edit_max_price) edit_max_price.SetText(row.MaxPriceThreshold.ToString());
		if (edit_sell_percent) edit_sell_percent.SetText(row.SellPricePercent.ToString());
		if (edit_min_stock) edit_min_stock.SetText(row.MinStockThreshold.ToString());
		if (edit_max_stock) edit_max_stock.SetText(row.MaxStockThreshold.ToString());
		if (edit_quantity_percent) edit_quantity_percent.SetText(row.QuantityPercent.ToString());
		if (edit_stock)
		{
			if (row.CurrentStock >= 0)
			{
				edit_stock.SetText(row.CurrentStock.ToString());
			}
			else
			{
				edit_stock.SetText("");
			}
		}
	}

	protected void SendApply()
	{
		if (m_SelectedCategory == "" || m_SelectedClass == "")
		{
			SetStatus("Select a category and an item first.");
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(m_SelectedCategory);
		rpc.Write(m_SelectedClass);
		rpc.Write(edit_min_price.GetText().ToInt());
		rpc.Write(edit_max_price.GetText().ToInt());
		rpc.Write(edit_sell_percent.GetText().ToFloat());
		rpc.Write(edit_min_stock.GetText().ToInt());
		rpc.Write(edit_max_stock.GetText().ToInt());
		rpc.Write(edit_quantity_percent.GetText().ToInt());
		rpc.Send(null, DVMERPCs.APPLY_ITEM, true);
		SetStatus("Applying '" + m_SelectedClass + "'...");
	}

	protected void SendSetStock()
	{
		if (m_SelectedClass == "")
		{
			SetStatus("Select an item first.");
			return;
		}

		if (!edit_stock || edit_stock.GetText() == "")
		{
			SetStatus("Enter a stock value first.");
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(m_SelectedClass);
		rpc.Write(edit_stock.GetText().ToInt());
		rpc.Send(null, DVMERPCs.SET_STOCK, true);
		SetStatus("Setting stock of '" + m_SelectedClass + "'...");
	}

	protected void SendReloadCategory()
	{
		if (m_SelectedCategory == "")
		{
			SetStatus("Select a category first.");
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(m_SelectedCategory);
		rpc.Send(null, DVMERPCs.RELOAD_CATEGORY, true);
		SetStatus("Reloading '" + m_SelectedCategory + "' from disk...");
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == btn_refresh)
		{
			RequestCategories();
			if (m_SelectedCategory != "")
			{
				RequestItems(m_SelectedCategory);
			}
			return true;
		}

		if (w == btn_apply)
		{
			SendApply();
			return true;
		}

		if (w == btn_set_stock)
		{
			SendSetStock();
			return true;
		}

		if (w == btn_reload_cat)
		{
			SendReloadCategory();
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
	{
		if (w == list_categories)
		{
			int selected = list_categories.GetSelectedRow();
			if (selected >= 0 && selected < DVMEClientState.CategoryFiles.Count())
			{
				RequestItems(DVMEClientState.CategoryFiles.Get(selected));
			}
			return true;
		}

		if (w == list_items)
		{
			LoadSelectionIntoEditor();
			return true;
		}

		return super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w == edit_search)
		{
			PopulateItems();
			return true;
		}

		return super.OnChange(w, x, y, finished);
	}
}
