# DuskVale Territory Overrides (standalone override mod)

`DuskVale_TerritoryOverrides/` is a **standalone override mod** that sits on top of
**MultiplixTerritoryV2 (ElysianTerritoriesPlus)** and **LBmaster Groups**. It uses only
`modded class` hooks — no files inside the Multiplix or LBmaster mods are edited, so you do
NOT need to rebuild their PBOs. Both mods are hard requirements (`requiredAddons`).

## Installing

1. Pack the `DuskVale_TerritoryOverrides` folder into `DuskVale_TerritoryOverrides.pbo`
   (DayZ Tools → Addon Builder, or PBO Manager). The `$PBOPREFIX$.txt` is already set.
2. Put the PBO in `@DuskVale-Overrides/addons/` (server and client).
3. Load it in your `-mod=` line **after** `@LBmaster-Groups` and your Multiplix territory mod.

## Rules

1. **Group required for territories** — A player must be in an LBmaster group before they can
   place a Territory Computer kit or claim ownership of a computer. Enforced client-side
   (deploy/place prompts hidden with a warning notification) and server-side
   (placement + `MENU_CLAIM_ADMIN` RPC both gated).

2. **"My Property" map marker** — When a territory computer is placed, an LBmaster **group map
   marker** named `My Property` is created at the exact placement position using the LBmaster
   flag icon (`LBmaster_Groups\gui\icons\flag.paa`). Visible to the placer's whole group.

3. **7-day decay** — Territories have a maximum life span of **7 days**. The override forces
   `FloppyLifetimeDays` to `7` in `$profile:Multiplix/MultiTerritoriesConfig.json` whenever it
   is `0` or missing (existing configs are migrated automatically on server start). The
   floppydisk integrity display on the computer reflects the remaining time as before.

4. **NailBox refresh** — Holding a vanilla **Box Of Nails (`NailBox`)** while looking at a running
   territory computer offers *"Refresh Territory (Box Of Nails)"*. Using it consumes the box and
   resets the countdown back to the full 7 days. Only territory members (incl. the owner's LB
   group when the MPX bridge is active) can refresh.

5. **Decay warning every 2.5 hours** — While the countdown is running, all online members of the
   owner's LBmaster group receive:
   `"<Group Name> has <hours> hours left before your base decays away, have you found any nails to prevent this?"`
   Interval and message are configurable (`DecayWarningIntervalHours`, `DecayWarningMessage`).

6. **Nametags inside territories** — Inside an active territory radius, **all** player nametags
   are visible, regardless of group/territory association. This plugs into LBmaster's safezone
   nametag system, so `SafezoneMarkers.json` must have `enablePlayerMarkers: 1` and at least one
   entry in `safezoneMarkers` (the standard LBmaster requirement for player nametags).

7. **Owner-only floppydisk removal** — Only the **territory owner** (or a server admin) can eject
   the floppydisk, via the world action, the computer menu's EJECT button, and the server RPC.
   Unclaimed computers can still have their floppy ejected by anyone.

## Files

| File | Purpose |
|---|---|
| `config.cpp` | CfgPatches/CfgMods, requires ElysianTerritoriesPlus + LBmaster_Groups |
| `scripts/3_Game/DVT_Config.c` | Config additions + 7-day default enforcement |
| `scripts/4_World/DVT_TerritoryOverrides.c` | All gameplay overrides (gates, marker, decay warning, nails refresh, nametags, eject lock) |
| `scripts/5_Mission/DVT_MenuOverrides.c` | Hides the menu EJECT button for non-owners |

## New / changed config fields (`MultiTerritoriesConfig.json`)

| Field | Default | Purpose |
|---|---|---|
| `FloppyLifetimeDays` | forced to `7` when `0` | Maximum territory life span in days |
| `DecayWarningIntervalHours` | `2.5` | How often the decay warning is sent |
| `DecayWarningMessage` | see above | `$GROUP$` and `$HOURS$` placeholders |
| `TerritoryRefreshedMessage` | ... | `$DAYS$` placeholder, shown after a NailBox refresh |
| `GroupRequiredWarningMessage` | ... | Shown when a groupless player tries to place a territory |
