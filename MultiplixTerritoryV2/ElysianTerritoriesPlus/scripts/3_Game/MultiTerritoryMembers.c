class MultiTerritoryMembers
{
	ref map<string, int> m_Members;
		
	void MultiTerritoryMembers()
	{
		m_Members = new map<string, int>;
	}
	
	bool CheckId(string guid)
	{
		if (!m_Members)
		{
			m_Members = new map<string, int>;
		}

		if (m_Members.Contains(guid))
		{
			return true;
		}

		return false;
	}
	
	bool Check(string guid, int permission)
	{
		if (CheckId(guid))
		{
			int perms = m_Members.Get(guid);
			if (perms)
			{
				int CalcedPerms = perms & permission;
				if ( CalcedPerms == permission || perms == 1)
				{
					return true;
				}
			}
		} 

		return false;
	}
	
	bool AddMember(string guid, int permission = -1)
	{
		if (permission == -1)
		{
			permission = GetMultiTerritoriesConfig().MemberPermission();
		}

		if (!CheckId(guid))
		{
			m_Members.Insert(guid, permission);
			return true;
		}

		return false;
	}
	
	bool RemoveMember(string guid)
	{
		if (CheckId(guid))
		{
			m_Members.Remove(guid);
			return true;
		}

		return false;
	}
	
	array<string> GetMemberArray()
	{
		if (!m_Members)
		{
			m_Members = new map<string, int>;;
		}

		return m_Members.GetKeyArray();
	}
	
	int Count()
	{
		if (!m_Members)
		{
			return 0;
		}

		return m_Members.Count();
	}

	int GetPermissions(string guid)
	{
		if (!m_Members)
		{
			return 0;
		}

		if (!m_Members.Contains(guid))
		{
			return 0;
		}

		return m_Members.Get(guid);
	}

	bool SetPermissions(string guid, int permissions)
	{
		if (!m_Members)
		{
			m_Members = new map<string, int>;
		}

		if (!m_Members.Contains(guid))
		{
			return false;
		}

		m_Members.Set(guid, permissions);
		return true;
	}

	string DebugDump()
	{
		if (!m_Members || m_Members.Count() == 0)
		{
			return "[]";
		}

		string output = "[";
		array<string> guids = m_Members.GetKeyArray();
		for (int i = 0; i < guids.Count(); i++)
		{
			string guid = guids.Get(i);
			int perms = m_Members.Get(guid);

			if (i > 0)
			{
				output += ", ";
			}

			output += guid + ":" + perms.ToString();
		}

		output += "]";
		return output;
	}
	
	void Clear()
	{
		if (!m_Members)
		{
			m_Members = new map<string, int>;
		} else 
		{
			m_Members.Clear();
		}
		
	}
}

class TerritoryConst
{
	static float FLAGDOWNSTATE = 0.9;
	static float FLAGUPSTATE = 0.2;
}
