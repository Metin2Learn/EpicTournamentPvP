//1.) Search for:
#include "questlua.h"
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	#include "tournament.h"
#endif

//1.) Search for:
	if (g_noticeBattleZone)
	{
		if (FN_is_battle_zone(ch))
		{
			ch->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("? ??? ???? ??? ??? ? ????."));
			ch->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("? ??? ???? ???"));
			ch->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("??? ?? ? ???? ????? ????."));
		}
	}
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	CTournamentPvP::instance().OnLogin(ch);
#endif