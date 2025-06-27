//1.) Search for:
	PyModule_AddIntConstant(poModule, "NAMECOLOR_EXTRA", CInstanceBase::NAMECOLOR_EXTRA);
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	PyModule_AddIntConstant(poModule, "NAMECOLOR_TOURNAMENT_TEAM_A", CInstanceBase::NAMECOLOR_TOURNAMENT_TEAM_A);
	PyModule_AddIntConstant(poModule, "NAMECOLOR_TOURNAMENT_TEAM_B", CInstanceBase::NAMECOLOR_TOURNAMENT_TEAM_B);
#endif