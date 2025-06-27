//1.) Search for:
#include "start_position.h"
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM   
	#include "tournament.h"
#endif

//1.) Search for:
BYTE SECTREE_MANAGER::GetEmpireFromMapIndex(long lMapIndex)
{
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM  
	if (lMapIndex == TOURNAMENT_MAP_INDEX)
		return 0;
#endif