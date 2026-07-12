# DuskVale Trader Fixes

Changes applied to `DuskVale Root`. Trader rosters and positions come from the
corrected camp trader files supplied on 2026-07-12 (`Camp*001Traders.map`).

## Functional fixes

1. **Currency registration (the big one)** — Every trader file (`DV_*.json`) and the P2P traders
   use `money_dollar100`, and `Market/Exchange.json` defines its value — but
   `mpmissions/expansion/settings/MarketSettings.json` registered gold/silver bars & nuggets
   (which have no value entries in any market category). Transactions could not complete.
   `Currencies` is now `["money_dollar100"]`.
   > If your live server profile already has `ExpansionMod/Settings/MarketSettings.json`
   > generated, delete it (or edit its `Currencies` the same way) so it re-seeds from the mission.

2. **P2P traders (Naomi) now spawn at all three camps** — P2P traders are NOT spawned from
   `expansion/traders/*.map`; they spawn from `expansion/p2pmarket/P2PTrader_<ID>.json`, which
   is what links the NPC to the P2P market menu (a map-placed `ExpansionP2PTraderNaomi` would
   just stand there with no menu). The Naomi entries from the corrected camp maps were therefore
   converted into P2P configs at their exact positions/orientations:
   - `P2PTrader_1.json` — Camp Kraz (existed; orientation synced to the corrected map)
   - `P2PTrader_2.json` — Camp Mercy (new)
   - `P2PTrader_3.json` — Camp Sanctuary (new)
   All three use the same loadout (`YellowKingLoadout`), currency and settings as the original.
   > Vehicle spawn position for P2P vehicle listings is inherited from trader 1
   > (`7100, 300, 7750`) on all three — adjust per camp if you want local vehicle delivery.

3. **Trader-file bindings restored** — The corrected camp maps listed NPC class names without
   the `.DV_*` trader file suffix (a market trader without a trader file has no stock). Every
   market trader line is bound to its DV file again (e.g. `ExpansionTraderHassan.DV_Guns`).

4. **Black Market trader file filled** — `DV_BlackMarket.json` had 0 categories / 0 items; it now
   sells `Event`, `Explosives_And_Grenades`, `FOG_NVG`, `Ghillies`, `Machine_Guns`,
   `Sniper_Rifles`. Note: **Kaito is not placed at any camp** (he is not in the corrected camp
   files). When you pick his spot, add a line like:
   `ExpansionTraderKaito.DV_BlackMarket|X Y Z|YAW 0 0|GorkaPants_Flat,GorkaEJacket_Flat,UKAssVest_Black,BalaclavaMask_Black,WorkingGloves_Black,AviatorGlasses,BoonieHat_DPM,TTSKOBoots,AssaultBag_Black,Expansion_BenelliM4`

5. **Roster note** — Camp Mercy has no Peter (Medical) in the corrected files (Kraz and
   Sanctuary have him). Add a `ExpansionTraderPeter.DV_Medical|...` line there if that's not
   intended.

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
| Kaito | Black Market | Working Zalesie black-market Seth (line ready above, not placed) |
| Eva | Farming | kept (farmer outfit — no Working counterpart) |
| Judy | Drinks | kept (bartender outfit — no Working counterpart) |
| Manua | Ammunition | kept (TTSKO military — no Working counterpart) |
| Naomi | P2P Market | via `YellowKingLoadout` in the three P2P configs |
