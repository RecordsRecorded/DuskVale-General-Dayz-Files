modded class ItemBase
{
    string m_SourceTextureName;
    string m_VoidTextureName;

    void ItemBase()
    {
        m_SourceTextureName = GetObjectTexture( 0 );
        m_VoidTextureName = "#(argb,8,8,3)color(0.3,0.3,0.3,0.15,ca)";
    }

    string GetSourceTextureName()
    {
        return m_SourceTextureName;
    }

    string GetVoidTextureName()
    {
        return m_VoidTextureName;
    }
}