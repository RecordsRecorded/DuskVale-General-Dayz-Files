# DuskVale Territory Overrides (Multiplix Territory V2 + LBmaster)

Custom territory rules implemented on top of **MultiplixTerritoryV2 (ElysianTerritoriesPlus)** and the
**MPX-LBmasters-Hook** bridge mod. LBmaster-dependent features live in the bridge
(`MPX-LBmasters-Hook/.../scripts/4_World/LBmasterBridge/LBGroupsTerritoryIntegration.c`) and are wrapped
in `#ifdef LBmaster_Groups`, so everything still compiles if LBmaster is removed.

## Rules

1. **Group required for territories** — A player must be in an LBmaster group before they can
   place a Territory Computer kit or claim ownership of a computer. Enforced client-side
   (deploy/place prompts hidden with a warning notification) and server-side
   (placement + `MENU_CLAIM_ADMIN` RPC both gated).

2. **"My Property" map marker** — When a territory computer is placed, an LBmaster **group map
   marker** named `My Property` is created at the exact placement position using the LBmaster
   flag icon (`LBmaster_Groups\gui\icons\flag.paa`). Visible to the placer's whole group.

3. **7-day decay** — Territories have a maximum life span of **7 days**
   (`FloppyLifetimeDays = 7` in `$profile:Multiplix/MultiTerritoriesConfig.json`; config version
   bumped to `5`, existing configs with `0` are migrated to `7` automatically). The floppydisk
   integrity display on the computer reflects the remaining time as before.

4. **NailBox refresh** — Holding a vanilla **Box Of Nails (`NailBox`)** while looking at a running
   territory computer offers *"Refresh Territory (Box Of Nails)"*. Using it consumes the box and
   resets the countdown back to the full 7 days. Only territory members (incl. LB group members
   via the bridge) can refresh.

5. **Decay warning every 2.5 hours** — While the countdown is running, all online members of the
   owner's LBmaster group receive:
   `"<Group Name> has <hours> hours left before your base decays away, have you found any nails to prevent this?"`
   Interval and message are configurable (`DecayWarningIntervalHours`, `DecayWarningMessage`).

6. **Nametags inside territories** — Inside an active territory radius, **all** player nametags
   are visible, regardless of group/territory association. This plugs into LBmaster's safezone
   nametag system, so `SafezoneMarkers.json` must have `enablePlayerMarkers: 1` and at least one
   entry in `safezoneMarkers` (the standard LBmaster requirement for player nametags).

7. **Owner-only floppydisk removal** — Only the **territory owner** (or a server admin) can eject
   the floppydisk, via the world action, the computer menu button, and the server RPC. Unclaimed
   computers can still have their floppy ejected by anyone.

## New / changed config fields (`MultiTerritoriesConfig.json`)

| Field | Default | Purpose |
|---|---|---|
| `FloppyLifetimeDays` | `7` | Maximum territory life span in days |
| `DecayWarningIntervalHours` | `2.5` | How often the decay warning is sent |
| `DecayWarningMessage` | see above | `$GROUP$` and `$HOURS$` placeholders |
| `TerritoryRefreshedMessage` | ... | `$DAYS$` placeholder, shown after a NailBox refresh |
| `GroupRequiredWarningMessage` | ... | Shown when a groupless player tries to place a territory |
