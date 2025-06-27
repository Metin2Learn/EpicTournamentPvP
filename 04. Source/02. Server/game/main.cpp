//1.) Search for:
#include "skill_power.h"
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	#include "tournament.h"
#endif

//1.) Search for:
	sys_log(0, "<shutdown> Destroying COXEventManager...");
	OXEvent_manager.Destroy();
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	sys_log(0, "<shutdown> Destroying CTournamentPvP...");
	TournamentPvP.Destroy();
#endif

//1.) Search for:
	OXEvent_manager.Initialize();
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	TournamentPvP.Initialize();
#endif

//1.) Search for:
	COXEventManager OXEvent_manager;
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	CTournamentPvP	TournamentPvP;
#endif