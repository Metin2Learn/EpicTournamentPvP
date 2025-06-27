//1.) Search for:
#include "DragonLair.h"
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM   
	#include "tournament.h"
#endif

//1.) Search for:
void CHARACTER::UpdateAlignment(int iAmount)
{
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (GetMapIndex() == TOURNAMENT_MAP_INDEX)
		return;
#endif

//1.) Search for:
		isDuel = CArenaManager::instance().OnDead(pkKiller, this);
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
		CTournamentPvP::instance().OnKill(pkKiller, this);	
#endif

//1.) Search for (x2):
	if (CBattleArena::instance().IsBattleArenaMap(GetMapIndex()) == true)
	{
		return;
	}
//2.) Add after (x2):
#ifdef TOURNAMENT_PVP_SYSTEM
	if (GetMapIndex() == TOURNAMENT_MAP_INDEX)
		return;
#endif