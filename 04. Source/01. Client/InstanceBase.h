//1.) Search for:
		bool					IsPartyMember();
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
		bool					IsTournamentMap();
#endif

//1.) Search for:
			NAMECOLOR_WAYPOINT,	
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
			NAMECOLOR_TOURNAMENT_TEAM_A,
			NAMECOLOR_TOURNAMENT_TEAM_B,
#endif	