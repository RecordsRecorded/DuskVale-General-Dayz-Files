# Sunnyvale Price Sheet -> Expansion Market Pricing Report

Applied by script `apply_prices.py` (hand-curated display-name -> classname mapping).
Market files: `ExpansionTradersWork/Market Reorganization/ExpansionMod/Market/*.json`

## Rules used
- `MinPriceThreshold = MaxPriceThreshold =` purchase price (flat prices).
- `SellPricePercent = round(sell / purchase * 100, 1)` (50.0 written as 50).
- `Not Purchasable` / purchase `N/A`: `Min = Max = sell * 2`, `SellPricePercent = 50` -> the trader sell value matches the sheet, but the item is still technically buyable at 2x its sell value (sell-only priced).
- Stock `Yes` -> Min 1 / Max 100; `No`, `N/A` or column absent -> Min = Max = 1 (static). Items whose ClassName starts with `A6_Mag_` never had stock thresholds touched (already set from the A6 types.xml).
- `SpawnAttachments`, `Variants`, `QuantityPercent` untouched. Rows matching a *Variant* of a market parent cannot be priced separately (variants share the parent's price) and are listed below.

## Totals
- PDF rows parsed (hand-transcribed): **1617** (incl. ~149 verbatim repeats of the magazine/loose-ammo lists on the Black Market Armory Supplies pages)
- Rows applied to market items: **923** (touched **1024** market entries; some rows price multiple entries, some classnames exist in two files)
- Rows matching a Variant (parent priced from its own row instead): **38**
- Duplicate / alternate-column rows skipped: **24** summary entries (+ the verbatim BM repeats)
- Unmatched rows: **483**

## Sell-only priced items ("Not Purchasable" on sheet, still technically buyable at 2x sell)
- Psilocybin Blue Veils -> `iM7s_Psilocybin_BlueVeils` (sell 7500, priced 15000)
- Psilocybin GhostCaps -> `iM7s_Psilocybin_GhostCaps` (sell 7500, priced 15000)
- Psilocybin Golden Teacher -> `iM7s_Psilocybin_GoldenTeacher` (sell 7500, priced 15000)
- Psilocybin Mystic Void -> `iM7s_Psilocybin_MysticVoid` (sell 7500, priced 15000)
- Psilocybin Sunspores -> `iM7s_Psilocybin_Sunspores` (sell 7500, priced 15000)
- Beehive Kit -> `rag_beehive_kit` (sell 37500, priced 75000)
- Honey Jar -> `rag_honey_jar` (sell 30000, priced 60000)
- HoneyWine Bottle -> `rag_honeywine_bottle` (sell 55000, priced 110000)
- Black Bass Fillet -> `geb_BlackBass` (sell 315, priced 630)
- Black Devil Snail -> `geb_BlackDevilSnail` (sell 175, priced 350)
- Bluegill Fillet -> `geb_BlueGill` (sell 225, priced 450)
- Bowfin Fillet -> `geb_BowFin` (sell 225, priced 450)
- Brook Trout Fillet -> `geb_BrookTrout` (sell 315, priced 630)
- Brown Trout Fillet -> `geb_BrownTrout` (sell 315, priced 630)
- Carp Fillet -> `carpfilletmeat` (sell 225, priced 450)
- Cutthroat Trout Fillet -> `geb_CutThroatTrout` (sell 315, priced 630)
- European Crayfish -> `geb_EuropeanCrayFish` (sell 175, priced 350)
- Fathead Minnow Fillet -> `geb_FatHeadMinnow` (sell 175, priced 350)
- Flathead Catfish Fillet -> `geb_FlatHeadCatFish` (sell 468, priced 936)
- Lake Trout Fillet -> `geb_LakeTrout` (sell 225, priced 450)
- Largemouth Bass Fillet -> `geb_LargeMouthBass` (sell 315, priced 630)
- Northern Pike Fillet -> `geb_NorthernPike` (sell 945, priced 1890)
- Rainbow Trout Fillet -> `geb_RainbowTrout` (sell 315, priced 630)
- Sauger Fillet -> `geb_Sauger` (sell 225, priced 450)
- Signal Crayfish -> `geb_SignalCrayFish` (sell 175, priced 350)
- Slimy Sculpin Pillet -> `geb_SlimySculpin` (sell 225, priced 450)
- Smallmouth Bass Fillet -> `geb_SmallMouthBass` (sell 315, priced 630)
- Sockeye Salmon Fillet -> `geb_SockEyeSalmon` (sell 225, priced 450)
- Steelhead Trout Fillet -> `steelheadtroutfilletmeat` (sell 225, priced 450)
- Sunfish Fillet -> `geb_SunFish` (sell 315, priced 630)
- Walleye Fillet -> `geb_WallEye` (sell 315, priced 630)
- Walleye Pollock Fillet -> `walleyepollock` (sell 225, priced 450)
- White Bass Fillet -> `geb_WhiteBass` (sell 315, priced 630)
- Yellow Perch Fillet -> `geb_YellowPerch` (sell 225, priced 450)
- Angelfish Fillet -> `geb_AngelFish` (sell 315, priced 630)
- Angelshark Fillet -> `geb_AngelShark` (sell 2310, priced 4620)
- Asian Sea Bass Fillet -> `geb_AsianSeaBass` (sell 945, priced 1890)
- Atlantic Blue Marlin Fillet -> `geb_AtlanticBlueMarlin` (sell 1350, priced 2700)
- Blood Clam -> `geb_BloodClam` (sell 455, priced 910)
- Blue Jellyfish Meat -> `geb_BlueJellyFish` (sell 225, priced 450)
- Blue Tang Fillet -> `geb_BlueTang` (sell 315, priced 630)
- Bonita Fillet -> `geb_Bonita` (sell 225, priced 450)
- Cherry Salmon -> `geb_CherrySalmon` (sell 225, priced 450)
- Chinook Salmon Fillet -> `geb_ChinookSalmon` (sell 315, priced 630)
- Flathead Mullet Fillet -> `geb_FlatHeadMullet` (sell 225, priced 450)
- Great White Shark Fillet -> `geb_GreatWhiteShark` (sell 3150, priced 6300)
- Humphead Wrasse Fillet -> `geb_HumpHeadWrasse` (sell 1260, priced 2520)
- King Crab Legs -> `geb_KingCrab` (sell 315, priced 630)
- Largehead Hairtail Fish Fillet -> `geb_LargeHeadHairTailFish` (sell 175, priced 350)
- Leopard Shark Fillet -> `geb_LeopardShark` (sell 1260, priced 2520)
- Mackerel Fillet -> `mackerelfilletmeat` (sell 315, priced 630)
- Mahi Mahi Fillet -> `geb_MahiMahi` (sell 2100, priced 4200)
- Mussel -> `geb_Mussel` (sell 455, priced 910)
- Pacific Cod Fillet -> `geb_PacificCod` (sell 315, priced 630)
- Readhead Cichlid Fillet -> `geb_RedHeadCichlid` (sell 315, priced 630)
- Roughneck Rock Fish Fillet -> `geb_RoughNeckRock` (sell 315, priced 630)
- Severum Fillet -> `geb_Severum` (sell 315, priced 630)
- Siamese Tiger Fish Fillet -> `geb_SiameseTigerFish` (sell 1420, priced 2840)
- Snowcrab Legs -> `geb_SnowCrab` (sell 315, priced 630)
- Starfish -> `geb_StarFish` (sell 325, priced 650)
- Yellow Fin Tuna Fillet -> `geb_YellowFinTuna` (sell 1680, priced 3360)
- Bear Steak -> `bearsteakmeat` (sell 300, priced 600)
- Beef Steak -> `cowsteakmeat` (sell 1120, priced 2240)
- Boar Steak -> `boarsteakmeat` (sell 960, priced 1920)
- Chevon Steak -> `goatsteakmeat` (sell 1200, priced 2400)
- Chicken Steak -> `chickenbreastmeat` (sell 950, priced 1900)
- Crocodile Meat -> `CrocodileSteakMeat` (sell 600, priced 1200)
- Fat -> `Lard` (sell 1800, priced 3600)
- Fox Steak -> `foxsteakmeat` (sell 950, priced 1900)
- Hare Leg -> `rabbitlegmeat` (sell 600, priced 1200)
- Mutton Steak -> `sheepsteakmeat` (sell 1200, priced 2400)
- Otter Meat -> `OtterSteakMeat` (sell 1100, priced 2200)
- Pork Steak -> `pigsteakmeat` (sell 960, priced 1920)
- Reindeer Steak -> `reindeersteakmeat` (sell 1220, priced 2440)
- Venison Steak -> `deersteakmeat` (sell 1120, priced 2240)
- Wolf Steak -> `wolfsteakmeat` (sell 100, priced 200)
- GameBoy Cartridge -> `Hive_Retro_GBC_Alice_In_Wonderland` (sell 20000, priced 40000)
- Barney Brick -> `iM7s_Brick_Barney` (sell 20000, priced 40000)
- Black Tar Brick -> `iM7s_Brick_BlackTar` (sell 35000, priced 70000)
- Blue Cookies Brick -> `iM7s_Brick_BlueCookies` (sell 20000, priced 40000)
- Brick of Brown -> `iM7s_Brick_Brown` (sell 40000, priced 80000)
- C.U.M x Baby Batter Brick -> `iM7s_Brick_BabyBatter` (sell 20000, priced 40000)
- Champagne Brick -> `iM7s_Brick_Champagne` (sell 20000, priced 40000)
- Cheetah Piss Brick -> `iM7s_Brick_CheetahPiss` (sell 20000, priced 40000)
- Cocaine Brick -> `iM7s_Brick_Coke` (sell 40000, priced 80000)
- Crystal Blue Brick -> `iM7s_Brick_Crystal_Blue` (sell 50000, priced 100000)
- Crystal PB Brick -> `iM7s_Brick_iM7s_Brick_PB` (sell 60000, priced 120000)
- Crystal Pink Brick -> `iM7s_Brick_Crystal_Pink` (sell 50000, priced 100000)
- Ecstasy Brick -> `iM7s_Brick_Thizz_Blue` (sell 45000, priced 90000)
- Ecstasy Brick (2nd) -> `iM7s_Brick_Thizz_Green` (sell 45000, priced 90000)
- Ecstasy Brick (3rd) -> `iM7s_Brick_Thizz_Purple` (sell 45000, priced 90000)
- Ecstasy Brick (4th) -> `iM7s_Brick_Thizz_Yellow` (sell 45000, priced 90000)
- Gelato Brick -> `im7s_Brick_Gelato` (sell 20000, priced 40000)
- MDMA Brick -> `iM7s_Brick_MDMA` (sell 25000, priced 50000)
- Paragon Brick -> `iM7s_Brick_Paragon` (sell 20000, priced 40000)

## Judgment calls / notable mappings
- **Blood Collection Kit** -> `bloodbagempty`: BloodBagEmpty = blood collection kit
- **Chlorine Tablets** -> `purificationtablets`: renamed vanilla Water Purification Tablets (only chlorine-tablet item)
- **Codine Pills** -> `painkillertablets`: renamed vanilla Painkillers (only remaining pill item)
- **First Aid Pouch** -> `firstaidkit`: renamed vanilla First Aid Kit
- **IFAK** -> `ams_ifak_black`: parent; color IFAKs are Variants
- **Liberty Cap** -> `psilocybemushroom`: vanilla PsilocybeMushroom display = Liberty Cap
- **Crankbait Lure** -> `geb_Lure`: best candidate among geb_Lure..Lure4 (base class); judgment call
- **Fun Red Tackle Box** -> `geb_FunGreenTackle`: parent of geb_FunRedTackle variant
- **Old Green Tackle Box** -> `geb_OldBlueTackle`: parent of geb_OldGreenTackle variant
- **Red Fishing Rod** -> `geb_BlueFishingRod`: parent of geb_RedFishingRod variant
- **Red Tackle Box** -> `geb_BlueTackle`: parent of geb_RedTackle variant
- **Black Bass Fillet** -> `geb_BlackBass`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Black Devil Snail** -> `geb_BlackDevilSnail`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Bluegill Fillet** -> `geb_BlueGill`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Bowfin Fillet** -> `geb_BowFin`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Brook Trout Fillet** -> `geb_BrookTrout`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Brown Trout Fillet** -> `geb_BrownTrout`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Carp Fillet** -> `carpfilletmeat`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Cutthroat Trout Fillet** -> `geb_CutThroatTrout`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **European Crayfish** -> `geb_EuropeanCrayFish`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Fathead Minnow Fillet** -> `geb_FatHeadMinnow`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Flathead Catfish Fillet** -> `geb_FlatHeadCatFish`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Lake Trout Fillet** -> `geb_LakeTrout`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Largemouth Bass Fillet** -> `geb_LargeMouthBass`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Northern Pike Fillet** -> `geb_NorthernPike`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Rainbow Trout Fillet** -> `geb_RainbowTrout`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Sauger Fillet** -> `geb_Sauger`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Signal Crayfish** -> `geb_SignalCrayFish`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Slimy Sculpin Pillet** -> `geb_SlimySculpin`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Smallmouth Bass Fillet** -> `geb_SmallMouthBass`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Sockeye Salmon Fillet** -> `geb_SockEyeSalmon`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Steelhead Trout Fillet** -> `steelheadtroutfilletmeat`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Sunfish Fillet** -> `geb_SunFish`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Walleye Fillet** -> `geb_WallEye`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Walleye Pollock Fillet** -> `walleyepollock`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **White Bass Fillet** -> `geb_WhiteBass`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Yellow Perch Fillet** -> `geb_YellowPerch`: fillet row applied to the fish item present in market (GEB mod has one market entry per species)
- **Angelfish Fillet** -> `geb_AngelFish`: fillet row applied to the fish item present in market
- **Angelshark Fillet** -> `geb_AngelShark`: fillet row applied to the fish item present in market
- **Asian Sea Bass Fillet** -> `geb_AsianSeaBass`: fillet row applied to the fish item present in market
- **Atlantic Blue Marlin Fillet** -> `geb_AtlanticBlueMarlin`: fillet row applied to the fish item present in market
- **Blood Clam** -> `geb_BloodClam`: fillet row applied to the fish item present in market
- **Blue Jellyfish Meat** -> `geb_BlueJellyFish`: fillet row applied to the fish item present in market
- **Blue Tang Fillet** -> `geb_BlueTang`: fillet row applied to the fish item present in market
- **Bonita Fillet** -> `geb_Bonita`: fillet row applied to the fish item present in market
- **Cherry Salmon** -> `geb_CherrySalmon`: fillet row applied to the fish item present in market
- **Chinook Salmon Fillet** -> `geb_ChinookSalmon`: fillet row applied to the fish item present in market
- **Flathead Mullet Fillet** -> `geb_FlatHeadMullet`: fillet row applied to the fish item present in market
- **Great White Shark Fillet** -> `geb_GreatWhiteShark`: fillet row applied to the fish item present in market
- **Humphead Wrasse Fillet** -> `geb_HumpHeadWrasse`: fillet row applied to the fish item present in market
- **King Crab Legs** -> `geb_KingCrab`: fillet row applied to the fish item present in market
- **Largehead Hairtail Fish Fillet** -> `geb_LargeHeadHairTailFish`: fillet row applied to the fish item present in market
- **Leopard Shark Fillet** -> `geb_LeopardShark`: fillet row applied to the fish item present in market
- **Mackerel Fillet** -> `mackerelfilletmeat`: fillet row applied to the fish item present in market
- **Mahi Mahi Fillet** -> `geb_MahiMahi`: fillet row applied to the fish item present in market
- **Mussel** -> `geb_Mussel`: fillet row applied to the fish item present in market
- **Pacific Cod Fillet** -> `geb_PacificCod`: fillet row applied to the fish item present in market
- **Readhead Cichlid Fillet** -> `geb_RedHeadCichlid`: fillet row applied to the fish item present in market
- **Roughneck Rock Fish Fillet** -> `geb_RoughNeckRock`: fillet row applied to the fish item present in market
- **Severum Fillet** -> `geb_Severum`: fillet row applied to the fish item present in market
- **Siamese Tiger Fish Fillet** -> `geb_SiameseTigerFish`: fillet row applied to the fish item present in market
- **Snowcrab Legs** -> `geb_SnowCrab`: fillet row applied to the fish item present in market
- **Starfish** -> `geb_StarFish`: fillet row applied to the fish item present in market
- **Yellow Fin Tuna Fillet** -> `geb_YellowFinTuna`: fillet row applied to the fish item present in market
- **Chevon Steak** -> `goatsteakmeat`: chevon = goat meat
- **Chicken Steak** -> `chickenbreastmeat`: chicken breast = only chicken meat item
- **Fat** -> `Lard`: vanilla Lard display name = Fat
- **Big Drawer Cabinet** -> `rag_baseitems_big_drawer_cabinet_kit`: darkwood version left unpriced (no row)
- **Carpet Rectangular** -> `rag_baseitems_carpet_rectangular_celtic`, `rag_baseitems_carpet_rectangular_checker`, `rag_baseitems_carpet_rectangular_cross_round`, `rag_baseitems_carpet_rectangular_rag`, `rag_baseitems_carpet_rectangular_simple`, `rag_baseitems_carpet_rectangular_square_dashed`, `rag_baseitems_carpet_rectangular_stripes`, `rag_baseitems_carpet_rectangular_waves`: single sheet row applied to all 8 rectangular carpet patterns
- **Blue Single Locker** -> `rag_baseitems_single_locker_kit`: base kit assumed blue; color kits left unpriced
- **Blue Triple Locker** -> `rag_baseitems_triple_locker_kit`: base kit assumed blue
- **Teddy Bear** -> `rag_teddybear`: RaG section context; vanilla bear_beige left unpriced
- **Ancient Bed** -> `rag_baseitems_bed_kit`: only bed kit item
- **Coffee Station** -> `rag_baseitems_coffee_hutch_kit`: coffee hutch = coffee station
- **Vintage Floor Lamp** -> `rag_baseitems_lantern_kit`: floor lantern kit; judgment call
- **Kiddie Pool Blue** -> `rag_baseitems_small_pool_blue_kit`: other pool colors left unpriced
- **Victorian Antique Round Table** -> `rag_baseitems_victorian_table_white_kit`: only remaining victorian table; same price as wood one
- **Paintnarok unpainted** -> `rag_baseitems_wallpicture_unpainted`: unpainted RaGnarok wall picture
- **Big Wooden shelf Darkwood** -> `rag_baseitems_woodenshelf_big_kit`: big shelf = darkwood; lightwood version left unpriced
- **Barrel** -> `barrel_blue`: closed barrel parent; barrelholes_blue (open) left unpriced
- **Canopy Tent** -> `partytent`: party tent = canopy tent; judgment call
- **Medium Tent** -> `mediumtent`: mediumtent_green separate entry left unpriced
- **Cooking Pot** -> `pot`: sheet gives 4.8k/6k (80% sell) as printed
- **Gas stove** -> `portablegasstove`: BM page lists 'Portable Gas Stove 9k/9k' for the same class; regular trader row applied, BM row skipped
- **PVS-31** -> `fog_pvs31`: fde/wrapped PVS-31 entries left unpriced
- **Construction Light** -> `spotlight`: vanilla Spotlight is the only work-light; judgment call
- **Flag** -> `flag_altis`: only flag entry (all other flags are its Variants)
- **Wire Mech Barrier** -> `hescobox`: vanilla HescoBox display = Wire Mesh Barrier
- **Repaint SprayCan** -> `rag_baseitems_spraycan_paint`: RaG repaint can; spraycan_white left unpriced
- **Sledgehammer** -> `sledgehammer`: sheet prints 400/5k (8% sell) - applied as printed, possible sheet typo
- **Tire Iron** -> `lugwrench`: vanilla LugWrench display = Tire Iron
- **Weaponoil** -> `rag_baseitems_oil_bottle`: RaG weapon oil bottle; judgment call
- **Saw** -> `handsaw`: hand saw
- **GoKart** -> `Kart`: Kart/Kart_Orange = go-kart; judgment call
- **Ada 4x4** -> `OffroadHatchback`: vanilla Ada 4x4
- **Audi RS6 ABT White** -> `audi_rs6`: parent; colors are Variants
- **BMW M3 (White)** -> `bmw`: bmw parent (M3); BMW_X5 separate, unpriced
- **FordCrown_Victoria (Sheriff)** -> `interceptor`: Interceptor = Crown Victoria; judgment call
- **Gunter 2** -> `Hatchback_02`: vanilla Gunter 2
- **MotorHome White** -> `motorhome`: parent; colors are separate entries left unpriced
- **Olga 24** -> `Sedan_02`: vanilla Olga 24
- **Sarka 120** -> `CivilianSedan`: vanilla Sarka 120
- **Transit Bus** -> `expansionbus`: Expansion bus; judgment call
- **M3S Covered** -> `Truck_01_Covered`: vanilla M3S covered truck
- **Mh-6 Back-Left Door** -> `expansion_mh6_door_2_1`: door index by convention; all four doors share 250/500
- **Mh-6 Back-Right Door** -> `expansion_mh6_door_2_2`: door index by convention
- **Mh-6 Front-Left Door** -> `expansion_mh6_door_1_1`: door index by convention
- **Mh-6 Front-Right Door** -> `expansion_mh6_door_1_2`: door index by convention
- **UH-1H Left Door** -> `expansionuh1hdoor_1_1`: two identical Left Door rows mapped to the two left doors (same price)
- **AW101 MerlinHC3 BAF** -> `expansionmerlin`: non-PVP price column applied
- **Gyrocopter** -> `expansiongyrocopter`: two gyro rows: smaller/cheaper = gyrocopter
- **Gyrocopter (2nd row)** -> `expansionbiggyrocopter`: larger/pricier = big gyrocopter
- **Mh-6 Little Bird** -> `expansionmh6`: non-PVP price column applied
- **UH-1H** -> `expansionuh1h`: non-PVP price column applied
- **AK-74** -> `a6_ak74`: A6 AK-74 (sheet uses 'KA-' for vanilla); vanilla ak74 entry left untouched
- **AR-15 SBR** -> `a6_ar15_carbine`: SBR = carbine version
- **M16** -> `a6_m16`: M16A2/A3/A4 entries left unpriced (no rows)
- **Brass Knuckles** -> `brassknuckles_shiny`: parent; dull variant shares
- **Desert Eagle** -> `a6_deserteagle`: vanilla deagle_gold entry left untouched
- **Glock-19** -> `a6_glock19`: A6 version priced; bare 'glock19' entry in Pistols.json left untouched (ambiguous)
- **M9 Beretta** -> `a6_m9`: a6_m9_desert separate entry left unpriced
- **PP-19 Vityaz** -> `a6_pp19`: a6_pp19 = Vityaz (Bizon is a6_pp19bizon)
- **AK Wooden Buttstock** -> `a6_akm_buttstock`: AKM wooden stock; judgment call
- **MOE Carbine Mil-Spec** -> `a6_carbine_buttstock`: carbine (MOE mil-spec) stock; judgment call
- **Mossberg Magpul Buttstock** -> `a6_mossberg_buttstock_plastic_2`: plastic_2 assumed = Magpul SGA; judgment call
- **Scorpion Ever Ergo Buttstock** -> `a6_scorpion_evo_tactical_buttstock`: 'Ergo' mapped to tactical stock; judgment call
- **AN/PEQ-15 Laser** -> `a6_anpeq15`, `a6_anpeq15_tan`: tan colorway entry given same price
- **Holosun LS321G Laser** -> `a6_ls321g_black`, `a6_ls321g_vanilla`: both colorway entries priced from single row
- **Pistol Flashlight** -> `a6_foxtrot_tan_pistollight`: tan Foxtrot pistol light (same price as FOXTROT2R row); judgment call
- **BCM Gunfighter Vertical Grip Mod3** -> `a6_bcmgfvg_grip`: tan twin entry in Weapons_Grips left unpriced
- **AK RIS Polymer Handguard** -> `a6_ak_handguard_tactical_polymer_tan`: tactical polymer handguard; judgment call
- **AK Wooden Handguard** -> `a6_ak_handguard_akm`: AKM wooden handguard; judgment call
- **DD M4A1 RIS II** -> `a6_m4a1_handguard_dd125_tan`: 12.5in RIS II parent (tan, black is Variant)
- **DD MK18 RIS II** -> `a6_m4a1_handguard_dd95_black`: 9.5in RIS II (MK18)
- **M16A3 Handguard (2nd row)** -> `a6_m16_a4_handguard`: second 'M16A3' row assumed to be the M16A4 handguard; judgment call
- **M4A1 Carbin Handguard** -> `a6_m4a1_handguard_plastic`: standard plastic carbine handguard
- **M4A1 Magpul MOE SL Handguard** -> `a6_m4a1_handguard_mp`: 'mp' = Magpul handguard
- **MPS Forearm** -> `a6_mp5_forearm`: 'MPS' = MP5 base forearm typo
- **PK B-50 Handguard** -> `a6_pk_handguard`: PK machine-gun B-50 handguard
- **607 Flash Hider** -> `a6_car15_flashhider`: Colt 607 moderator = CAR-15 flash hider; judgment call
- **AK Muzzle Brake** -> `a6_ak74_muzzle`: AK-74 style brake; judgment call
- **AK Muzzle Brake/Flash Hider** -> `a6_akm_muzzle`: AKM brake/FH; judgment call
- **ASK Muzzle Break/Flash Hider** -> `a6_aks74u_muzzle`: 'ASK' = AKS-74U typo
- **FAL Belgium Flash Hider** -> `a6_fal_muzzle`: fal_muzzle/_2 pairing by row order; judgment call
- **G1 FAL Flash Hider** -> `a6_fal_muzzle_2`: judgment call
- **PRK Flash Hider** -> `a6_rpk_muzzle`: 'PRK' = RPK typo
- **Dovetail Scope Mount** -> `a6_optic_mount9a91`: 9A-91 dovetail mount; all mounts share 1k/2k so no price risk
- **ToopMount Optic Mount** -> `a6_optic_mountsvd`: 'TopMount' = SVD top mount; same 1k/2k as all mounts
- **Docter Signt Gen 3** -> `a6_docteroptic`: docteroptic_mrdmount combo entry left unpriced
- **Trijicon ACOG 4x32** -> `a6_acogoptic_black`: A6_AcogOptic_Tan separate entry left unpriced
- **Trijicon RMR Type 2** -> `a6_rmroptic_black`: mrdmount combo entries left unpriced
- **20rd Standardized Mag** -> `A6_Mag_Stanag_20Rnd`: A6 STANAG 20rd
- **30rd Standardized Mag** -> `A6_Mag_Stanag_30Rnd`: first of two '30rd Standardized' rows applied to the A6 30rd STANAG
- **AK-74 Magazine** -> `A6_Mag_AK74_30Rnd`: x3 rows at ascending prices = 30/45/60rd
- **FAL Magazine** -> `A6_Mag_FAL_20Rnd`: two rows ascending = 20/30rd
- **HK MP5 Magazine** -> `A6_Mag_MP5_15Rnd`: two rows; ordered by purchase price = 15/30rd (sheet sell prices look garbled, applied as printed)
- **HK MP5 Magazine (2nd row)** -> `A6_Mag_MP5_30Rnd`: as printed
- **M14 Magazine** -> `A6_Mag_M14_10Rnd`: two rows ascending = 10/20rd
- **MP7A2 Magazine** -> `A6_Mag_MP7A2_20Rnd`: two rows; ordered by purchase price = 20/40rd (sell prices garbled on sheet, applied as printed)
- **MP7A2 Magazine (2nd row)** -> `A6_Mag_MP7A2_40Rnd`: as printed
- **SCAR-H Magazine** -> `A6_Mag_SCARH_20Rnd`: tan mag separate entry left unpriced
- **Scorpion EVO 3 A1 PMAG** -> `A6_Mag_Scorpion_Evo_40Rnd`: PMAG row mapped to the 40rd Evo mag; judgment call
- **SR-3 Magazine** -> `A6_Mag_SR3_30Rnd`: red variant separate entry left unpriced
- **SVD Magazine** -> `A6_Mag_SVD_10Rnd`: two rows ascending = 10/20rd
- **Vector/Glock Drum Mag** -> `A6_Mag_Vector_50Rnd`: two rows ascending price = 33/50rd; this is the pricier 50rd
- **Vector/Glock Drum Magazine** -> `A6_Mag_Vector_33Rnd`: cheaper row = 33rd
- **Vikhr Magazine** -> `A6_Mag_Vikhr_30Rnd`: red variant separate entry left unpriced
- **VSS Vintorez Magazine** -> `A6_Mag_VSS_10Rnd`: red variant separate entry left unpriced
- **Ghillie Cloak** -> `ghillietop_tan`: ghillie top = cloak; judgment call
- **Ghillie Shrug** -> `ghilliebushrag_tan`: bushrag = shrug
- **Ballistic Helmet** -> `ballistichelmet_navy`: parent; colors are Variants
- **Combat Helmet** -> `mich2001helmet`: vanilla Mich2001 display = Combat Helmet
- **EARMOR M32 Headset** -> `fog_m32_headset_black`: headset item (fog_m32_helmet_black left unpriced)
- **Enduro Helmet Mouthgaurd** -> `dirtbikehelmet_mouthguard`: visor/chernarus dirtbike entries left unpriced (no rows)
- **Fast Bump Helmet** -> `fog_helmet_fast_bump_black`, `FOG_Helmet_FAST_Bump`: both FAST Bump entries (Helmets + BM_Armor) given the row price
- **Fast MT Super HighCut Helmet** -> `fog_helmet_fastmt_aor1`: FAST MT; judgment call
- **Flight Helmet** -> `zsh3pilothelmet`: ZSh-3 pilot helmet; black version separate entry left unpriced
- **Hard Hat** -> `constructionhelmet_blue`: parent; colors are Variants
- **Motorbike Helmet** -> `motohelmet_lime`, `motohelmet_black`: first row -> regular moto helmet parents (both color families)
- **Motorbike Helmet (2nd row)** -> `darkmotohelmet_lime`, `darkmotohelmet_black`, `darkmotohelmet_yellowscarred`: second row -> dark moto helmet family; judgment call
- **Hight Capacity Vest (A-TACS)** -> `highcapacityvest_black`: vanilla high-cap vest parent (A-TACS colorway not present); judgment call
- **Plate Carrier** -> `platecarriervest`: parent; green/black are Variants
- **Plate Carrier (A-TACS)** -> `platecarriervest_camo`: camo plate carrier; judgment call
- **S&S Plateframe Carrier** -> `fog_vest_ssmk4_black`: SS MK4 = S&S Precision PlateFrame MK4
- **Stab Vest** -> `chestplate`: vanilla ChestPlate display = Stab Vest
- **Wartech LBSTV110 Plate Carrier** -> `fog_vest_tv110t_au`, `fog_vest_tv110t_black`, `fog_vest_tv110t_fg`: all three TV110 colorway entries priced from single row
- **GameBoy Cartridge** -> `Hive_Retro_GBC_Alice_In_Wonderland`: only GBC cartridge in market files; judgment call
- **Ecstasy Brick** -> `iM7s_Brick_Thizz_Blue`: four identical Ecstasy rows mapped to the four Thizz colors (same price)
- **Regenerative Stim** -> `ams_health_stim`: AMS health stim = regenerative stim
- **Military Hacking Device** -> `a6_hacking_tablet`: A6 hacking tablet; judgment call
- **AVON M53A1 Gasmask** -> `fog_avonm53_black`: clear-lens version separate entry left unpriced
- **PO-X Antidote** -> `anticheminjector`: vanilla AntiChemInjector = PO-X antidote
- **Old Sleeping Bag** -> `rag_baseitems_sleepingbag_packed_ckeckered_blue`: packed checkered bag assumed = 'Old'; judgment call (unpacked entries left unpriced)
- **Sleeping Bag** -> `rag_baseitems_sleepingbag_packed_rag_edition`: packed RaG-edition bag; judgment call
- **AX-50** -> `a6_ax`: a6_ax assumed to be the AX-50; judgment call
- **Bergen Rucksack** -> `fog_bergen_rucksack_aor1`, `fog_bergen_rucksack_erdl`: both Bergen parent entries priced from single row
- **SSO Attack 2 Raid Bag** -> `fog_bag_sso_black`: sso_toxic separate entry left unpriced
- **BRS PARA Buttstock** -> `a6_sa58_brs_buttstock`: SA58 BRS PARA stock; judgment call
- **Fortis Shift Tactical Foregrip** -> `a6_fortis_shift_grip`: short version separate entry left unpriced
- **Harris Bipod** -> `a6_bipod_ris_tan`: two bipod rows paired to two bipod classes; judgment call
- **Magpul Bipod** -> `a6_bipod_rotator`: judgment call (pairing with Harris row)
- **RK-2 Grip** -> `a6_rk2_grip`: rk0/rk6 left unpriced (no rows)
- **Troy Grip** -> `a6_troy_grip`: troymini left unpriced
- **Accuracy International Tactical Muzzle Brake** -> `a6_ax_muzzle`: AI brake -> AX muzzle; a6_awr_muzzle left unpriced; judgment call
- **PK B-51 Optic Mount** -> `a6_pk_optic_mount`: tan twin entry left unpriced
- **Schmidt & Bender 5-25x56mm PM II Scope** -> `a6_schmidtbenderpmii_optic`: tan twin entry left unpriced
- **Vortex Razor AMG 6-24x50mm** -> `a6_vortexoptic`: remaining Vortex scope entry; judgment call
- **Vortex Razor HD Gen III 1-10x24mm** -> `a6_vortexrazoroptic`: judgment call
- **Tactical Dynamics Skeleton Grip** -> `a6_m4a1_pistolgrip_skeleton`: skeleton grip; judgment call vs F1 row
- **SilencerCo Chimera Silencer** -> `a6_chimera_300_suppressor`, `A6_Chimera_300__Suppressor`: two near-duplicate market entries (Muzzles.json / BM_Suppressors.json) both priced
- **SilencerCo Omega 300 Multi Caliber Silencer** -> `a6_omega_300_suppressor`, `a6_omega_300_tan`: tan colorway entry given same price
- **.408 CheyTac Magazine** -> `A6_Mag_M200_7Rnd`: tan mag separate entry left unpriced
- **60rd Standardized Mag** -> `A6_Mag_Stanag_60Rnd`: first of two 60rd rows applied
- **AX-50 Magazine** -> `A6_Mag_AX_7Rnd`: AX mag paired with AX-50 (a6_ax); judgment call
- **1-M8 Smoke Grenade** -> `m18smokegrenade_red`: M18 smoke parent; colors are Variants
- **40mm Smoke Grenade** -> `Ammo_40mm_Smoke_White`: parent; colors are Variants

## Rows matching Variants (not separately priceable)
Expansion variants always share the parent's price; these sheet rows therefore could not be applied individually. The parent got the price from its own sheet row.
- SVD Tiger (sheet 20000/40000) -> variant of `a6_svd`. A6_SVD_Tiger is a Variant of a6_svd - shares parent price, cannot be priced separately
- MP5 Folding Buttstock (sheet 425/850) -> variant of `a6_mp5_buttstock`. folding stock is a Variant of a6_mp5_buttstock
- MP5 Retractable Buttstock (sheet 500/1000) -> variant of `a6_mp5_buttstock`. retractable stock is a Variant
- MP5 Wood Buttstock (sheet 50/100) -> variant of `a6_mp5_buttstock`. wood stock is a Variant (same price as parent anyway)
- Mossberg 590 Magpul Forearm (sheet 375/750) -> variant of `a6_mossberg_forearm_plastic`. plastic_2 forearm is a Variant of the plastic forearm
- MP5 RIS Forearm (sheet 200/400) -> variant of `a6_mp5_forearm`. Variant of MP5 forearm
- MP5 Slimline Forearm (sheet 350/700) -> variant of `a6_mp5_forearm`. Variant of MP5 forearm
- MP5 Vented Forearm (sheet 300/600) -> variant of `a6_mp5_forearm`. Variant of MP5 forearm
- MP5 Wood Forearm (sheet 150/300) -> variant of `a6_mp5_forearm`. Variant of MP5 forearm
- MP5k Proto Forearm (sheet 237/475) -> variant of `a6_mp5k_forearm`. Variant of MP5k forearm
- MP5k SP Forearm (sheet 250/500) -> variant of `a6_mp5k_forearm`. Variant of MP5k forearm
- .308 WIN Tracer (sheet 42/65) -> variant of `Ammo_308Win`. Ammo_308WinTracer is a Variant
- 12ga 00 Buckshots (sheet 9/15) -> variant of `Ammo_12gaSlug`. Ammo_12gaPellets is a Variant of the slug entry
- 12ga Rubber Slugs (sheet 2/4) -> variant of `Ammo_12gaSlug`. Ammo_12gaRubberSlug is a Variant
- 5.45x39mm FMJ (sheet 12/25) -> variant of `Ammo_545x39`. FMJ is a Variant of Ammo_545x39
- 5.45x39mm HP (sheet 10/20) -> variant of `Ammo_545x39`. HP is a Variant
- 5.45x39mm PP gs (sheet 20/40) -> variant of `Ammo_545x39`. PPgs is a Variant
- 5.45x39mm PS gs (sheet 17/35) -> variant of `Ammo_545x39`. PSgs is a Variant
- 5.45x39mm T gs (sheet 12/25) -> variant of `Ammo_545x39`. Tgs is a Variant
- 5.45x39mm Tracer (sheet 16/25) -> variant of `Ammo_545x39`. Tracer is a Variant
- 5.56x45mm FMJ (sheet 22/45) -> variant of `Ammo_556x45`. FMJ is a Variant of Ammo_556x45
- 5.56x45mm HP (sheet 20/40) -> variant of `Ammo_556x45`. HP is a Variant
- 5.56x45mm M856 (sheet 17/35) -> variant of `Ammo_556x45`. M856 is a Variant
- 7.62x39mm FMJ (sheet 17/35) -> variant of `Ammo_762x39`. FMJ is a Variant of Ammo_762x39
- 7.62x39mm HP (sheet 15/30) -> variant of `Ammo_762x39`. HP is a Variant
- 7.62x39mm Tracer (sheet 26/40) -> variant of `Ammo_762x51M80`. oddly stored as a Variant of Ammo_762x51M80 in AmmoT_Ammunitions.json
- 7.62x51mm BCP FMJ (sheet 37/75) -> variant of `Ammo_762x51M80`. BCP FMJ is a Variant of the M80 entry
- 7.62x54 R FMJ (sheet 50/100) -> variant of `Ammo_762x54`. FMJ is a Variant of Ammo_762x54
- 7.62x54mm R HP BT (sheet 35/70) -> variant of `Ammo_762x54`. HPBT is a Variant
- 7.62x54mmR Tracer (sheet 45/70) -> variant of `Ammo_762x54`. Tracer is a Variant
- 5.56x45mm M855 (sheet 33/67) -> variant of `Ammo_556x45`. M855 is a Variant of Ammo_556x45 (parent priced from plain 5.56 row)
- 7.62x39mm PP gzh (sheet 22/45) -> variant of `Ammo_762x39`. PPgzh is a Variant
- 7.62x39mm PS gzh (sheet 20/40) -> variant of `Ammo_762x39`. PSgzh is a Variant
- 7.62x39mm T-45M1 gzh (sheet 20/40) -> variant of `Ammo_762x39`. T45M1gzh is a Variant
- 7.62x51mm M62 Tracer (sheet 75/150) -> variant of `Ammo_762x51M80`. M62Tracer is a Variant
- 7.62x54mm R LPS gzh (sheet 62/125) -> variant of `Ammo_762x54`. LPSgzh is a Variant
- 7.62x54mm R PS gzh (sheet 62/125) -> variant of `Ammo_762x54`. PSgzh is a Variant
- 7.62x54mm R T-46M gzh (sheet 55/110) -> variant of `Ammo_762x54`. T46Mgzh is a Variant

## Duplicate / skipped rows
- Disinfectant Spray (2nd row): second sheet row for one market item; first row applied
- 9v Battery (2nd row): second sheet row for one market item; first row applied
- Garden Lime (2nd row): second sheet row for one market item; first row applied
- Pliers (2nd row): second sheet row for one market item; first row applied
- Spark Plug (boat trader): duplicate of aircraft-trader Spark Plug row (same price)
- Jerrycan (aircraft trader): duplicate of hardware Jerrycan row (same price)
- AW101 MerlinHC3 BAF [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- AW159 Wildcat [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- CH-47F Chinook [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- Gyrocopter [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- Gyrocopter (2nd) [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- Kamov KA-60 Kasatka [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- Mh-6 Little Bird [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- MH-6 WIP Little Bird [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- Mi-8 [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- MI24 [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- UH-1H [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- UH-60M Black Hawk [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- UH1Y Venom [PVP-servers column]: alternate PVP-server price column - not applied (regular column used)
- Bolt (2nd listing): listed twice on sheet with identical prices
- MAGAZINES list repeated on BM Armory Supplies pages: ~105 magazine rows repeat verbatim (same prices) on pages 43-44 - deduplicated
- LOOSE AMMUNITION list repeated on BM Armory Supplies page: ~46 loose-ammo rows repeat verbatim (same prices) on page 43 - deduplicated
- Portable Gas Stove (BM row): same class as 'Gas stove' cooking row (portablegasstove) - regular trader price applied, BM 9k/9k row skipped
- Gas Mask (2nd row): second Gas Mask row - one vanilla gasmask class; first row applied

## Unmatched PDF rows
| Sheet row | Sell | Purchase | Reason |
|---|---|---|---|
| IV Saline Bag | 1150 | 2300 | SalineBagIV not in any market file |
| Quick Application Tourniquet | 750 | 1500 | no tourniquet item in market files |
| ZynCan 6MG | 75 | 150 | no matching item in market files |
| Absolute Vodka | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Backwoods 3 Pack Honey Berry | 500 | 1000 | liquor/smokes mod items not present in market files |
| Baileys Irish Creme | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Ballenties Blended Scotch Wiskey | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Becherovka | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Beefeater Gin | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Blue Curacao Liqueur | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Brugal Anejo Rum | 1050 | 2100 | liquor/smokes mod items not present in market files |
| Brugal Doble Reserva Rum | 1100 | 2200 | liquor/smokes mod items not present in market files |
| Chivas Regal 12 | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Cointreau Liqueur | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Colt 45 Malt Liquor | 825 | 1650 | liquor/smokes mod items not present in market files |
| Colt 45 Malt Liquor (2nd row) | 900 | 1800 | liquor/smokes mod items not present in market files |
| CremaDiLimoncino Liqueur | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Glass Bong | 2100 | 4200 | liquor/smokes mod items not present in market files |
| Havana Club Rum | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Hennessy VS Cognac | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Jack Daniels Whiskey | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Jaggermeister | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Jameson Irish Whiskey | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Khalua Coffee Liqueur | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Klasztorny Mead | 950 | 1900 | liquor/smokes mod items not present in market files |
| Magnum Malt Liquor | 900 | 1800 | liquor/smokes mod items not present in market files |
| Malibu Rum | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Martel VS Fine Cagnac | 1000 | 2000 | liquor/smokes mod items not present in market files |
| MD 20/20 Banana Red | 875 | 1750 | liquor/smokes mod items not present in market files |
| MD 20/20 Blue Raspberry | 875 | 1750 | liquor/smokes mod items not present in market files |
| MD 20/20 Coco Loco | 875 | 1750 | liquor/smokes mod items not present in market files |
| MD 20/20 Dragon Fruit | 875 | 1750 | liquor/smokes mod items not present in market files |
| MD 20/20 Electric Melon | 875 | 1750 | liquor/smokes mod items not present in market files |
| MD 20/20 Golden Pineapple | 875 | 1750 | liquor/smokes mod items not present in market files |
| MD 20/20 Habanero Lime-arita | 900 | 1800 | liquor/smokes mod items not present in market files |
| MD 20/20 Kiwi Lemon | 875 | 1750 | liquor/smokes mod items not present in market files |
| MD 20/20 Orange Jubilee | 875 | 1750 | liquor/smokes mod items not present in market files |
| MD 20/20 Peaches & Creme | 875 | 1750 | liquor/smokes mod items not present in market files |
| MD 20/20 Red Grape Wine | 925 | 1850 | liquor/smokes mod items not present in market files |
| Mickeys Malt Liquor | 800 | 1600 | liquor/smokes mod items not present in market files |
| Molinari Sambuca | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Olde English Malt Liquor | 800 | 1600 | liquor/smokes mod items not present in market files |
| Olmeca Tequila | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Pernod Absinthe | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Private Stock Malt Liquor | 850 | 1700 | liquor/smokes mod items not present in market files |
| Ramazzotti Liqueur | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Schlitz Malt Liquor | 800 | 1600 | liquor/smokes mod items not present in market files |
| Single Backwood | 225 | 450 | liquor/smokes mod items not present in market files |
| Sortilege Whisky | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Suntory Whisky | 1000 | 2000 | liquor/smokes mod items not present in market files |
| White Horse Whisky | 1000 | 2000 | liquor/smokes mod items not present in market files |
| Bone Fishing Hook | 35 | 70 | vanilla BoneHook not in market files |
| Simple Fishing Rod | 25 | 50 | improvised rod not in market files |
| Wooden Fishing Hook | 25 | 50 | vanilla WoodenHook not in market files |
| American Lobster Claw | 225 | Not Purchasable | only whole geb_AmericanLobster exists; claw/tail ambiguous |
| American Lobster Tail | 315 | Not Purchasable | only whole geb_AmericanLobster exists; claw/tail ambiguous |
| Atlantic Sailfish Fillet | 2310 | Not Purchasable | no sailfish item in market files |
| Human Steak | 100 | Not Purchasable | HumanSteakMeat not in market files |
| Skinned Rat | 400 | Not Purchasable | no rat meat item in market files |
| Skinned Squirrel | 700 | Not Purchasable | no squirrel item in market files |
| Bear Pelt | 15000 | Not Purchasable | pelt items not present in market files |
| Boar Pelt | 5000 | Not Purchasable | pelt items not present in market files |
| Cow Pelt | 9000 | Not Purchasable | pelt items not present in market files |
| Crocodile Skin (White) | 20000 | Not Purchasable | pelt items not present in market files |
| Crocodile Skin (Green) | 15000 | Not Purchasable | pelt items not present in market files |
| Crocodile Skin Parts (White) | 20000 | Not Purchasable | pelt items not present in market files |
| Crocodile Skin Parts (Green) | 15000 | Not Purchasable | pelt items not present in market files |
| Deer Pelt | 8000 | Not Purchasable | pelt items not present in market files |
| Fox Pelt | 3000 | Not Purchasable | pelt items not present in market files |
| Goat Pelt Black | 7000 | Not Purchasable | pelt items not present in market files |
| Goat Pelt White | 7500 | Not Purchasable | pelt items not present in market files |
| Hare Pelt | 1000 | Not Purchasable | pelt items not present in market files |
| Otter Pelt | 2000 | Not Purchasable | pelt items not present in market files |
| Pig Pelt | 5000 | Not Purchasable | pelt items not present in market files |
| Reindeer Pelt | 8800 | Not Purchasable | pelt items not present in market files |
| Sheep Pelt | 7000 | Not Purchasable | pelt items not present in market files |
| Wolf Pelt Grey | 8500 | Not Purchasable | pelt items not present in market files |
| Wolf Pelt White | 9000 | Not Purchasable | pelt items not present in market files |
| Bear Head | 5000 | Not Purchasable | trophy head items not present in market files |
| Bull Head | 1500 | Not Purchasable | trophy head items not present in market files |
| Bull Head (2nd) | 1000 | Not Purchasable | trophy head items not present in market files |
| Bull Head (3rd) | 1000 | Not Purchasable | trophy head items not present in market files |
| Crocodile Teeth | 1500 | Not Purchasable | trophy head items not present in market files |
| Deer Head | 3000 | Not Purchasable | trophy head items not present in market files |
| Deer Head (2nd) | 2700 | Not Purchasable | trophy head items not present in market files |
| Fox Head | 1000 | Not Purchasable | trophy head items not present in market files |
| Goat Head | 1500 | Not Purchasable | trophy head items not present in market files |
| Goat Head (2nd) | 1500 | Not Purchasable | trophy head items not present in market files |
| Goat Head (3rd) | 2500 | Not Purchasable | trophy head items not present in market files |
| Pig Head | 1500 | Not Purchasable | trophy head items not present in market files |
| Ram Head | 3000 | Not Purchasable | trophy head items not present in market files |
| Reindeer Head | 4000 | Not Purchasable | trophy head items not present in market files |
| Wildboar Head | 1500 | Not Purchasable | trophy head items not present in market files |
| Wolf Head Grey | 2000 | Not Purchasable | trophy head items not present in market files |
| Wolf Head White | 2500 | Not Purchasable | trophy head items not present in market files |
| Pepe Toy | 2500 | 5000 | no plush/toy pepe item in market (wallpicture_pepe is a painting) |
| All Books | 750 | Not Purchasable | book items not present in market files |
| Bible | 10000 | Not Purchasable | book items not present in market files |
| Harris Falcon III | 125 | 250 | radio mod item not in market files |
| Military Battery | 400 | 800 | no matching battery item (truckbattery/carbattery have no sheet row) |
| Motorola T80 | 225 | 450 | radio mod item not in market files |
| Walkie-Talkie | 125 | 250 | personalradio already matched by Handheld Transceiver row |
| Rangefinder | 375 | 750 | vanilla Rangefinder not in market files |
| Binoculars | 100 | 200 | vanilla Binoculars not in market files |
| Firewood | 17 | 35 | vanilla Firewood not in market files |
| Toilet Paper | 300 | 600 | no toilet paper item in market files |
| BBP Tape Measure | 450 | 900 | BBP items not in market files |
| Brick Panel Wallpaper 001 | 6250 | 12500 | BBP wallpaper items not in market files |
| Concrete Panel Wallpaper 001 | 6250 | 12500 | BBP wallpaper items not in market files |
| Flooring Carpet 1 | 6250 | 12500 | BBP flooring items not in market files |
| Flooring Solid 1 | 6250 | 12500 | BBP flooring items not in market files |
| Flooring Tile 1 | 6250 | 12500 | BBP flooring items not in market files |
| Flooring Wood 1 | 6250 | 12500 | BBP flooring items not in market files |
| Insecticide Spray | 400 | 800 | no insecticide item in market files |
| Insecticide Spray (2nd row) | 450 | 900 | no insecticide item in market files |
| Mando | 6250 | 12500 | BBP skin item not in market files |
| Metal Panel Wallpaper 001 | 6250 | 12500 | BBP wallpaper items not in market files |
| Nature's Herbs | 6250 | 12500 | BBP skin item not in market files |
| Plaster 1 | 6250 | 12500 | BBP flooring items not in market files |
| Tanned Leather | 2 | 4 | TannedLeather not in market files |
| Tarp | 75 | 150 | no tarp item in market files (Supplies 'fabric' left untouched - uncertain) |
| Wood Panel Wallpeper 001 | 6250 | 12500 | BBP wallpaper items not in market files |
| Wooden Log | 125 | 250 | vanilla WoodenLog not in market files |
| Repaint Workbench | 0 | 20000 | no repaint workbench item in market files |
| 12 Inch Gold Dildo. by Arrakis | 7500 | 15000 | adult-item mod not present in market files |
| Condom | 500 | 1000 | adult-item mod not present in market files |
| Dildo Repair Lube | 4000 | 8000 | adult-item mod not present in market files |
| Exploding Blue Butt Plug. by Arrakis | 6000 | 12000 | adult-item mod not present in market files |
| Fleshlight | 12500 | 25000 | adult-item mod not present in market files |
| Soap on a Rope. by Arrakis | 6000 | 12000 | adult-item mod not present in market files |
| Bolt Cutters | 250 | 500 | no bolt cutters item in market files |
| Tools Repair Kit | 237 | 475 | no tool repair kit item in market files |
| WD40 | 300 | 600 | WD40 item not in market files |
| WD40 (2nd row) | 500 | 1000 | WD40 item not in market files |
| CanAm Maverick Orange | 81500 | 168850 | vehicle mod not in market files |
| HarleyDavidson FayBoy - Black | 86000 | 179950 | vehicle mod not in market files |
| Honda CRF450R - Red | 52500 | 111850 | vehicle mod not in market files |
| Honda CRF450R - Red Dirty | 42500 | 91850 | vehicle mod not in market files |
| Kawasaki Mule SX 4x4 | 98500 | 199360 | vehicle mod not in market files |
| Suzuki GSX-R750 - Yellow | 110000 | 223250 | vehicle mod not in market files |
| Yamah YZF 450 ATV - Blue | 65000 | 134050 | vehicle mod not in market files |
| Acura Integra White | 117500 | 244110 | vehicle mod not in market files |
| BelAir | 189500 | 395110 | vehicle mod not in market files |
| Chevrolet_NapalmNova_Black | 155000 | 331110 | vehicle mod not in market files |
| Dodge Charger Hellcat (Black) | 175000 | 376110 | vehicle mod not in market files |
| DodgeChallenger SRT (White) | 157500 | 336110 | vehicle mod not in market files |
| Ford_Mustang_Shelby_GT500_White | 200000 | 421110 | two Mustang rows but only one 'mustang' family - ambiguous, left unpriced |
| FordMustang SVT White | 137500 | 296110 | two Mustang rows but only one 'mustang' family - ambiguous, left unpriced |
| HondaCivic White | 132500 | 286110 | vehicle mod not in market files |
| Mazda RX-8 | 107500 | 230110 | vehicle mod not in market files |
| Mitsubishi_LancerEvolutionIX White | 180000 | 386100 | vehicle mod not in market files |
| mkivota SupraMK4 (White) | 167500 | 356110 | vehicle mod not in market files |
| Nissan 180 SX | 140000 | 295110 | vehicle mod not in market files |
| Nissan_Skyline (White) | 152500 | 326110 | vehicle mod not in market files |
| NissanGTRNismo White | 202500 | 426110 | vehicle mod not in market files |
| NSX | 170000 | 355110 | vehicle mod not in market files |
| Porsche_911RWB (White) | 190000 | 401110 | vehicle mod not in market files |
| Tourist Bus | 275000 | 570110 | ambiguous (PAZ candidates); left unpriced |
| VW Golf MK1 | 120000 | 256110 | vehicle mod not in market files |
| VW_T6 | 175000 | 370110 | vehicle mod not in market files |
| DodgeRam_2021_White | 220000 | 471110 | vehicle mod not in market files |
| DodgeRam_3500 (White) | 258500 | 528110 | vehicle mod not in market files |
| DodgeRam_3500 (White) (2nd row) | 287500 | 585110 | vehicle mod not in market files |
| Ford_Bronco (Blue) | 129500 | 282620 | vehicle mod not in market files |
| Ford_Raptor_MonsterTruck USA | 400000 | 821110 | monster truck variant not identifiable in market files |
| GMC (Red) | 180000 | 383610 | vehicle mod not in market files |
| Jeep_GladiatorF9_White | 310000 | 666110 | vehicle mod not in market files |
| Jeep_Rubicon Black | 320000 | 671110 | jeep_wrangler exists but Rubicon likely a different mod class - not applied |
| Tahoe_White | 207500 | 441120 | vehicle mod not in market files |
| Toyota 4runner | 237500 | 503620 | vehicle mod not in market files |
| Kamaz_TyphoonK_CamoGreen | 425000 | 901110 | two Typhoon families in market (typhoon / kamaz_typhoon) - ambiguous |
| M1025 | 161000 | 328460 | military humvee not in market files (hammer_h1 is the civilian H1) |
| MAN HX58 British | 340000 | 698110 | vehicle mod not in market files |
| UAZ 469 Cargo | 96500 | 198660 | no clear cargo-UAZ class in market files |
| CarLock | 0 | 2000 | Expansion CarLock item not in market files |
| Garage Kit | 50000 | 100000 | virtual garage kit item not in market files |
| Airboat | 30000 | 60750 | boat mod not in market files |
| Jetski Green | 30000 | 60750 | boat mod not in market files |
| RHIB | 30000 | 60750 | boat mod not in market files |
| Rubber Boat | 30000 | 60750 | Zodiac parent entry in Veh_Sea.json has malformed ClassName 'ExpansionZodiacBoat (Colors as variants)' - not priced |
| AW159 Back Wheel | 250 | 500 | AW159 parts not in market files |
| AW159 Front Wheel | 250 | 500 | AW159 parts not in market files |
| KA60 Back Wheel | 250 | 500 | KA60 parts not in market files |
| KA60 Cargo Door | 250 | 500 | KA60 parts not in market files |
| KA60 Front Wheel | 250 | 500 | KA60 parts not in market files |
| MerlinHC3 Cargo Bay Door | 250 | 500 | Merlin doors not in market files |
| MerlinHC3 Door | 250 | 500 | Merlin doors not in market files |
| MerlinHC3 Door (2nd row) | 250 | 500 | Merlin doors not in market files |
| MI 24 - Back Wheel | 250 | 500 | Mi-24 parts not in market files |
| MI 24 - Back Wheel (2nd) | 250 | 500 | Mi-24 parts not in market files |
| MI 24 - Front Wheel | 250 | 500 | Mi-24 parts not in market files |
| MI 24 - Front Wheel (2nd) | 250 | 500 | Mi-24 parts not in market files |
| MI 24 Door | 250 | 500 | Mi-24 parts not in market files |
| MI 24 Door (2nd) | 250 | 500 | Mi-24 parts not in market files |
| MI-24 Cargo Door | 250 | 500 | Mi-24 parts not in market files |
| MI-24 Cargo Door (2nd) | 250 | 500 | Mi-24 parts not in market files |
| Mi-8 Back Wheel | 250 | 500 | Mi-8 parts not in market files |
| MI-8 Door | 250 | 500 | Mi-8 parts not in market files |
| MI-8 Front Wheel | 250 | 500 | Mi-8 parts not in market files |
| UH1Y CoPilot Door | 250 | 500 | UH1Y parts not in market files |
| UH1Y Pilot Door | 250 | 500 | UH1Y parts not in market files |
| UH60M CoPilot Door | 250 | 500 | UH60M parts not in market files |
| UH60M Left Cargo Door | 250 | 500 | UH60M parts not in market files |
| UH60M Pilot Door | 250 | 500 | UH60M parts not in market files |
| UH60M Right Cargo Door | 250 | 500 | UH60M parts not in market files |
| AW159 Wildcat | 650000 | 1310000 | helicopter mod not in market files |
| CH-47F Chinook | 1000000 | 2010000 | helicopter mod not in market files |
| Kamov KA-60 Kasatka | 1060000 | 2130000 | helicopter mod not in market files |
| MH-6 WIP Little Bird | 570000 | 1150000 | WIP variant not in market files |
| Mi-8 | 620000 | 1250000 | helicopter mod not in market files |
| MI24 | 750000 | 1510000 | helicopter mod not in market files |
| UH-60M Black Hawk | 700000 | 1410000 | helicopter mod not in market files |
| UH1Y Venom | 700000 | 1410000 | helicopter mod not in market files |
| Fabric Parachute | 1500 | 10000 | parachute mod not in market files |
| Fabric Parachute (2nd row) | 3000 | 20000 | parachute mod not in market files |
| Tactical Parachute | 4500 | 30000 | parachute mod not in market files |
| Tactical Parachute (2nd row) | 7500 | 50000 | parachute mod not in market files |
| BK-43 | 250 | 500 | vanilla double-barrel not in market files |
| R12 | 1000 | 2000 | no matching shotgun classname identified |
| DownRange Tomahawk | 650 | 1300 | tomahawk not in market files |
| Nailed Bat | 95 | 190 | nailed bat not in market files |
| Stun Baton | 87 | 175 | stun baton not in market files |
| BK-18 | 625 | 1250 | vanilla single-shot rifle not in market files |
| Blaze | 1500 | 3000 | vanilla Blaze not in market files |
| CR-527 | 1950 | 3900 | vanilla CR-527 not in market files |
| CR-550 Savanna | 3250 | 6500 | vanilla CR-550 not in market files |
| DMR | 3100 | 6200 | no DMR classname in market files |
| M70 Tundra | 2150 | 4300 | vanilla M70 Tundra not in market files |
| Pioneer | 2000 | 4000 | vanilla Pioneer not in market files |
| Repeater Carbine | 1500 | 3000 | vanilla Repeater not in market files |
| SK 59/66 | 2250 | 4500 | vanilla SK 59/66 not in market files |
| Sporter 22 | 1450 | 2900 | vanilla Sporter 22 not in market files |
| CR-75 | 312 | 625 | vanilla CR-75 not in market files |
| Derringer | 250 | 500 | vanilla Derringer not in market files |
| Engraved Kolt 1911 | 500 | 1000 | vanilla Engraved 1911 not in market files |
| FX-45 | 337 | 675 | vanilla FX-45 not in market files |
| IJ-70 | 100 | 200 | vanilla IJ-70 not in market files |
| Longhorn | 2000 | 4000 | vanilla LongHorn not in market files |
| Signal Pistol | 300 | 600 | vanilla flare gun not in market files |
| CR-61 Skorpion | 500 | 1000 | vanilla CR-61 not in market files |
| SSG 82 | 3000 | 6000 | SSG 82 rifle not in market files |
| Universal Flashlight | 75 | 900 | vanilla UniversalLight not in market files |
| Zentico 6P29 Picatini Handguard | 625 | 1250 | no matching handguard classname in market files |
| Mosin 91/30 Compensator | 600 | 1200 | vanilla Mosin compensator not in market files |
| SG5-K Compensator | 400 | 800 | vanilla MP5-K compensator not in market files |
| 1PN51 Scope | 1800 | 4350 | vanilla 1PN51 night scope not in market files |
| ATOG 4.32 Scope | 1600 | 3200 | vanilla ATOG 4x32 not in market files (A6 ACOG matched by Trijicon row) |
| ATOG 6x48 Scope | 2250 | 4500 | vanilla ATOG 6x48 not in market files |
| Backup Iron Sights | 75 | 150 | vanilla BUIS not in market files |
| Baraka Sights | 150 | 1050 | no matching sight classname in market files |
| Combat Sights | 750 | 2250 | no matching sight classname in market files |
| Handgun Scope | 500 | 1000 | vanilla pistol scope not in market files |
| Hunting Scope | 3000 | 6000 | vanilla hunting scope not in market files |
| Kobra Sights | 800 | 2350 | vanilla Kobra not in market files |
| Mini Sights | 125 | 1000 | no matching sight classname in market files |
| P1-87-L Scope | 750 | 1500 | vanilla P1-87-L not in market files |
| PSO-1 Scope | 1550 | 3850 | vanilla PSO-1 not in market files |
| PSO-1-1 Scope | 1700 | 4150 | vanilla PSO-1-1 not in market files |
| PU Scope | 950 | 1900 | vanilla PU scope not in market files |
| RVN Sights | 750 | 2250 | no matching sight classname in market files |
| Sportpoint Sights | 1100 | 2200 | vanilla Sportpoint not in market files |
| 5.56x45mm Tracer | 19 | 30 | Ammo_556x45Tracer not in market files (not even as a Variant) |
| Boxed .22 LR | 97 | 150 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed .300 AAC Blackout | 690 | 1380 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed .308 WIN | 780 | 1200 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed .308 WIN Tracer | 844 | 1300 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed .357 | 116 | 180 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed .380 ACP | 227 | 350 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed .45 ACP | 130 | 200 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed .50 Action Express | 1120 | 2250 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed .500 S&W Magnum | 1500 | 3000 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 12ga 00 Buckshots | 97 | 150 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 12ga Rifled Slugs | 84 | 130 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 12ga Rubber Slugs | 26 | 40 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 5.45x39mm FMJ | 487 | 750 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 5.45x39mm HP | 300 | 600 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 5.45x39mm | 312 | 480 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 5.45.39mm T gs | 487 | 750 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 5.45x39mm Tracer | 325 | 500 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 5.56x45mm | 351 | 540 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 5.56x45mm Tracer | 390 | 600 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 5.7x28mm SS190 | 338 | 520 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 6.8mm Remington SPC | 800 | 1600 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 7.62x39mm | 390 | 600 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 7.62x39mm Tracer | 390 | 600 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 7.62x54mmR | 909 | 1400 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 7.62x54mmR Tracer | 909 | 1400 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 8.6 Blackout GF | 1750 | 3500 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 9x19mm AP 6.3 Rounds | 162 | 250 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 9x19mm | 243 | 375 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 9x39mm Armor-Piercing | 233 | 360 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed 9x39mm Rounds | 195 | 300 | boxed-ammo (AmmoBox_*) classes not present in market files |
| Boxed HK 4.6x30mm AP SX Rounds | 390 | 600 | boxed-ammo (AmmoBox_*) classes not present in market files |
| 10rd CR-550 Mag | 637 | 1880 | vanilla Mag_CZ550_10Rnd not in market files |
| 10rd MK II Mag | 1500 | 3030 | vanilla MK II mag not in market files |
| 10rd Sporter 22 Mag | 50 | 130 | vanilla Sporter mags not in market files |
| 10rd VSD Mag | 1420 | 3550 | vanilla VSD mag not in market files (A6 SVD mags matched by 'SVD Magazine' rows) |
| 10rd VSS Mag | 550 | 1190 | vanilla VSS mag not in market files (A6 VSS mag matched by 'VSS Vintorez Magazine' row) |
| 12rd IJ-70 Mag | 125 | 370 | vanilla mag not in market files |
| 15rd CR-75 Mag | 175 | 530 | vanilla mag not in market files |
| 15rd FX-45 Mag | 162 | 475 | vanilla mag not in market files |
| 15rd Mlock-91 Mag | 162 | 505 | vanilla mag not in market files |
| 15rd Sporter 22 Mag | 75 | 195 | vanilla mag not in market files |
| 20rd CR-61 Mag | 750 | 1700 | vanilla mag not in market files |
| 20rd Drum Vaiga Mag | 600 | 1500 | vanilla Saiga drum not in market files |
| 20rd LAR Mag | 1350 | 3900 | vanilla FAL/LAR mag not in market files (A6 FAL mags matched by 'FAL Magazine' rows) |
| 20rd Mag-C Mag | 750 | 2040 | vanilla CMAG not in market files |
| 20rd SV AL Mag | 1220 | 2630 | vanilla VAL mag not in market files (A6 VAL mag matched by 'AS-V AL Magazine' row) |
| 20rd LE-MAS Mag | 600 | 1880 | vanilla AUG mag not in market files |
| 25rd USG-45 Mag | 575 | 1400 | vanilla UMP mag not in market files (A6 UMP mag matched by 'HK UMP45 Magazine' row) |
| 30rd AUR Mag | 950 | 2710 | vanilla AUG A1 mag not in market files |
| 30rd KA-101 Mag | 1100 | 3010 | vanilla Mag_AK101_30Rnd not in market files |
| 30rd KA-74 Mag | 1300 | 3320 | vanilla Mag_AK74_30Rnd not in market files |
| 30rd KA-M Mag | 1120 | 3150 | vanilla Mag_AKM_30Rnd not in market files |
| 30rd KA-M Polymer Mag | 1190 | 3280 | vanilla polymer AKM mag not in market files |
| 30rd Mag-C Mag | 1200 | 3210 | vanilla CMAG not in market files |
| 30rd SG5-K Mag | 750 | 1860 | vanilla MP5 30rd mag not in market files |
| 30rd Sporter 22 Mag | 187 | 465 | vanilla mag not in market files |
| 30rd Standardized Mag (2nd row) | 1320 | 3460 | second 30rd Standardized row - only one A6 30rd STANAG class exists; likely the vanilla mag |
| 40rd Mag-C Mag | 1700 | 4480 | vanilla CMAG not in market files |
| 45rd KA-74 Mag | 1600 | 4280 | vanilla 45rd AK-74 mag not in market files |
| 5rd Cr-527 Mag | 850 | 1850 | vanilla mag not in market files |
| 5rd Pioneer Mag | 375 | 885 | vanilla mag not in market files |
| 5rd SSG 82 Mag | 500 | 1120 | SSG 82 mag not in market files |
| 5rd Vaiga Mag | 200 | 475 | vanilla mag not in market files |
| 64rd Bizon Mag | 225 | 1090 | vanilla 64rd Bizon mag not in market files (A6 version matched by 'PP-19 Bizon Magazine' row) |
| 7rd Kolt 1911 Mag | 150 | 370 | vanilla Mag_1911_7Rnd not in market files (A6 version matched by 'M1911 Magazine' row) |
| 8rd P1 Mag | 125 | 346 | vanilla mag not in market files |
| 8rd Vaiga Mag | 250 | 620 | vanilla mag not in market files |
| STR_CfgMagazines_Mag_CZ550_4rnd0 | 700 | 1640 | row is the CZ550 4rnd mag, but Mag_CZ550_4rnd is not in any market file |
| Ghillie Rifle WRap | 210 | 420 | GhillieAtt not in market files |
| Improvised Camo Net Poncho | 1750 | 3500 | no matching item in market files |
| Improvised Ghillie | 1500 | 3000 | no matching item in market files |
| Sniper Hood | 750 | 1500 | no matching item in market files |
| Assault Helmet | 95 | 240 | no matching helmet classname in market files |
| Assault Helmet Visor | 25 | 50 | no matching item in market files |
| MVS 6B47 - OD | 500 | 1000 | MVS mod items not in market files |
| MVS Altyn Helmet - OD | 1000 | 2000 | MVS mod items not in market files |
| MVS Altyn Visor - OD | 650 | 1300 | MVS mod items not in market files |
| MVS Combat Helmet - OD | 200 | 400 | MVS mod items not in market files |
| Tactical Helmet | 75 | 150 | no matching helmet classname in market files |
| Tactical Helmet (A-TACS) | 130 | 260 | no matching helmet classname in market files |
| Assault Vest (A-TACS) | 350 | 700 | vanilla AssaultVest not in market files |
| Ballistic Vest | 75 | 150 | vanilla BallisticVest not in market files |
| Field Vest | 125 | 250 | no field vest classname in market files |
| MVS - OD | 15000 | 30000 | MVS mod items not in market files |
| Tactical Vest | 275 | 550 | no matching vest classname in market files |
| Elysium Industries Printer Battery | 15000 | 100000 | 3D printer mod not in market files |
| Printer Table Kit | 10000 | 100000 | 3D printer mod not in market files |
| Filament Spool Holder | 25000 | 100000 | 3D printer mod not in market files |
| Bitcoin | 30000 | Not Purchasable | collectible mod items not in market files |
| Diamond Cross Necklace | 12500 | Not Purchasable | collectible mod items not in market files |
| Evidence USB | 29000 | Not Purchasable | collectible mod items not in market files |
| Floppy Disk - 1999 | 15000 | Not Purchasable | collectible mod items not in market files |
| Graphics Card | 36000 | Not Purchasable | collectible mod items not in market files |
| Intel Folder | 20000 | Not Purchasable | collectible mod items not in market files |
| Pocket Watch | 12500 | Not Purchasable | collectible mod items not in market files |
| Pokemon Card | 6250 | Not Purchasable | collectible mod items not in market files |
| Polaroid Photo | 10000 | Not Purchasable | collectible mod items not in market files |
| RAM | 17000 | Not Purchasable | collectible mod items not in market files |
| Retro Memory Card | 14000 | Not Purchasable | collectible mod items not in market files |
| Retro Toy | 9000 | Not Purchasable | collectible mod items not in market files |
| Smartphone | 27500 | Not Purchasable | collectible mod items not in market files |
| Solid State Drive | 32500 | Not Purchasable | collectible mod items not in market files |
| Tetris Toy | 19000 | Not Purchasable | collectible mod items not in market files |
| Acetic Anhydride | 5000 | 10000 | drug-production chemical/consumable not present in market files |
| Acetone | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Ammonia | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Ammonium Nitrate | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Baking Soda | 5000 | 10000 | drug-production chemical/consumable not present in market files |
| Battery Acid (black) | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Battery Acid (green) | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Binding Agent | 5000 | 5000 | drug-production chemical/consumable not present in market files |
| Binding Agent - Blue | 5000 | 10000 | drug-production chemical/consumable not present in market files |
| Binding Agent - Green | 5000 | 10000 | drug-production chemical/consumable not present in market files |
| Binding Agent - Purple | 5000 | 10000 | drug-production chemical/consumable not present in market files |
| Binding Agent - Yellow | 5000 | 10000 | drug-production chemical/consumable not present in market files |
| Bleach (yellow) | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Bleach (white) | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Blotter Sheets | 10000 | 10000 | drug-production chemical/consumable not present in market files |
| Brake Fluid | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Butane Canister | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Color Pouch Blue | 5500 | 5500 | drug-production chemical/consumable not present in market files |
| Color Pouch Green | 5500 | 5500 | drug-production chemical/consumable not present in market files |
| Color Pouch Purple | 5500 | 5500 | drug-production chemical/consumable not present in market files |
| Color Pouch Yellow | 5500 | 5500 | drug-production chemical/consumable not present in market files |
| Hydraulic Press | 45000 | 45000 | drug-production chemical/consumable not present in market files |
| Hydrofluoric Acid | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Isopropyl Alcohol | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| LSD Blotter Sheets - Rick | 40000 | 40000 | drug-production chemical/consumable not present in market files |
| LSD Dropper | 10000 | 10000 | drug-production chemical/consumable not present in market files |
| Precursor 209 | 2500 | 5000 | drug-production chemical/consumable not present in market files |
| Precursor 510 | 2500 | 5000 | drug-production chemical/consumable not present in market files |
| Precursor 707 | 2500 | 5000 | drug-production chemical/consumable not present in market files |
| Precursor 916 | 2500 | 5000 | drug-production chemical/consumable not present in market files |
| Promethazine W/Codeine | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Pseudoephedrine Tablets 30mg | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Sodium Hdroxide | 3500 | 3500 | drug-production chemical/consumable not present in market files |
| Sulfuric Acid | 5000 | 10000 | drug-production chemical/consumable not present in market files |
| Transport Bag | 8000 | 8000 | drug-production chemical/consumable not present in market files |
| Ziplock Bag | 200 | 400 | drug-production chemical/consumable not present in market files |
| MDMA Pack | 5000 | Not Purchasable | not in market files |
| Morphine 100mg Capsule | 3500 | Not Purchasable | not in market files |
| Morphine Bottle 100mg Capsules | 3500 | Not Purchasable | not in market files |
| 40v Battery | 28000 | 80000 | keycard-crafting mod items not present in market files |
| Bolts | 2000 | 4000 | keycard-crafting mod items not present in market files |
| Flux | 25000 | 100000 | keycard-crafting mod items not present in market files |
| Helping Hands Kit | 7500 | 30000 | keycard-crafting mod items not present in market files |
| KeyCard Chip Blister Pack | 11250 | 75000 | keycard-crafting mod items not present in market files |
| KeyCard Chip Blister Pack (2nd) | 18750 | 125000 | keycard-crafting mod items not present in market files |
| KeyCard Chip Blister Pack (3rd) | 18750 | 125000 | keycard-crafting mod items not present in market files |
| KeyCard Chip Blister Pack (4th) | 26250 | 175000 | keycard-crafting mod items not present in market files |
| KeyCard Chip Blister Pack (5th) | 45000 | 300000 | keycard-crafting mod items not present in market files |
| Laptop | 7500 | 30000 | keycard-crafting mod items not present in market files |
| Lookout Door Reader | 5000 | 10000 | keycard-crafting mod items not present in market files |
| Solder | 25000 | 100000 | keycard-crafting mod items not present in market files |
| Solder Station Kit | 7500 | 30000 | keycard-crafting mod items not present in market files |
| Narcan | 5000 | 10000 | no Narcan item in market files |
| Heavy Breaching Charge | 62500 | 250000 | breaching charge mod not in market files |
| Homemade Breaching Charge | 42500 | 170000 | breaching charge mod not in market files |
| CarLockPick | 50000 | Not Purchasable | not in market files |
| Garage Hacking Tool | 50000 | 200000 | not in market files |
| Raid Alert Base | 50000 | 50000 | raid alert mod not in market files |
| Raid Alert Battery | 20000 | 20000 | raid alert mod not in market files |
| Raid Alert Sender | 125000 | 125000 | raid alert mod not in market files |
| RPG-7 | 62500 | 250000 | RPG-7 not in market files |
| RPG-7 HEAT Rocket | 18750 | 75000 | RPG-7 rocket not in market files |
| Gas Mask (Black) | 4990 | 30000 | black gas mask variant not identifiable in market files |
| M50 Respirator - OD | 4990 | 30000 | MVS/OD respirator not in market files |
| NBC Respirator | 4990 | 30000 | vanilla NBC respirator not in market files |
| PMK-2 Respirator - Black | 4990 | 30000 | not in market files |
| S10 Respirator - OD | 4990 | 30000 | not in market files |
| Cement Mixer Kit | 60000 | 300000 | not in market files |
| Concrete Mix | 7500 | 15000 | BBP material not in market files |
| Mortar Mix | 7500 | 15000 | BBP material not in market files |
| AK-50 BeoWulf | 40000 | 80000 | Beowulf rifles not in market files |
| ASH-12 | 61880 | 123750 | ASH-12 rifle not in market files (only its suppressor/mag exist elsewhere) |
| AR-15 BeoWulf | 37500 | 75000 | Beowulf rifles not in market files |
| AR-50A1 | 220000 | 440000 | not in market files |
| AS50 | 325000 | 650000 | not in market files |
| AX-MKIII | 44000 | 88000 | only one AX rifle class (a6_ax, mapped to AX-50); MKIII not present |
| GM6 Lynx | 310000 | 620000 | not in market files |
| M82A1 | 345000 | 690000 | not in market files |
| Snipex Alligator | 335000 | 837500 | not in market files |
| VSSK | 105000 | 210000 | not in market files |
| RSH118 Raid Backpack | 23400 | 52000 | not in market files |
| MVS Rucksack - OD | 15000 | 30000 | MVS mod items not in market files |
| Turtle Backpack | 14000 | 28000 | no matching bag classname in market files |
| Crye JPC 2.0 | 27500 | 45000 | only one JPC class (matched by 'JPC Plate Carrier' row); JPC 2.0 not present |
| MVS Armored Helmet | 22500 | 45000 | MVS mod items not in market files |
| MVS Heavy - OD | 27500 | 55000 | MVS mod items not in market files |
| MVS Warrior Helmet | 32500 | 65000 | MVS mod items not in market files |
| Airmax90s Shoes - In red and white | 2500 | 5000 | sneaker mod not in market files |
| Airmax98s Shoes - Black | 10000 | 20000 | sneaker mod not in market files |
| Jordans Shoe - Banned | 15000 | 30000 | sneaker mod not in market files |
| ShoeRack | 2250 | 4500 | not in market files |
| White Ultra Boost 4 | 12500 | 25000 | sneaker mod not in market files |
| Yezy Shoes | 25000 | 50000 | sneaker mod not in market files |
| Magpul STR MIII-Spec Buttstock | 3250 | 6500 | no matching buttstock classname (a6_m110_buttstock uncertain - left unpriced) |
| PSR Muzzle Brake | 1580 | 3150 | ambiguous (candidates a6_762_muzzle_brake / a6_awr_muzzle) - left unpriced |
| Normalized Suppressor | 8750 | 35000 | vanilla AK suppressor not in market files |
| Pistol Suppressor | 3750 | 15000 | vanilla pistol suppressor not in market files |
| Standardized Suppressor | 8750 | 35000 | vanilla M4 suppressor not in market files |
| Boxed .227 SIG FURY HYBRID Rounds | 1750 | 3500 | boxed-ammo classes not present in market files |
| Boxed .338 Lapua Magnum FMJ Rounds | 3250 | 6500 | boxed-ammo classes not present in market files |
| Boxed .408 Cheyenne Tactical Rounds | 5000 | 10000 | boxed-ammo classes not present in market files |
| Boxed .50 Beowulf Rounds | 2000 | 4000 | boxed-ammo classes not present in market files |
| Boxed .50 Cal. BMG Rounds | 3750 | 7500 | boxed-ammo classes not present in market files |
| Boxed 12.7x55mm STs-130VPS Rounds | 2000 | 4000 | boxed-ammo classes not present in market files |
| Boxed 12.5x114mm BZT API-T Rounds | 5000 | 10000 | boxed-ammo classes not present in market files |
| .50 BeoWulf Magazine | 4500 | 11000 | Beowulf mags not in market files |
| 30rd Coupled Standardized Mag | 1320 | 4270 | coupled STANAG not in market files |
| 60rd Standardized Mag (2nd row) | 3750 | 9120 | second 60rd Standardized row - only one A6 60rd STANAG class |
| 75rd KA-M Drum Mag | 4000 | 10250 | vanilla AKM drum not in market files |
| AK-74 6L31 Magazine | 3250 | 7940 | 6L31 60rd mag row conflicts with 'AK-74 Magazine' 3rd row already applied to A6_Mag_AK74_60Rnd - left unapplied |
| Alligator Snipex Magazine | 13500 | 32000 | not in market files |
| ASH12 Magazine | 6000 | 16000 | not in market files |
| ASH12 Magazine (2nd row) | 8000 | 24000 | not in market files |
| AX-MKIII Magazine | 3500 | 9270 | only one AX mag class (mapped to AX-50 row) |
| GM6 Lynx Magazine | 12000 | 27500 | not in market files |
| M82A1 Magazine | 16000 | 39500 | not in market files |
| VSSK Magazine | 6000 | 14000 | not in market files |
| M79 | 75000 | 300000 | M79 launcher not in market files |

## Other notes
- Vanilla-named rows (e.g. `30rd KA-74 Mag`, `Sporter 22`, `CR-75`, `BK-43`): the sheet prices both vanilla items and their A6-mod replacements, but the market files contain almost no vanilla weapons/magazines - those vanilla rows are listed as unmatched above. A6 rows (`AK-74 Magazine`, etc.) were matched to `A6_Mag_*` / `a6_*` classes.
- Several A6 magazines exist twice (mixed-case in `AmmoT_Magazines.json`/`BM_Magazines.json` and lowercase in `Magazines.json`); prices were applied to every entry with the same case-insensitive classname.
- Helicopter prices use the regular `USE AT OWN RISK` column; the `(PVP SERVERS)` column was not applied.
- `Sledgehammer 400 / 5k` and `Cooking Pot 4.8k / 6k` look like sheet typos (8% / 80% sell ratios) but were applied exactly as printed.
- `Rubber Boat` could not be priced: the Zodiac parent entry in `Veh_Sea.json` has a malformed ClassName (`ExpansionZodiacBoat (Colors as variants)`); same malformation exists on the UtilityBoat/keychain/PAZ/ZIL entries.
- Rows priced 0 sell (Beekeeper clothing, Repaint SprayCan) got `SellPricePercent = 0` so the buy price matches the sheet and sell value is 0, as printed.
