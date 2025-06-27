//1.) Search for:
#include "mob_manager.h"
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	#include "tournament.h"
#endif

//1.) Search for:
	int pc_mount(lua_State* L)
	{
		[......]
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
		if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_MOUNT))
		{
			lua_pushnumber(L, 0);
			return 0;
		}
#endif