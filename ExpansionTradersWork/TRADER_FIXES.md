# DuskVale Trader Fixes

Changes applied to `DuskVale Root` to make the traders functional and outfitted like `Working Traders`.

## Functional fixes

1. **Currency registration (the big one)** — Every trader file (`DV_*.json`) and the P2P trader
   use `money_dollar100`, and `Market/Exchange.json` defines its value — but
   `mpmissions/expansion/settings/MarketSettings.json` registered gold/silver bars & nuggets
   (which have no value entries in any market category). Transactions could not complete.
   `Currencies` is now `["money_dollar100"]`.
   > If your live server profile already has `ExpansionMod/Settings/MarketSettings.json`
   > generated, delete it (or edit its `Currencies` the same way) so it re-seeds from the mission.

2. **Black Market trader had nothing to sell** — `DV_BlackMarket.json` had 0 categories / 0 items.
   It now sells: `Event`, `Explosives_And_Grenades`, `FOG_NVG`, `Ghillies`, `Machine_Guns`,
   `Sniper_Rifles`. Adjust to taste.

3. **Missing NPCs**
   - `ExpansionTraderPeter.DV_Medical` (MEDICAL) was missing at **Camp Mercy** — added next to Irena/Judy.
   - `ExpansionTraderKaito.DV_BlackMarket` (BLACK MARKET) existed nowhere — added at **all three camps**,
     placed next to the gun trader Hassan. Nudge positions in-game if they clip anything.
   - `ExpansionP2PTraderNaomi` (P2P MARKET) already spawns from `p2pmarket/P2PTrader_1.json`
     at Camp Kraz (dressed via `YellowKingLoadout`) — verified valid, unchanged.

## Outfits (taken from Working Traders, matched by profession)

| Trader | Profession | Outfit source |
|---|---|---|
| Boris | Clothing | Working `Denis.Clothing` (suit + flat cap) |
| Irena | Food | Working `Quinn.Consumables` |
| Peter | Medical | Working `Judy.Medicals` (paramedic) |
| Baty | Base Building | Working `Niki.BuildingSupplies` |
| Francis | Vehicles | Working `Rolf.Vehicles` (mechanic) |
| Cyril | Vehicle Parts | Working `Naomi.Vehicle_Parts` (mechanic) |
| Hassan | Guns & Attachments | Working `Seth.Attachments` (military dealer) |
| Rolf | Hunting/Fishing | Working `Mirek.Weapons` (hunter) |
| Kaito | Black Market | Working Zalesie black-market Seth (Gorka + balaclava + Benelli) |
| Eva | Farming | kept (farmer outfit — no Working counterpart) |
| Judy | Drinks | kept (bartender outfit — no Working counterpart) |
| Manua | Ammunition | kept (TTSKO military — no Working counterpart) |
| Naomi | P2P Market | via `YellowKingLoadout` in P2P config |
