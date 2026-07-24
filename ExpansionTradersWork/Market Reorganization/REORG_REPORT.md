# DuskVale Market Reorganization (v2)

Built from the updated `Trader_Organization.cs` under these rules:
1. All categories created as described and assigned to their trader.
2. Duplicates allowed — an item lives in one market category (Expansion engine rule), and every
   additional trader that lists it sells it via its trader `Items` map (`"classname": 0`).
3. Black Market items are exclusive — they are not sold by any non-black-market trader.
4. The OUTLAWED block stays excluded.
5. Clothing and Base Building traders and their categories are untouched.

Drop-in: replace `ExpansionMod/Market` and `ExpansionMod/Traders` in the server profile.
**Delete the old Market folder first** (14 old category files were removed as fully-moved:
AMS_Stims, Batteries, Bayonets, Buttstocks, Cars, Crossbows, Fishing, Gardening, Handguards,
Knifes, Machine_Guns, Pistol_Grips, Shotguns, Submachine_Guns).

## Numbers

- 70 new category files, 12 traders rebuilt (Drugs + Collectibles are new).
- 667 items moved with their existing prices/stock preserved.
- 936 brand-new items at placeholder pricing (buy 100, stock 1-100) — tune via the Market Editor.
- **299 `(Variant to X)` entries attached as skin variants on their parent items** — they appear
  in the parent's skin selector at the trader, exactly how Expansion handles variants.
- 32 duplicate listings wired through trader Items maps (e.g. knives at both Weapons and Hunting,
  Base-Building-owned tools also sold at Hunting/Farming/Food without touching Base Building).
- 1 Black Market exclusivity block: `Ammo_762x39` is BM-only, removed from the Ammunition trader
  listing per rule 3.
- 34 OUTLAWED items excluded (AMR rifles + their ammo/mags/suppressors).
- Verification pass: every classname mentioned in the .cs is accounted for — sellable physically,
  sellable via Items map, attached as a variant, outlawed, or BM-blocked. Zero dropped.

## Still open

- Collectibles "Cards" category exists but the .cs lists no items for it yet.
- `iM7s_Brick_iM7s_Brick_PB` kept verbatim (looks like a typo'd classname).
- DV_Drugs / DV_Collectibles need NPCs — e.g. repoint Linda/Maria at the Dambog bunker:
  `ExpansionTraderLinda.DV_Drugs|...`, `ExpansionTraderMaria.DV_Collectibles|...`
