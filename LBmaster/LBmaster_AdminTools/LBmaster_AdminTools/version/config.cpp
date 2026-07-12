class CfgPatches {
    class Admin_Tools_Version {
        requiredVersion = 1.0;
        requiredAddons[] = {};
    };
};
class CfgMods {
    class Admin_Tools_Version {
        dir="Admin_Tools_Version";
        name="Admin_Tools_Version";
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
                    "LBmaster_AdminTools/version/scripts"
                };
            };
        };
    };
};
class LBVersions {
    class LBmaster_AdminTools {
        version= "5.9.0";
    };
};
