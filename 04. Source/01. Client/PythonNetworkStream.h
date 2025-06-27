//1.) Search:
		bool RecvPartyParameter();
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
		bool RecvTournamentAdd();
#endif