//1.) Search for:
			if (IsSameEmpire(rkInstVictim))	
//2.) Replace with:
#ifdef TOURNAMENT_PVP_SYSTEM
			if (IsSameEmpire(rkInstVictim) && !IsTournamentMap())
#else
			if (IsSameEmpire(rkInstVictim))	
#endif

//1.) Search for:
			if (PK_MODE_GUILD == GetPKMode())
				if (GetGuildID() == rkInstVictim.GetGuildID())
					return false;
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
			if (IsTournamentMap())
			{
				if (GetPKMode() == rkInstVictim.GetPKMode())
					return false;
				
				if (GetPKMode() == PK_MODE_TEAM_A && rkInstVictim.GetPKMode() == PK_MODE_TEAM_B)
					return true;
				
				if (GetPKMode() == PK_MODE_TEAM_B && rkInstVictim.GetPKMode() == PK_MODE_TEAM_A)
					return true;
			}
#endif

//1.) Search for ( in function: bool CInstanceBase::IsAttackableInstance(CInstanceBase& rkInstVictim) ):
			else
			{
				return true;
			}
//2.) Replace with:
			else
			{
#ifdef TOURNAMENT_PVP_SYSTEM
				if (!IsTournamentMap())
					return true;
#else
				return true;
#endif
			}
			
//1.) Search for:
DWORD CInstanceBase::GetDuelMode()
{
	return m_dwDuelMode;
}
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
bool CInstanceBase::IsTournamentMap()
{
	const std::string& c_rstrMapFileName = CPythonBackground::Instance().GetWarpMapName();
	return c_rstrMapFileName == "metin2_map_colosseum";
}
#endif