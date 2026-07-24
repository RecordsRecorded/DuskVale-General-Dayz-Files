# DuskVale Market Reorganization

Generated from `Trader_Organization.cs` against the current server market (the Traders.zip you provided).
Drop-in replacement: copy `ExpansionMod/Market` and `ExpansionMod/Traders` over your server profile's
`ExpansionMod` folder (replace both folders). **Delete the old Market folder first** so removed/renamed
category files don't linger — leftovers would create duplicate items.

## What was built

- **12 trader files rebuilt** with per-trader categories exactly as organized in the .cs:
  Drugs (NEW), Collectibles (NEW), BlackMarket, Ammo, Guns, Hunting, Drinks, Food, Farming,
  Medical, VehicleParts, Vehicles.
- **DV_Clothing.json and DV_BaseBuilding.json untouched**, and every market category they
  reference is untouched too.
- **69 new market category files** (named `<Trader>_<Category>.json`, e.g. `Weapons_Sidearms.json`,
  `BM_Explosives.json`, `Drugs_Bricks.json`).
- **850 items moved** from the old categories into the new ones **with their existing prices,
  stock thresholds and spawn attachments preserved**.
- **1109 brand-new items** (drugs, retro game collectibles, most vehicles, etc. that were not in
  any market file) added with **placeholder pricing: buy 100 / sell default / stock 1-100** —
  tune them with the Market Editor admin page or by editing the JSONs.
- **14 old category files removed** because everything in them moved out:
  AMS_Stims, Batteries, Bayonets, Buttstocks, Cars, Crossbows, Fishing, Gardening, Handguards,
  Knifes, Machine_Guns, Pistol_Grips, Shotguns, Submachine_Guns.
- Old categories that still hold unmoved items were kept as-is (81 files) so nothing you didn't
  reorganize disappears from the economy.

## Decisions you may want to review

1. **OUTLAWED block excluded** — the `(--- OUTLAWED ... )` block inside the Black Market section
   (AMR rifles A6_AK50/AS50/M82/etc. + their ammo/mags/suppressors, 34 items) was NOT added to
   any trader, since it's marked outlawed. Say the word if they should be sold after all.

2. **Items listed under two traders** (33 cases) — first listing wins, the duplicate was skipped
   (an item can only live in one Expansion market category). Notables:
   - `Ammo_762x39`: kept at **Black Market** (skipped at Ammunition trader)
   - ~28 A6 grips listed under both Weapons "Grips" and "Handguards": kept in **Grips**
   - `huntingknife`, `fangeknife`, `FOG_Strongarm_Knife`: the .cs lists them under Weapons
     category 13 "Silencers" (which actually contains all the melee/knives) AND under Hunting
     "Knives" — kept at **Weapons**. The Hunting Knives category still has 8 other knives.

3. **Clothing/Base Building protection conflicts** (28 items) — the .cs asks for these at other
   traders, but they currently live in categories referenced by DV_BaseBuilding (Tools, Supplies,
   Lights, Kits, Containers, RAG_Misc, A6_Base_Storage). Since Base Building must not be touched,
   they **stayed with Base Building** and were skipped at the requesting trader:
   sickle/crowbar/shovel/pickaxe/sledgehammer/farminghoe/woodaxe/firefighteraxe/pitchfork/
   fieldshovel/meattenderizer/iceaxe (wanted by Weapons melee), BearTrap/Pot/FryingPan/
   PortableGasStove/SmallGasCanister/PetrolLighter/Matchbox/LeatherSewingKit (wanted by
   Hunting/Food), greenhouse kits rag_baseitems_greenhouse_kit/_big_kit/A6_GreenHouse_Kit
   (wanted by Farming "Housing" — that category is currently empty because of this),
   SewingKit/CamoNet/WoodenCrate/SeaChest/Barrel_Blue.
   If you want any of these moved anyway, tell me which and I'll re-run.

4. **Empty categories from the .cs**: Collectibles "Cards" (no items listed yet) and Farming
   "Housing" (see above) exist but are empty.

5. The typo'd classname `iM7s_Brick_iM7s_Brick_PB` was kept verbatim in Drugs_Bricks — fix the
   classname in the JSON if it doesn't exist in the mod.

## New traders need NPCs

`DV_Drugs.json` and `DV_Collectibles.json` are ready but no NPC sells them yet. Add lines to a
trader .map, e.g. at the Dambog bunker:
```
ExpansionTraderLinda.DV_Drugs|<x y z>|<yaw 0 0>|<outfit>
ExpansionTraderMaria.DV_Collectibles|<x y z>|<yaw 0 0>|<outfit>
```
(Currently Linda and Maria are both bound to DV_BlackMarket — repointing them like above would
give the bunker a drugs dealer and a collectibles fence.)
