#ifndef LB_VERSION_CHECK
#define LB_VERSION_CHECK
string lb_version_list;
#endif
int lb_version_Admin_Tools = lb_version_Admin_Tools_();
int lb_version_Admin_Tools_() {
    Print("Loaded Admin Tools (Client PBO) Version 5.9.0:stable");
    lb_version_list += "Admin_Tools#5.9.0:stable;";
    return 0;
}
string lb_version2_Admin_Tools = lb_version2_Admin_Tools_();
string lb_version2_Admin_Tools_() {
    #ifdef SERVER
    #ifndef DIAG
    #ifndef WORKBENCH
    string version = "None";
    int index = lb_version_list.IndexOf("Admin_Tools_Server#");
    if (index == -1) {
        string mods = "NOT SET";
        string servermods = "NOT SET";
        if (GetCLIParam("mod", mods)) {
            mods = "-mod=" + mods;
        } else {
            mods = "-mod NOT SET";
        }
        if (GetCLIParam("servermod", servermods)) {
            servermods = "-servermod=" + servermods;
        } else {
            servermods = "-servermod NOT SET";
        }
        Print("" + mods);
        Print("" + servermods);
        Print("Admin Tools requires a Serverside Mod for the full functionality! You can use this mod on your server, but it might not function as expected. If you bought it, download the latest server version from https://lbmaster.de/download/index.php?p=41 or buy it from https://lbmaster.de/product.php?id=41 and Make sure the Server Mod is properly loaded and check if the -mod and -servermod parameter are setup properly or the Server Mod is added to the Addons folder of your DayZ Server. Do not upload it to the steam workshop! If you have any questions, please check the Installation Guide on my website: https://lbmaster.de/install?p=41");
        Error2("LBmaster Version Check", "Admin Tools requires a Serverside Mod for the full functionality! You can use this mod on your server, but it might not function as expected. If you bought it, download the latest server version from https://lbmaster.de/download/index.php?p=41 or buy it from https://lbmaster.de/product.php?id=41 and Make sure the Server Mod is properly loaded and check if the -mod and -servermod parameter are setup properly or the Server Mod is added to the Addons folder of your DayZ Server. Do not upload it to the steam workshop! If you have any questions, please check the Installation Guide on my website: https://lbmaster.de/install?p=41");
    }
    #endif
    #endif
    #endif
    return "";
}

