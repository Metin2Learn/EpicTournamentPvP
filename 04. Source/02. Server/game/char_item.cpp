//1.) Search for:
#include "buff_on_attributes.h"
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	#include "tournament.h"
#endif

//1.) Search for:
bool CHARACTER::ItemProcess_Polymorph(LPITEM item)
{
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsTournamentMap(this, TOURNAMENT_BLOCK_POLY))	
		return false;
#endif

//1.) Search for:
											if (CArenaManager::instance().IsArenaMap(pMarriage->ch1->GetMapIndex()) == true)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?? ??? ??? ? ?? ?????."));
												break;
											}
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
											if (CTournamentPvP::instance().IsTournamentMap(pMarriage->ch1, TOURNAMENT_BLOCK_RING_MARRIAGE))
												return false;
#endif

//1.) Search for:
											if (CArenaManager::instance().IsArenaMap(pMarriage->ch2->GetMapIndex()) == true)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?? ??? ??? ? ?? ?????."));
												break;
											}
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
											if (CTournamentPvP::instance().IsTournamentMap(pMarriage->ch2, TOURNAMENT_BLOCK_RING_MARRIAGE))
												return false;
#endif

//1.) Search for ( in function: bool CHARACTER::UseItem(TItemPos Cell, TItemPos DestCell) ):
	if (false == FN_check_item_sex(this, item))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??? ???? ? ???? ??? ? ????."));
		return false;
	}
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	if (CTournamentPvP::instance().IsLimitedItem(this, item->GetVnum()))
		return false;
	
	if (!CTournamentPvP::instance().CanUseItem(this, item))
		return false;
#endif