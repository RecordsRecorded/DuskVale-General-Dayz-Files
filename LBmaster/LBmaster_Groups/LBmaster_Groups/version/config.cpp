class CfgPatches {
    class Advanced_Groups_Version {
        requiredVersion = 1.0;
        requiredAddons[] = {};
    };
};
class CfgMods {
    class Advanced_Groups_Version {
        dir="Advanced_Groups_Version";
        name="Advanced_Groups_Version";
        type="mod";
        dependencies[]=
        {
            "gamelib"
        };
        class defs
        {
            class gameLibScriptModule
            {
                value="";
                files[]=
                {
                    "LBmaster_Groups/version/scripts"
                };
            };
        };
    };
};
class LBVersions {
    class LBmaster_Groups {
        version= "5.4.0";
    };
};
