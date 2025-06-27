//1.) Search for:
#include "buffer_manager.h"
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM   
	#include "tournament.h"
#endif

//1.) Search for:
	if (pkChr->GetEmpire() != pkVictim->GetEmpire())
//2.) Replace with:
#ifdef TOURNAMENT_PVP_SYSTEM   
	if (pkChr->GetEmpire() != pkVictim->GetEmpire() && pkChr->GetMapIndex() != TOURNAMENT_MAP_INDEX)
#else
	if (pkChr->GetEmpire() != pkVictim->GetEmpire())
#endif

//1.) Search for:
    CPVP kPVP(pkChr->GetPlayerID(), pkVictim->GetPlayerID());
    CPVP * pkPVP = Find(kPVP.m_dwCRC);
//2.) Add before !!!:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (pkChr->GetMapIndex() == TOURNAMENT_MAP_INDEX && pkVictim->GetMapIndex() == TOURNAMENT_MAP_INDEX)
	{
		if (pkChr->GetPKMode() == pkVictim->GetPKMode())
			return false;
		
		if (pkChr->GetPKMode() != pkVictim->GetPKMode())
			return true;
	}
#endif