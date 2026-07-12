# DuskVale Live Market Editor

A standalone mod that adds a **"Market Editor"** page to the **LBmaster (i) admin menu** for
editing DayZ Expansion Market prices and stock **live on the server** — no restart, no reconnect.

## Requirements

- LBmaster Core + LBmaster AdminTools (the (i) admin menu)
- DayZ Expansion Market (+ CF)
- Load this mod after all of the above on server **and** clients.

## Permission

The page is only visible/usable for admins with the LBmaster permission:

```
duskvale.marketeditor
```

Add it to your permission group in LBmaster's `Admins` config (admins with `*` already have it).

## What it does

- Lists every loaded market category file (profile `ExpansionMod/Market/*.json`).
- Lists the items of a category with `classname | buy min-max | sell% | stock min-max (current)`.
  A search box filters the list.
- Edit per item: **Min/Max Price Threshold** (buy price range), **Sell Price %**
  (-1 = server default), **Min/Max Stock Threshold**, **Quantity %**, and the **current stock**.
- **Apply + Save + Sync** — applies the values to the running server, saves the category JSON
  to the profile Market folder, and pushes the new values to every connected client
  (Expansion caches item prices client-side per session; this mod patches those caches via RPC,
  so players see the new prices on their next menu refresh without reconnecting).
- **Set Stock Only** — writes the entered stock for the item into every trader zone and saves
  the zone files (mission `expansion/traderzones/*.json`).
- **Reload Category File From Disk** — re-reads the category JSON (edit it externally, then
  reload) and live-updates all items that already exist in memory. Items *added* to the file
  need a server restart (Expansion assigns network IDs at startup); the page tells you when
  that's the case.

## Notes / limitations

- Adding or removing items/categories still requires a restart — this editor changes values
  of existing items (prices, sell %, stock thresholds, quantity %, current stock).
- Price = f(stock): low stock → price near MaxPriceThreshold, full stock → near
  MinPriceThreshold. Setting Min Stock = Max Stock makes an item static price/stock.
- All edits are logged through the LBmaster logger (AdminTools log) with the acting admin.

## Packing

Pack the `DuskVale_MarketEditor` folder into `DuskVale_MarketEditor.pbo`
(`$PBOPREFIX$` is already set) and ship it in your mod folder, e.g.
`@DuskVale-Overrides/addons/`.
