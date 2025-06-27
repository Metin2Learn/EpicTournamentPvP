//1.) Search for:
#include "item.h"
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	#include "tournament.h"
#endif

//1.) Search for:
ACMD(do_party_request_accept)
{
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_PARTY))
		return;
#endif

//1.) Search for:
ACMD(do_party_request)
{
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_PARTY))
		return;
#endif

//1.) Search for:
ACMD(do_observer_exit)
{
	[..........]
}
//2.) Replace all with:
ACMD(do_observer_exit)
{
	if (ch->IsObserverMode())
	{
#ifdef TOURNAMENT_PVP_SYSTEM
		if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_EXIT_OBSERVER_MODE))
			return;
#endif
		if (ch->GetWarMap())
			ch->SetWarMap(NULL);

		if (ch->GetArena() != NULL || ch->GetArenaObserverMode() == true)
		{
			ch->SetArenaObserverMode(false);

			if (ch->GetArena() != NULL)
				ch->GetArena()->RemoveObserver(ch->GetPlayerID());

			ch->SetArena(NULL);
			ch->WarpSet(ARENA_RETURN_POINT_X(ch->GetEmpire()), ARENA_RETURN_POINT_Y(ch->GetEmpire()));
		}
		else {
			ch->ExitToSavedLocation();
		}
		ch->SetObserverMode(false);
	}
}

//1.) Search for:
	if (ch->GetLevel() < PK_PROTECT_LEVEL && mode != 0)
		return;
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_CHANGE_PKMODE))
		return;

	if (mode == PK_MODE_TEAM_A || mode == PK_MODE_TEAM_B)
		return;
#endif

//1.) Search for:
ACMD(do_pvp)
{
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_DUEL))
		return;
#endif

//1.) Search for:
ACMD(do_pvp_advanced) // If you use duel system from me.
{   
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(ch, TOURNAMENT_BLOCK_DUEL))
		return;
#endif