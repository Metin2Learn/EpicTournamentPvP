//1.) Search for:
		if (m_isKiller)
		{
			return NAMECOLOR_PK;
		}
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM		
		if (GetPKMode() == PK_MODE_TEAM_A) {
			return NAMECOLOR_TOURNAMENT_TEAM_A;
		}
		
		if (GetPKMode() == PK_MODE_TEAM_B) {
			return NAMECOLOR_TOURNAMENT_TEAM_B;
		}
#endif

//1.) Search for:
			else
			{
				return NAMECOLOR_PVP;
			}
//2.) Replace with:
			else
			{
#ifdef TOURNAMENT_PVP_SYSTEM
				if (!IsTournamentMap())
					return NAMECOLOR_PVP;					
#else
				return NAMECOLOR_PVP;
#endif
			}