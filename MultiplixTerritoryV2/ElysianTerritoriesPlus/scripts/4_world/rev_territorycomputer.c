class REV_TerritoryComputer extends TerritoryFlag
{  
    /////////////////////////////////
    ////////// NOTES/TO-DO //////////
    /////////////////////////////////
    // --> Has full functionality as a territory flag!
    // --> (KINDA DONE) The animation for inserting the floppydisk still doesn't work
    // --> (DONE) Maybe add a display somewhere that shows how much time is left on the current floppydisk, alternatively
    //     change the main display to show the remaining time/percentage remaining
    // --> (FIXED FOR NOW) Not game breaking, but on server restart, the LCD will say "CLAIM" even if it does have an owner. It will
    //     go back to normal after OnCEUpdate (the function that updates the 'flag' position) is called again.
    // --> Add a sound loop that plays when the floppydisk is ruined for an additional indication to the player to replace
    //     the floppydisk.
    // --> Look into logging territory interactions thru CF Cloud.
    // --> Add a sound for every interaction with the territory, maybe like a mouse clicking sound effect or button press sound
    // --> Still need to make the Territory Computer Kit you're holding invisible when entering placement mode (look into PerspectiveDeploy mod)
    // --> (DONE) And maybe try to get the multi-word indicators to work on the LCD screen
    // --> Avoid temp floppy attachment state; the real floppy attachment drives the visual.
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    /////////////////////////////////

    protected EffectSound 		m_AmbientSoundLoop;
	protected EffectSound		m_SoundTurnOn;
    protected EffectSound       m_SoundTurnOff;
    protected EffectSound       m_SoundBootSequence;
	protected EffectSound       m_SoundInsertFloppy;
	protected EffectSound       m_SoundEjectFloppy;

    static protected float      m_BlinkingLightInterval = 0.4; // How often the light blinks
    static protected float      m_LCDScreenChangeInterval = 0.6;
    ref Timer                   m_UpdateLCDTimer;
    ref Timer                   m_UpdateLightTimer;

    const string		SWITCH_ON_SOUND 		= "Elysian_TerritoryComputer_SwitchOn_SoundSet";
	const string		SWITCH_OFF_SOUND 		= "Elysian_TerritoryComputer_SwitchOff_SoundSet";
	const string		BOOT_SEQUENCE_SOUND 	= "Elysian_TerritoryComputer_BootSequence_SoundSet";
	const string		INSERT_FLOPPY_SOUND 	= "Elysian_TerritoryComputer_InsertFloppy_SoundSet";
	const string		EJECT_FLOPPY_SOUND		= "Elysian_TerritoryComputer_EjectFloppy_SoundSet";
	const string		LOOP_SOUND 				= "Elysian_TerritoryComputer_SoundSet";

    protected bool		m_TurnedOn				        = false;
    protected bool      m_HasValidFloppy                = false;
    protected bool      m_FloppyLocked                  = false;
    protected bool      m_LCDScreenState                = false; // Status of LCD Screen
    //protected bool      m_HasOwner;

    static const string SEL_SWITCH 				= "switch";
	static const string SEL_KEYPAD				= "keypad";
	static const string SEL_FLOPPY_INSERT		= "floppy_insert";
	static const string SEL_MAIN				= "main";

    static bool IsPrimaryInteractionSelection(string selection)
    {
        if (selection == SEL_SWITCH)
        {
            return true;
        }

        if (selection == SEL_KEYPAD)
        {
            return true;
        }

        if (selection == SEL_FLOPPY_INSERT)
        {
            return true;
        }

        if (selection == SEL_MAIN)
        {
            return true;
        }

        return false;
    }

    // Model Selections
	static protected const string SEL_SWITCH_ON				= "switch_on";
	static protected const string SEL_SWITCH_OFF			= "switch_off";
	static protected const string SEL_LIGHT_POWER			= "light_power";
	static protected const string SEL_LIGHT_WORKING			= "light_working";
	static protected const string SEL_LIGHT_NOT_WORKING		= "light_not_working";
	static protected const string SEL_HAS_FLOPPY			= "has_floppy";
	static protected const string SEL_NO_FLOPPY				= "no_floppy";
	static protected const string SEL_SCREEN_LCD			= "screen_lcd";
	static protected const string SEL_SCREEN_MAIN			= "screen_main";

    // Hidden Selection Indexes
    protected int ID_SCREEN_MAIN                         = GetHiddenSelectionIndex("screen_main");
    protected int ID_LIGHT_POWER                         = GetHiddenSelectionIndex("light_power");
    protected int ID_LIGHT_WORKING                       = GetHiddenSelectionIndex("light_working");
    protected int ID_LIGHT_NOT_WORKING                   = GetHiddenSelectionIndex("light_not_working");
    protected int ID_LIGHT_HAS_FLOPPY                    = GetHiddenSelectionIndex("light_has_floppy");
    protected int ID_LIGHT_NO_FLOPPY                     = GetHiddenSelectionIndex("light_no_floppy");
    protected int ID_SCREEN_LCD                          = GetHiddenSelectionIndex("screen_lcd");

	// glow materials
	static protected const string RED_LIGHT_GLOW			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\indicatorLights\\camo_indicatorLights_red.rvmat";
	static protected const string YELLOW_LIGHT_GLOW			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\indicatorLights\\camo_indicatorLights_yellow.rvmat";
	static protected const string GREEN_LIGHT_GLOW			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\indicatorLights\\camo_indicatorLights_green.rvmat";
	static protected const string INDICATOR_LIGHT_OFF		= "ElysianTerritoriesPlus\\Models\\Computer\\data\\indicatorLights\\camo_indicatorLights_off.rvmat";
	static protected const string SCREEN_MAIN_ON			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_on.rvmat";
    static protected const string SCREEN_MAIN_BOOT1			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_boot1.rvmat"; // <--
    static protected const string SCREEN_MAIN_BOOT2			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_boot2.rvmat"; //   |
    static protected const string SCREEN_MAIN_BOOT3			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_boot3.rvmat"; //   |
    static protected const string SCREEN_MAIN_BOOT4			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_boot4.rvmat"; //   |
    static protected const string SCREEN_MAIN_BOOT5			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_boot5.rvmat"; //   Boot Sequence Steps
    static protected const string SCREEN_MAIN_BOOT6			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_boot6.rvmat"; //   |
    static protected const string SCREEN_MAIN_BOOT7			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_boot7.rvmat"; //   |
    static protected const string SCREEN_MAIN_BOOT8			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_boot8.rvmat"; //   |
    static protected const string SCREEN_MAIN_BOOT9			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_boot9.rvmat"; // <--
    static protected const string SCREEN_MAIN_INTEGRITY_100	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_100.rvmat";    // <--
    static protected const string SCREEN_MAIN_INTEGRITY_89	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_89.rvmat";     //   |
    static protected const string SCREEN_MAIN_INTEGRITY_78	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_78.rvmat";     //   |
    static protected const string SCREEN_MAIN_INTEGRITY_68	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_68.rvmat";     //   |
    static protected const string SCREEN_MAIN_INTEGRITY_57	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_57.rvmat";     //   |
    static protected const string SCREEN_MAIN_INTEGRITY_46	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_46.rvmat";     //   Floppydisk Integrity
    static protected const string SCREEN_MAIN_INTEGRITY_35	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_35.rvmat";     //   |
    static protected const string SCREEN_MAIN_INTEGRITY_24	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_24.rvmat";     //   |
    static protected const string SCREEN_MAIN_INTEGRITY_14	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_14.rvmat";     //   |
    static protected const string SCREEN_MAIN_INTEGRITY_3	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_3.rvmat";      //   |
    static protected const string SCREEN_MAIN_INTEGRITY_0	= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_INTEGRITY_0.rvmat";      // <--
	static protected const string SCREEN_MAIN_OFF			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\main_display\\camo_main_display_off.rvmat";
	static protected const string SCREEN_LCD_ADD			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_ADD.rvmat";
	static protected const string SCREEN_LCD_EMPTY			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_EMPTY.rvmat";
	static protected const string SCREEN_LCD_ERROR			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_ERROR.rvmat";
	static protected const string SCREEN_LCD_FLOPPY			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_FLOPPY.rvmat";
	static protected const string SCREEN_LCD_NAILS			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_NAILS.rvmat";
	static protected const string SCREEN_LCD_OKAY			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_OKAY.rvmat";
    static protected const string SCREEN_LCD_CLAIM			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_CLAIM.rvmat";
    static protected const string SCREEN_LCD_OWNER			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_OWNER.rvmat";
    static protected const string SCREEN_LCD_NEED			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_NEED.rvmat";
    static protected const string SCREEN_LCD_MEMBER			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_MEMBER.rvmat";
    static protected const string SCREEN_LCD_BOOT			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_BOOT.rvmat";
    static protected const string SCREEN_LCD_INSERT			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_INSERT.rvmat";
    static protected const string SCREEN_LCD_000000			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_000000.rvmat";
	static protected const string SCREEN_LCD_OFF			= "ElysianTerritoriesPlus\\Models\\Computer\\data\\segment_display\\segment_display_off.rvmat";

    override void EEInit()
    {
        super.EEInit();

        if (GetGame().IsServer())
        {
            ApplyRefresherState(CanTerritoryBeActive());
        }

        if ( !GetGame().IsDedicatedServer() )
        {
            if ( IsTurnedOn() )
            {
                PlaySoundSetLoop( m_AmbientSoundLoop, LOOP_SOUND, 0, 0 );
                UpdateIndicatorLights();
                UpdateAnimationStates();

            }
        }
    }

    void REV_TerritoryComputer()
    {
        m_TurnedOn = false;
        m_HasValidFloppy = false;
        RegisterNetSyncVariableBool("m_TurnedOn");
        RegisterNetSyncVariableBool("m_HasValidFloppy");
        RegisterNetSyncVariableBool("m_FloppyLocked");
        // Sync the refresher time so clients can read accurate floppy %.
        // Without this, GetRefresherTime01() always returns 0 on client.
        RegisterNetSyncVariableInt("m_RefresherTimeRemaining", 0, 5184000); // up to 60 days
    }

    void ~REV_TerritoryComputer()
    {
        StopAmbientLoop();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.RefreshTerritoryLifetimes);

        if (GetGame() && GetGame().IsServer())
        {
            RemoveRefresherPosition();
        }
    }

    override string GetConstructionKitType()
	{
		return "REV_TerritoryComputerKit";
	}

    override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		ctx.Write(m_TurnedOn);
        ctx.Write(m_HasValidFloppy);
        ctx.Write(m_FloppyLocked);
	}

    override bool HasBase()
    {
        return true;
    }

    override void SetPartsAfterStoreLoad()
    {
        SetPartsFromSyncData();

        SetBaseState(true);

        SynchronizeBaseState();
    }

    override bool CanAddMember()
	{
		return m_CanAddMember;
	}

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
        {
            return false;
        }
			
		if (!ctx.Read(m_TurnedOn))
		{
			return false;
		}
        if (!ctx.Read(m_HasValidFloppy))
        {
            return false;
        }
        if (!ctx.Read(m_FloppyLocked))
        {
            return false;
        }

        HasValidFloppyCheck();
        UpdateAnimationStates();
        LockFloppydisk(m_FloppyLocked);
        UpdateIndicatorLights();
        SynchronizeRefresherTime();

        if (IsTurnedOn())                                                                   // Perform a power cycle on server start just in case it still doesn't delete the attachment
        {
            SwitchPowerOff();
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SwitchPowerOn, 2000, false);
        }
		return true;
	}

    override void AfterStoreLoad()
    {
        super.AfterStoreLoad();

        
    }

    bool IsTurnedOn()
    {
        return m_TurnedOn;
    }

    bool HasValidFloppy()
    {
        HasValidFloppyCheck();
        return m_HasValidFloppy;
    }

    bool CanTerritoryBeActive()
    {
        HasValidFloppyCheck();
        return IsTurnedOn() && IsFloppyLocked() && m_HasValidFloppy && m_RefresherTimeRemaining > 0;
    }

    float GetEffectiveRefresherDuration()
    {
        float duration = GetMultiTerritoriesConfig().GetFloppyLifetimeSeconds(m_FlagRefresherMaxDuration);
        if (duration < 1)
        {
            duration = 1;
        }

        return duration;
    }

    void ForceProtectionRefresh(bool syncRefresherTime = true)
    {
        if (syncRefresherTime)
        {
            SynchronizeRefresherTime();
        }

        bool active = CanTerritoryBeActive();
        ApplyRefresherState(active);
        UpdateTerritoryLifetimeRefresh(active);
        GetMultiTerritoriesConfig().DiagnosticLog("Protection refresh: turnedOn=" + IsTurnedOn() + " floppyLocked=" + IsFloppyLocked() + " validFloppy=" + m_HasValidFloppy + " timeRemaining=" + m_RefresherTimeRemaining + " active=" + active);
        UpdateIndicatorLights();
        UpdateAnimationStates();
        SetSynchDirty();
    }

    void ApplyRefresherState(bool active)
    {
        if (!GetGame().IsServer())
        {
            return;
        }

        if (active)
        {
            InitRefresherData();
            SetRefresherActive(true);
        }
        else
        {
            SetRefresherActive(false);
            RemoveRefresherPosition();
        }

        GetMultiTerritoriesConfig().DiagnosticLog("ApplyRefresherState active=" + active + " (hybrid mode)");
    }

    void UpdateTerritoryLifetimeRefresh(bool active)
    {
        if (!GetGame().IsServer())
        {
            return;
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.RefreshTerritoryLifetimes);

        if (!active)
        {
            return;
        }

        RefreshTerritoryLifetimes();

        int refreshMs = GetMultiTerritoriesConfig().GetProtectionRefreshIntervalSeconds() * 1000;
        if (refreshMs < 60000)
        {
            refreshMs = 60000;
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RefreshTerritoryLifetimes, refreshMs, true);
    }

    void RefreshTerritoryLifetimes()
    {
        if (!GetGame().IsServer() || !GetCEApi())
        {
            return;
        }

        if (!CanTerritoryBeActive())
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.RefreshTerritoryLifetimes);
            return;
        }

        float protectionRadius = GetMultiTerritoriesConfig().GetProtectionRadius();
        GetCEApi().RadiusLifetimeReset(GetPosition(), protectionRadius);

        array<Object> objects = new array<Object>;
        array<CargoBase> proxyCargos = new array<CargoBase>;
        GetGame().GetObjectsAtPosition(GetPosition(), protectionRadius, objects, proxyCargos);

        int refreshedObjects = 0;
        EntityAI entity;
        float maxLifetime;
        int configuredLifetime;
        for (int i = 0; i < objects.Count(); i++)
        {
            entity = EntityAI.Cast(objects.Get(i));
            if (!entity || entity == this)
            {
                continue;
            }

            configuredLifetime = GetMultiTerritoriesConfig().GetKitLifeTime(entity.GetType());
            if (configuredLifetime > 0)
            {
                entity.SetLifetime(configuredLifetime);
                GetMultiTerritoriesConfig().DiagnosticLog("KitLifeTimes refresh matched type=" + entity.GetType() + " lifetime=" + configuredLifetime);
                refreshedObjects++;
                continue;
            }

            maxLifetime = entity.GetLifetimeMax();
            if (maxLifetime <= 0)
            {
                continue;
            }

            entity.SetLifetime(maxLifetime);
            refreshedObjects++;
        }

        GetMultiTerritoriesConfig().DiagnosticLog("Territory lifetimes refreshed around active computer. Objects refreshed=" + refreshedObjects);
    }

    void OnTurnedOn()
    {
        //Print("[ElysianTerritoriesPlus][EXECUTE] OnTurnedOn"); Might add back in later
        m_TurnedOn = true;

        HasValidFloppyCheck();
    }

    void OnTurnedOff()
    {
        //Print("[ElysianTerritoriesPlus][EXECUTE] OnTurnedOff"); Might add back in later
        m_TurnedOn = false;

        HasValidFloppyCheck();
        StopLCDScreenCallQueue();

        ApplyRefresherState(false);
    }

void SwitchPowerOn()
    {
        GetMultiTerritoriesConfig().DebugLog("SwitchPowerOn called");
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ReactivateTerritoryIfNeeded);
        OnTurnedOn();
        GetMultiTerritoriesConfig().DiagnosticLog("m_TurnedOn set to true");

        HasValidFloppyCheck();
        ForceProtectionRefresh(true);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ReactivateTerritoryIfNeeded, 500, false);
        MainDisplayBoot();
    }
    
    void ReactivateTerritoryIfNeeded()
    {
        ForceProtectionRefresh(true);

        if (CanTerritoryBeActive())
        {
            GetMultiTerritoriesConfig().DiagnosticLog("Territory reactivated after power cycle. Time remaining: " + m_RefresherTimeRemaining);
        }
    }

    void SwitchPowerOff()
    {
        GetMultiTerritoriesConfig().DebugLog("SwitchPowerOff called");
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ReactivateTerritoryIfNeeded);
        OnTurnedOff();
        GetMultiTerritoriesConfig().DiagnosticLog("m_TurnedOn set to false");

        HasValidFloppyCheck();
        ForceProtectionRefresh(false);
    }

    void ClientPreviewPowerOn()
    {
        if (GetGame().IsDedicatedServer())
        {
            return;
        }

        m_TurnedOn = true;
        HasValidFloppyCheck();
        UpdateAnimationStates();
        UpdateIndicatorLights();
        MainDisplayBoot();
    }

    void ClientPreviewPowerOff()
    {
        if (GetGame().IsDedicatedServer())
        {
            return;
        }

        m_TurnedOn = false;
        HasValidFloppyCheck();
        StopLCDScreenCallQueue();
        PlayPowerOffSounds();
        UpdateAnimationStates();
        UpdateIndicatorLights();
        UpdateMainDisplayIntegrityBar();
    }

    void ClientRefreshComputerVisuals()
    {
        if (GetGame().IsDedicatedServer())
        {
            return;
        }

        HasValidFloppyCheck();
        UpdateAnimationStates();
        UpdateIndicatorLights();
        UpdateMainDisplayIntegrityBar();
    }

    void ClientPreviewFloppyInserted(bool updateMainDisplay)
    {
        if (GetGame().IsDedicatedServer())
        {
            return;
        }

        HasValidFloppyCheck();
        m_FloppyLocked = IsTurnedOn();
        UpdateAnimationStates();
        UpdateIndicatorLights();
        if (updateMainDisplay)
        {
            UpdateMainDisplayIntegrityBar();
        }
    }

    void ClientPreviewMemberInviteChanged(bool inviteActive)
    {
        if (GetGame().IsDedicatedServer())
        {
            return;
        }

        m_CanAddMember = inviteActive;
        UpdateIndicatorLights();
    }

    void ClientPreviewMemberDatabaseChanged()
    {
        if (GetGame().IsDedicatedServer())
        {
            return;
        }

        m_CanAddMember = false;
        UpdateIndicatorLights();
    }

    void UpdateAnimationStates()
    {
        //Print("[ElysianTerritoriesPlus][EXECUTE] UpdateAnimationStates"); Might add back in later

        if (IsTurnedOn())
		{
			SetAnimationPhase("switch_on", 0.0);
			SetAnimationPhase("switch_off", 1.0);
		}
		if (!IsTurnedOn())
		{
			SetAnimationPhase("switch_on", 1.0);
			SetAnimationPhase("switch_off", 0.0);
        }

        Territory_Floppydisk floppy = Territory_Floppydisk.Cast(FindAttachmentBySlotName("Material_FPole_Flag"));

        if (!floppy)
        {
            HideFloppyVisual();
            return;
        }

        if (IsFloppyLocked() || floppy.IsLockedInSlot())
        {
            HideFloppyVisual();
            return;
        }

        ShowFloppyVisual();
    }

    void HideFloppyVisual()
    {
        SetAnimationPhase("floppy_visual", 1.0);
    }

    void ShowFloppyVisual()
    {
        SetAnimationPhase("floppy_visual", 0.0);
    }


    bool HasFloppy()
    {
        if ( FindAttachmentBySlotName("Material_FPole_Flag") )
		{
            //Print("[ElysianTerritoriesPlus][INFO][BOOL] HasFloppy returned TRUE");
			return true;
		}
		else
		{
            //Print("[ElysianTerritoriesPlus][INFO][BOOL] HasFloppy returned FALSE");
			return false;
		}

    }

    void UpdateIndicatorLights()
	{
        //Print("[ElysianTerritoriesPlus][EXECUTE] UpdateIndicatorLights");
        HasValidFloppyCheck();
        UpdateLCDText();
		if ( IsTurnedOn() )
		{
			if ( HasValidFloppy() && IsFloppyLocked() )			                                // Has a valid floppydisk attached & inserted
			{
				LightWorkingOn();
				LightNotWorkingOff();
				LightHasFloppyOn();
				LightNoFloppyOff();
			}
            else if ( HasValidFloppy() && !IsFloppyLocked() )                                   // Has a valid floppydisk attached that has not been inserted
            {
                LightWorkingOff();
                LightNotWorkingOn();
                LightHasFloppyOff();
                LightNoFloppyOn();
            }
            else if ( HasFloppy() && !HasValidFloppy() )						                // Has an invalid floppy attached
			{
				LightWorkingOff();
				LightNotWorkingOn();
				LightHasFloppyOn();
				LightNoFloppyOff();
			}
			else if ( !HasFloppy() )											                // Does not have a floppydisk attached
			{
				LightWorkingOff();
				LightNotWorkingOn();
				LightHasFloppyOff();
				LightNoFloppyOn();
			}
			LightPowerOn();
		}
		if ( !IsTurnedOn() )											                        // Powered off (All lights off)
		{
			LCDScreenOff();
			MainDisplayOff();
			LightPowerOff();
			LightWorkingOff();
			LightNotWorkingOff();
			LightHasFloppyOff();
			LightNoFloppyOff();
		}
	}

    void MainDisplayBoot()
    {


        SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_BOOT1);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( SetObjectMaterial, 400, false, ID_SCREEN_MAIN, SCREEN_MAIN_BOOT2);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( SetObjectMaterial, 800, false, ID_SCREEN_MAIN, SCREEN_MAIN_BOOT3);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( SetObjectMaterial, 1200, false, ID_SCREEN_MAIN, SCREEN_MAIN_BOOT4);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( SetObjectMaterial, 1600, false, ID_SCREEN_MAIN, SCREEN_MAIN_BOOT5);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( SetObjectMaterial, 2000, false, ID_SCREEN_MAIN, SCREEN_MAIN_BOOT6);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( SetObjectMaterial, 2400, false, ID_SCREEN_MAIN, SCREEN_MAIN_BOOT7);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( SetObjectMaterial, 2800, false, ID_SCREEN_MAIN, SCREEN_MAIN_BOOT8);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( SetObjectMaterial, 3200, false, ID_SCREEN_MAIN, SCREEN_MAIN_BOOT9);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( UpdateMainDisplayIntegrityBar, 5200, false,);
    }

    void UpdateLCDText()
    {
        //Print("[ElysianTerritoriesPlus][EXECUTE] UpdateLCDText");
        if ( IsTurnedOn() )
        {
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // TO DO: Add "HasOwner()" condition to update the LCD to prompt the user to claim the territory //
            ///////////////////////////////////////////////////////////////////////////////////////////////////

            if ( CanAddMember() )                                                           // Inviting New Member
            {
                StopLCDScreenCallQueue();
                LCDScreenAddMemberCall();
            }
            else if ( HasValidFloppy() && IsFloppyLocked() )                                // Has a valid floppydisk attached & inserted
            {
                StopLCDScreenCallQueue();
                LCDScreenText(SCREEN_LCD_OKAY);
            }
            else if ( HasValidFloppy() && !IsFloppyLocked() )                               // Has a valid floppydisk attached that has not been inserted 
            {
                StopLCDScreenCallQueue();
                LCDScreenText(SCREEN_LCD_INSERT);
            }
            else if ( HasFloppy() && !HasValidFloppy() )                                    // Has an invalid floppydisk attached (presumably used)
            {
                StopLCDScreenCallQueue();
                LCDScreenFloppyErrorCall();
            }
            else if ( !HasFloppy() )                                                        // Does not have a floppydisk attached
            {
                StopLCDScreenCallQueue();
                LCDScreenNeedFloppyCall();
            }
            else                                                                            // Fallback in case of undefined state
            {
                StopLCDScreenCallQueue();
                LCDScreenText(SCREEN_LCD_ERROR);
            }
        }
    }

    void LockFloppydiskFromAction()
    {
        if (!IsTurnedOn())
        {
            SwitchPowerOn();
        }

        LockFloppydisk(true);
    }

    void EjectFloppy(PlayerBase player = null)
    {
        Territory_Floppydisk floppy = Territory_Floppydisk.Cast(FindAttachmentBySlotName("Material_FPole_Flag"));
        if (!floppy || !GetGame().IsServer())
        {
            return;
        }

        float floppyHealth = floppy.GetHealth("", "");
        vector ejectPos = GetPosition() + Vector(0, 0.35, 0);
        if (player)
        {
            ejectPos = player.GetPosition() + (player.GetDirection() * 0.35) + Vector(0, 0.15, 0);
        }

        if (IsTurnedOn())
        {
            SwitchPowerOff();
        }

        if (IsFloppyLocked())
        {
            LockFloppydisk(false);
        }

        EntityAI newFloppyEntity = EntityAI.Cast(GetGame().CreateObjectEx("Territory_Floppydisk", ejectPos, ECE_PLACE_ON_SURFACE));
        Territory_Floppydisk newFloppy = Territory_Floppydisk.Cast(newFloppyEntity);
        if (newFloppy)
        {
            newFloppy.SetHealth("", "", floppyHealth);
        }

        GetGame().ObjectDelete(floppy);
        HasValidFloppyCheck();
        ForceProtectionRefresh(true);
        UpdateMainDisplayIntegrityBar();
        UpdateIndicatorLights();
        PlayEjectFloppySound();
    }

    void LockFloppydisk(bool do_lock)
    {
        Territory_Floppydisk floppy = Territory_Floppydisk.Cast(FindAttachmentBySlotName("Material_FPole_Flag"));
        if ( floppy )
        {
            if ( do_lock )
            {
                m_FloppyLocked = true;
                HasValidFloppyCheck();

                if (m_RefresherTimeRemaining <= 0)
                {
                    m_RefresherTimeRemaining = Math.Round(GetEffectiveRefresherDuration());
                }
                
                bool active = CanTerritoryBeActive();
                ApplyRefresherState(active);
                UpdateTerritoryLifetimeRefresh(active);
                UpdateAnimationStates();

                if (active)
                {
                    GetMultiTerritoriesConfig().DiagnosticLog("Territory claimed and protection active. Duration: " + m_RefresherTimeRemaining);
                }
                else
                {
                    GetMultiTerritoriesConfig().DiagnosticLog("Floppy locked but protection inactive. turnedOn=" + IsTurnedOn() + " validFloppy=" + m_HasValidFloppy + " timeRemaining=" + m_RefresherTimeRemaining);
                }

                SetSynchDirty();
            }
            else if ( !do_lock )
            {
                m_FloppyLocked = false;
                ApplyRefresherState(false);
                UpdateTerritoryLifetimeRefresh(false);
                UpdateAnimationStates();
                GetMultiTerritoriesConfig().DiagnosticLog("Territory floppy removed/expired");

                SetSynchDirty();
            }

            UpdateIndicatorLights();
        }
    }

    bool IsFloppyLocked()
    {
        return m_FloppyLocked;
    }

    override void EEItemAttached(EntityAI item, string slot_name)
    {
        super.EEItemAttached(item, slot_name);

        if (slot_name == "Material_FPole_Flag")
        {
            HasValidFloppyCheck();
            ForceProtectionRefresh(true);
        }
    }

    override void EEItemDetached(EntityAI item, string slot_name)
    {
        super.EEItemDetached(item, slot_name);

        if (slot_name == "Material_FPole_Flag")
        {
            if (!GetGame().IsDedicatedServer())
            {
                m_TurnedOn = false;
                StopLCDScreenCallQueue();
                PlayPowerOffSounds();
            }

            HasValidFloppyCheck();
            PlayEjectFloppySound();
            UpdateMainDisplayIntegrityBar();
            LockFloppydisk(false);
            ForceProtectionRefresh(true);
        }
    }

//////////////////////////////
/////// SOUNDS CONTROL ///////
//////////////////////////////

	void PlayAmbientLoop()
	{
		if (IsTurnedOn())
		{
            StopSoundSet( m_AmbientSoundLoop );
			PlaySoundSetLoop( m_AmbientSoundLoop, LOOP_SOUND, 0, 0 );
			//Print("[ElysianTerritoriesPlus][INFO][SOUND] Playing Switch on sound");
		}
		else
		{
			//Print("[ElysianTerritoriesPlus][WARNING][SOUND] Tried to play ambient sound loop, but computer is turned off");
		}
	}
	void StopAmbientLoop()
	{
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.PlayAmbientLoop);
        StopSoundSet( m_AmbientSoundLoop );
		//Print("[ElysianTerritoriesPlus][INFO][SOUND] Stopped Ambient Computer Sound Loop");
	}
	void PlaySwitchOnSound()
	{
        PlaySoundSet( m_SoundTurnOn, SWITCH_ON_SOUND, 0, 0 );
		//Print("[ElysianTerritoriesPlus][INFO][SOUND] Playing Switch on sound");
	}
	void PlaySwitchOffSound()
	{
		PlaySoundSet( m_SoundTurnOff, SWITCH_OFF_SOUND, 0, 0 );
		//Print("[ElysianTerritoriesPlus][INFO][SOUND] Playing Switch off sound");
	}
	void PlayBootSequenceSound()
	{
        PlaySoundSet( m_SoundBootSequence, BOOT_SEQUENCE_SOUND, 0, 0 );
		//Print("[ElysianTerritoriesPlus][INFO][SOUND] Playing Boot Sequence sound");
	}
	void StopBootSequenceSound()
	{
		StopSoundSet( m_SoundBootSequence );
		//Print("[ElysianTerritoriesPlus][INFO][SOUND] Stopping Boot Sequence sound");
	}
	void PlayInsertFloppySound()
	{
        PlaySoundSet( m_SoundInsertFloppy, INSERT_FLOPPY_SOUND, 0, 0 );
        
		//Print("[ElysianTerritoriesPlus][INFO][SOUND] Playing Insert Floppy sound");
	}
	void PlayEjectFloppySound()
	{
        PlaySoundSet( m_SoundEjectFloppy, EJECT_FLOPPY_SOUND, 0, 0 );
		//Print("[ElysianTerritoriesPlus][INFO][SOUND] Playing Eject Floppy sound");
	}

    void PlayPowerOnSounds()
    {
        PlaySwitchOnSound();
        PlayBootSequenceSound();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.PlayAmbientLoop);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( this.PlayAmbientLoop, 6500, false );
    }
    void ClientPlayPowerOnSoundsFromFloppyInsert()
    {
        if (GetGame().IsDedicatedServer())
        {
            return;
        }

        ClientPreviewPowerOn();
        PlayPowerOnSounds();
    }
	void PlayPowerOffSounds()
	{
		PlaySwitchOffSound();
		StopAmbientLoop();
		StopBootSequenceSound();
	}

//////////////////////////////
/////// LIGHTS CONTROL ///////
//////////////////////////////

	//Lights On
	void MainDisplayOn()
	{
		SetObjectMaterial( ID_SCREEN_MAIN, SCREEN_MAIN_ON );
		//Print("[ElysianTerritoriesPlus][INFO][Lights] Set main screen to on");
	}
	void LightPowerOn()
	{
		SetObjectMaterial( ID_LIGHT_POWER, YELLOW_LIGHT_GLOW );
		//Print("[ElysianTerritoriesPlus][INFO][Lights] Set power indicator light to on");
	}
	void LightWorkingOn()
	{
		SetObjectMaterial( ID_LIGHT_WORKING, GREEN_LIGHT_GLOW );
		//Print("[ElysianTerritoriesPlus][INFO][Lights] Set working light to on");
	}
	void LightNotWorkingOn()
	{
		SetObjectMaterial( ID_LIGHT_NOT_WORKING, RED_LIGHT_GLOW );
		//Print("[ElysianTerritoriesPlus][INFO][Lights] Set not working light to on");
	}
	void LightHasFloppyOn()
	{
		SetObjectMaterial( ID_LIGHT_HAS_FLOPPY, GREEN_LIGHT_GLOW );
		//Print("[ElysianTerritoriesPlus][INFO][Lights] Set has floppy light to on");
	}
	void LightNoFloppyOn()
	{
		SetObjectMaterial( ID_LIGHT_NO_FLOPPY, RED_LIGHT_GLOW );
		//Print("[ElysianTerritoriesPlus][INFO][Lights] Set no floppy light to on");
	}
	

	//Lights Off
	void MainDisplayOff()
	{
		SetObjectMaterial( ID_SCREEN_MAIN, SCREEN_MAIN_OFF );
		//Print("[ElysianTerritoriesPlus][INFO][LCD] Set Main screen to off");
	}
	void LightPowerOff()
	{
		SetObjectMaterial( ID_LIGHT_POWER, INDICATOR_LIGHT_OFF );
		//Print("[ElysianTerritoriesPlus][INFO][LCD] Set power indicator light to off");
	}
	void LightWorkingOff()
	{
		SetObjectMaterial( ID_LIGHT_WORKING, INDICATOR_LIGHT_OFF );
		//Print("[ElysianTerritoriesPlus][INFO][LCD] Set working light to off");
	}
	void LightNotWorkingOff()
	{
		SetObjectMaterial( ID_LIGHT_NOT_WORKING, INDICATOR_LIGHT_OFF );
		//Print("[ElysianTerritoriesPlus][INFO][LCD] Set not working light to off");
	}
	void LightHasFloppyOff()
	{
		SetObjectMaterial( ID_LIGHT_HAS_FLOPPY, INDICATOR_LIGHT_OFF );
		//Print("[ElysianTerritoriesPlus][INFO][LCD] Set has floppy light to off");
	}
	void LightNoFloppyOff()
	{
		SetObjectMaterial( ID_LIGHT_NO_FLOPPY, INDICATOR_LIGHT_OFF );
		//Print("[ElysianTerritoriesPlus][INFO][LCD] Set no floppy light to off");
	}
	void LCDScreenOff()
	{
		SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_OFF );
		//Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to off");
	}

////////////////////////////////
////// LCD SCREEN CONTROL //////
////////////////////////////////

    void LCDScreenText(string screen_text)
	{
		switch (screen_text)
		{
			case SCREEN_LCD_ADD:
			{
				SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_ADD );
				//Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to ADD");
				break;
			}
			case SCREEN_LCD_EMPTY:
			{
				SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_EMPTY );
				//Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to EMPTY");
				break;
			}
			case SCREEN_LCD_ERROR:
			{
				SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_ERROR );
				//Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to ERROR");
				break;
			}
			case SCREEN_LCD_FLOPPY:
			{
				SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_FLOPPY );
				//Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to FLOPPY");
				break;
			}
			case SCREEN_LCD_NAILS:
			{
				SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_NAILS );
				//Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to NAILS");
				break;
			}
			case SCREEN_LCD_OKAY:
			{
				SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_OKAY );
				//Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to OKAY");
				break;
			}
            case SCREEN_LCD_ADD:
            {
                SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_ADD );
                //Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to ADD");
                break;
            }
            case SCREEN_LCD_OWNER:
            {
                SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_OWNER );
                //Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to OWNER");
                break;
            }
            case SCREEN_LCD_NEED:
            {
                SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_NEED );
                //Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to NEED");
                break;
            }
            case SCREEN_LCD_MEMBER:
            {
                SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_MEMBER );
                //Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to MEMBER");
                break;
            }
            case SCREEN_LCD_BOOT:
            {
                SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_BOOT );
                //Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to BOOT");
                break;
            }
            case SCREEN_LCD_CLAIM:
            {
                SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_CLAIM );
                //Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to CLAIM");
                break;
            }
            case SCREEN_LCD_000000:
            {
                SetObjectMaterial( ID_SCREEN_LCD, SCREEN_LCD_000000 );
                //Print("[ElysianTerritoriesPlus][INFO][LCD] Set LCD Screen to 000000");
                break;
            }
		}
	}

    void LCDScreenClaimOwnerCall()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LCDScreenClaimOwner, 1000, true);
    }
    void LCDScreenAddMemberCall()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LCDScreenAddMember, 1000, true);
    }
    void LCDScreenFloppyErrorCall()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LCDScreenFloppyError, 1000, true);
    }
    void LCDScreenNeedFloppyCall()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LCDScreenNeedFloppy, 1000, true);
    }
    void StopLCDScreenCallQueue()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(LCDScreenClaimOwner);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(LCDScreenAddMember);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(LCDScreenFloppyError);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(LCDScreenNeedFloppy);
        m_LCDScreenState = false;
    }
    void LCDScreenClaimOwner()
    {
        if (!m_LCDScreenState)
        {
            LCDScreenText(SCREEN_LCD_CLAIM);
        }
        if (m_LCDScreenState)
        {
            LCDScreenText(SCREEN_LCD_OWNER);
        }
        m_LCDScreenState = !m_LCDScreenState;
    }
    void LCDScreenAddMember()
    {
        if (!m_LCDScreenState)
        {
            LCDScreenText(SCREEN_LCD_ADD);
        }
        if (m_LCDScreenState)
        {
            LCDScreenText(SCREEN_LCD_MEMBER);
        }
        m_LCDScreenState = !m_LCDScreenState;
    }
    void LCDScreenFloppyError()
    {
        if (!m_LCDScreenState)
        {
            LCDScreenText(SCREEN_LCD_FLOPPY);
        }
        if (m_LCDScreenState)
        {
            LCDScreenText(SCREEN_LCD_ERROR);
        }
        m_LCDScreenState = !m_LCDScreenState;
    }
    void LCDScreenNeedFloppy()
    {
        if (!m_LCDScreenState)
        {
            LCDScreenText(SCREEN_LCD_NEED);
        }
        if (m_LCDScreenState)
        {
            LCDScreenText(SCREEN_LCD_FLOPPY);
        }
        m_LCDScreenState = !m_LCDScreenState;
    }

    override void SetActions()
	{
		super.SetActions();

		AddAction(ActionTurnOnTerritoryComputer);
		AddAction(ActionTurnOffTerritoryComputer);
        AddAction(ActionInsertFloppy);
        AddAction(ActionOpenTerritoryComputerMenu);
        AddAction(REV_ActionDismantleObject);
		RemoveAction(ActionAddTerritoryMember);
		RemoveAction(ActionResetTerritoryMembers);
		RemoveAction(ActionAcceptMembership);
		RemoveAction(ActionFoldBaseBuildingObject);
		RemoveAction(ActionLowerFlag);
		RemoveAction(ActionRaiseFlag);
	}

    override void ResetAllowMemberToBeAdded()
    {
        super.ResetAllowMemberToBeAdded();

        UpdateIndicatorLights();
    }

    override bool CanPutInCargo( EntityAI parent )
    {
        return false;
    }
    override bool CanPutIntoHands( EntityAI parent )
    {
        return false;
    }
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return false;
	}

    void HasValidFloppyCheck()
	{
		Territory_Floppydisk floppy = Territory_Floppydisk.Cast(FindAttachmentBySlotName("Material_FPole_Flag"));
		
		if (floppy)
		{
			if (!GetGame().IsClient() && GetGame().IsDedicatedServer())
			{
				if (floppy.GetHealth() > 0.0)
				{
					m_HasValidFloppy = true;
					SetSynchDirty();
				}
				else
				{
					m_HasValidFloppy = false;
					SetSynchDirty();
				}
			}
			else if (GetGame().IsClient())
			{
				m_HasValidFloppy = true;
			}
		}
		else
		{
			m_HasValidFloppy = false;
			SetSynchDirty();
		}
	}

    override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		string slot_name = InventorySlots.GetSlotName(slotId);
		if ( slot_name == "Material_FPole_Flag" )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!attachment)
		{
			return super.CanReleaseAttachment(attachment);
		}

		if (attachment == FindAttachmentBySlotName("Material_FPole_Flag"))
		{
			return false;
		}

		return super.CanReleaseAttachment(attachment);
	}

	override bool CanDisplayAttachmentCategory( string category_name )
	{
		if ( category_name == "Flag" )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
    override bool CanBeRepairedToPristine()
	{
		return false;
	}

	bool CanDismantle()
	{
		if (IsFloppyLocked() || HasFloppy() || IsTurnedOn())
			return false;

		return true;
	}

    override void AnimateFlagEx(float delta, PlayerBase player = null)
    {
        super.AnimateFlagEx(delta, player);

        Territory_Floppydisk floppy = Territory_Floppydisk.Cast(FindAttachmentBySlotName("Material_FPole_Flag"));

        float newFloppyHealth;

        if ( !GetGame().IsClient() && GetGame().IsDedicatedServer() && IsTurnedOn() )
        {
            if (IsFloppyLocked() && IsTurnedOn() && floppy)
            {
                float fraction = m_RefresherTimeRemaining / GetEffectiveRefresherDuration();
                fraction = Math.Clamp(fraction, 0, 1);

                newFloppyHealth = 100 * fraction;
                floppy.SetHealth("", "", newFloppyHealth);

                if (newFloppyHealth == 0)
                {
                    LockFloppydisk(false);
                    UpdateIndicatorLights();
                }
                SetSynchDirty();
            }
        }

        UpdateMainDisplayIntegrityBar();
    }

    void SynchronizeRefresherTime()
    {
        if (!GetGame().IsServer())
        {
            return;
        }

        Territory_Floppydisk floppy = Territory_Floppydisk.Cast(FindAttachmentBySlotName("Material_FPole_Flag"));
        float floppyHealth = 0;

        if (floppy)
        {
            floppyHealth = floppy.GetHealth();
        }

        if (IsFloppyLocked() && floppy && floppyHealth > 0)
        {
            float fraction = floppyHealth / 100;
            fraction = Math.Clamp(fraction, 0, 1);
            float newRefresherTime = GetEffectiveRefresherDuration() * fraction;
            m_RefresherTimeRemaining = Math.Round(newRefresherTime);
        }
        else
        {
            m_RefresherTimeRemaining = 0;
        }

        ApplyRefresherState(CanTerritoryBeActive());
        UpdateMainDisplayIntegrityBar();
        SetSynchDirty();
    }

    override float GetRefresherTime01()
    {
        float duration = GetEffectiveRefresherDuration();
        if (duration <= 0)
        {
            return 0;
        }

        float time01 = m_RefresherTimeRemaining / duration;
        return Math.Clamp(time01, 0, 1);
    }

    void UpdateMainDisplayIntegrityBar()
    {

        Territory_Floppydisk floppy = Territory_Floppydisk.Cast(FindAttachmentBySlotName("Material_FPole_Flag"));
        float floppyHealth = 0;

        if (!IsTurnedOn())
        {
            SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_OFF);
            return;
        }

        if (!floppy)
        {
            SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_0);
            return;
        }

        if (!GetGame().IsClient() && GetGame().IsDedicatedServer())
        {
            floppyHealth = floppy.GetHealth();
        }
        else
        {
            floppyHealth = 100;
        }

        if (IsTurnedOn())
        {
            if (floppyHealth >= 100)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_100);
            }
            else if (floppyHealth >= 89)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_89);
            }
            else if (floppyHealth >= 78)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_78);
            }
            else if (floppyHealth >= 68)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_68);
            }
            else if (floppyHealth >= 57)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_57);
            }
            else if (floppyHealth >= 46)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_46);
            }
            else if (floppyHealth >= 35)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_35);
            }
            else if (floppyHealth >= 24)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_24);
            }
            else if (floppyHealth >= 14)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_14);
            }
            else if (floppyHealth >= 3)
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_3);
            }
            else
            {
                SetObjectMaterial(ID_SCREEN_MAIN, SCREEN_MAIN_INTEGRITY_0);
            }
        }
    }
}
