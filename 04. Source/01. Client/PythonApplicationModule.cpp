//1.) Search for:
#ifdef ENABLE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_COSTUME_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_COSTUME_SYSTEM",	0);
#endif
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	PyModule_AddIntConstant(poModule, "TOURNAMENT_PVP_SYSTEM",	1);
#else
	PyModule_AddIntConstant(poModule, "TOURNAMENT_PVP_SYSTEM",	0);
#endif