modded class PluginRecipesManager
{    
    override void RegisterRecipies()
    {
        super.RegisterRecipies();

        UnregisterRecipe("CraftTerritoryFlagKit");
    }
};