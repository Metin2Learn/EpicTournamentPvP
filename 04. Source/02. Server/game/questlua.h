//1.) Search for:
	extern void RegisterMysqlFunctionTable();
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	extern void RegisterTournamentPvPFunctionTable();
#endif