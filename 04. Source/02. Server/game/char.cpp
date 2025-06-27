//1.) Search for:
#include "skill_power.h"
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	#include "tournament.h"
#endif

//1.) Search for:
void CHARACTER::PartyInvite(LPCHARACTER pchInvitee)
{
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(this, TOURNAMENT_BLOCK_PARTY))
		return;
#endif

//1.) Search for:
	if (GetShop())
	{
		GetShop()->RemoveGuest(this);
		SetShop(NULL);
	}
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (IsPC())
	{	
		CTournamentPvP::instance().OnDisconnect(this);
	}
#endif

//1.) Search for:
	m_points.level = level;
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (IsPC() && GetMapIndex() == TOURNAMENT_MAP_INDEX)
		return;
#endif