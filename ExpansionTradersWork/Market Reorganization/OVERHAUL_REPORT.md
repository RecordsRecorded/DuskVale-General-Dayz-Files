# DuskVale Trader Overhaul (7-point order, 2026-07-24)

Everything below was applied to `Market Reorganization/ExpansionMod` (the profile
drop-in) **and** mirrored into `DuskVale Root` (profile + mission), so the repo now
holds one coherent server image.

## 1. Exact categories per Trader Organization.cs

- Every non-protected trader now lists **exactly** the .cs categories, in .cs order
  (`UseCategoryOrder = 1`). All `Items`-map extra listings were removed — they were
  the cause of the ghost headers in your screenshot (KITS / SUPPLIES / Silencers on
  Hunting & Fishing): an item listed at a trader always renders under its *home*
  category's header, so the only way to get exact categories is for every item to
  physically live in one of the trader's own categories.
- 29 leftover legacy market categories deleted (Ammo, Pistols, Rifles, Medical,
  Meat, Fish, Food, Drinks, CookZ_*, MFP_*, Fortune_Vehicles, Event, ...).
  `Exchange.json` kept — it holds the market value of `money_dollar100`.
- Example-001 fixed: `BearTrap` and `LeatherSewingKit` now live in
  `Hunt_Traps_Tools` (they were stranded in Base Building's Supplies/Kits files,
  which is why they showed as 1-item ghost categories).
- Example-002 fixed: **PAZprison** is its own Transport entry with variants
  `PAZprison_blue/white/yellow`; PAZ keeps only `PAZ_beige/blue/green/red`;
  `PAZ_BANDIT` is its own Camper entry per the .cs.
- Newest .cs variant changes applied: `ExpansionKeyChain_*` are now variants of
  `ExpansionCarKey`; `Barrel_Red/Green/Yellow` are variants of `Barrel_Blue`
  (moved to `VParts_Storage_Expansion`, which also removes a live duplicate-classname
  config error — `Barrel_Red` existed in two categories).
- OUTLAWED block (AMR rifles/ammo/mags/attachments) verified absent from every file.
- Magazine classnames corrected again per `A6_Weapons_Magazines_Types.xml`
  (the .cs still uses the wrong `AG_Mag_*` prefixes; all mapped to `A6_Mag_*`,
  existing prices/stock preserved).

### Items the engine could not duplicate (one market category per classname)
The .cs lists these in two places; they were kept at their **first** .cs location:
- ~30 `a6_*_grip*` items: kept in Weapons **Grips** (also listed under Handguards).
- `FOG_Strongarm_Knife`, `HuntingKnife`, `FangeKnife`: kept in Weapons **Silencers**
  (the .cs cat-13 melee list) — also listed under Hunting Knives. Hunting Knives
  still shows BoneKnife, CrocodileDundeeKnife and the geb fish knives, matching
  your screenshot. Say the word if you'd rather have those three at Hunting instead.
- `Ammo_762x39`: Black-Market-exclusive (rule 3); its civilian variants
  (`PSgzh/FMJ/HP/T45M1gzh/PPgzh`) are standalone items at the Ammunition trader.

### Protected-file edits (flagging these per the "speak up" rule)
Clothing/Base Building traders and categories are otherwise untouched, but the .cs
explicitly claims these items for the 12 rebuilt traders, and the engine forbids an
item existing in two categories — so they were **moved out** of protected files:
- `Tools.json` → Weapons "Silencers" (melee list): sickle, crowbar, shovel, pickaxe,
  sledgehammer, farminghoe, woodaxe, firefighteraxe, pitchfork, meattenderizer,
  iceaxe, fryingpan
- `Supplies.json` → various: beartrap (Hunting), pot / gas canisters (Food Supplies),
  camonet (Vehicle Parts)
- `Kits.json` → sewingkit, leathersewingkit (Hunting / Food Supplies)
- `Lights.json` → matchbox, petrollighter, portablegasstove (Food Supplies)
- `Containers.json` → barrel_blue (+color variants), seachest, woodencrate
  (Vehicle Parts Storage Expansion)
- `A6_Base_Storage.json` / `RAG_Misc.json` → greenhouse kits (Farming Housing)
Say the word to revert any of these to their old category instead.

## 2. DV tag removed everywhere
Trader files renamed (`DV_Guns.json` → `Guns.json`, etc. — all 14), all `.map`
bindings updated (`ExpansionTraderHassan.DV_Guns` → `.Guns`). No `DV_` string
remains in any JSON or map file, so a future server rename is trivial.

## 3. Prices rounded to $100
Every `MinPriceThreshold`/`MaxPriceThreshold` in every category (including
Clothing/Base Building files — prices only, nothing else touched there) is a
multiple of 100, minimum 100. Sell percents rounded to whole numbers so they
survive Expansion's bfloat16 netsync exactly. Note: a *worn/damaged* item always
sells for proportionally less, so sub-$100 remainders on sales of damaged goods
are inherent to Expansion — payouts are made in $100 bills regardless.

## 4. DisplayCurrencyName = "USD"
Set in all 14 trader files and the 3 P2P trader configs.

## 5. Trader icons (valid DayZ Expansion icon names)
| Trader | Icon | | Trader | Icon |
|---|---|---|---|---|
| Medical | Medic Box | | Vehicles | Car |
| Vehicle Parts | Pipe Wrench | | Guns | Gun |
| Ammunition | Gun Bullets | | Black Market | Bandit |
| Hunting & Fishing | Fishing | | Drinks | Soda |
| Food | Can Of Beans Big | | Farming | Carrot |
| Drugs | Powder | | Collectibles | Star |
| Clothing | Cloth | | Base Building | Nails |

## 6. Stock & purchase policy
- **Infinite (static) stock** — flat price, never runs out: all Medical, Vehicles,
  Vehicle Parts, Weapons (guns + all attachments), Ammunition categories, plus
  Hunting Traps & Tools / Knives / Bait & Lures, Farming Housing / Seeds /
  Fertilizer / Bees, and Drugs Production equipment.
- **Sell-only** (players can sell, nobody can buy): Drugs Bricks, Hunting Meats,
  Hunting Fish — done at category level (`:2` suffix).
- **Player-driven stock** (start at 0; only purchasable after someone sells one):
  all Food and Drinks categories + Farming produce (Fruits/Veggies/Mushrooms,
  Shrooms). `InitStockPercent = 0` and the mission `traderzones/World.json` stock
  map was regenerated to match (382 items start at zero).
- Black Market kept on limited dynamic stock (3–6 for high-end pieces) so BM gear
  stays scarce — flag it if you want BM infinite too.

## 7. P2P Market fix
The endless-loading menu was caused by the `ExpansionP2PTraderNaomi` NPCs being
spawned from the trader `.map` files. A map-spawned P2P NPC has **no trader ID**
(only NPCs spawned from `p2pmarket/P2PTrader_<N>.json` configs get one), so the
menu requests hang forever. The Working Traders folder confirms this — it ships no
map-spawned P2P NPCs at all. Fix applied:
- Naomi lines removed from all trader `.map` files.
- The three valid `P2PTrader_1/2/3.json` configs (Kraz / Mercy / Sanctuary, same
  positions/rotations) spawn her instead, linked to the P2P menu, currency
  `Money_Dollar100`, display USD. `P2PMarketSettings.json` is enabled — the market
  itself remains fully player-driven, no listings were touched.

## Server install
1. Replace profile `ExpansionMod/Market` and `ExpansionMod/Traders` with the new
   folders (delete the old ones first).
2. Replace mission `expansion/traders/*.map`, `expansion/p2pmarket/`, and
   `expansion/traderzones/World.json` with the new ones.
3. Make sure no other `P2PTrader_*.json` (especially empty 0-byte ones) remain in
   the mission p2pmarket folder.
