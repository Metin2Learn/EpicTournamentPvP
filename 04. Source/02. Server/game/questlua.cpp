//1.) Search for:
		RegisterMysqlFunctionTable();
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
		RegisterTournamentPvPFunctionTable();
#endif