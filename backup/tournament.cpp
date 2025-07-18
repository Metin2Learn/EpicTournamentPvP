/*********************************************************************
* title_name		: Tournament PvP
* date_created		: 2017.02.12 / 01:41
* filename			: tournament.cpp
* author			: VegaS
* version_actual	: Version 0.0.3
* Created for		: Terabithia.pl
*/

#include "stdafx.h"
#include "desc.h"
#include "utils.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "affect.h"
#include "start_position.h"
#include "p2p.h"
#include "db.h"
#include "dungeon.h"
#include "castle.h"
#include <string>
#include <boost/algorithm/string/replace.hpp>
#include "desc_manager.h"
#include "buffer_manager.h"
#include "dev_log.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include "constants.h"
#include "questmanager.h"
#include "desc_client.h"
#include "sectree_manager.h"
#include "regen.h"
#include "item.h"
#include <boost/format.hpp>
#include "item_manager.h"
#include "tournament.h"

#define insert_winners(fmt, ...) RegisterWinners(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define FILENAME_STATUS			"tournament_status.txt"
#define FILENAME_LOG_WINNER		"tournament_logs_winners.txt"
#define FILENAME_BLOCK_ITEMS	"tournament_forbidden_items.txt"
#define MYSQL_DATABASE_RANKING	"player.tournament_ranking"

//std::map<DWORD, DWORD> m_map_category_low;	
//std::map<DWORD, DWORD> m_map_category_medium;	
//std::map<DWORD, DWORD> m_map_category_hard;
//std::map<DWORD, DWORD> m_map_pvp_register_all;


static const char* LC_TRANSLATE(const char* key) /* Configurable */
{
	typedef std::map<const char *, const char *> TMapTranslate;
	TMapTranslate LC_TRANSLATE;
    
    LC_TRANSLATE["TOURNAMENT_TEAM_MEMBER_RED"]							= "红队";		
    LC_TRANSLATE["TOURNAMENT_TEAM_MEMBER_BLUE"]							= "蓝队";	
    
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_START_LINE_1"]				= "<PvP对抗赛> 比赛将在几秒钟后开始，祝你好运！";
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_START_LINE_2"]				= "<PvP对抗赛> 战斗持续时间为 %d 分钟。";
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_START_LINE_3"]				= "<PvP对抗赛> 要观战，请点击左侧的任务按钮。";
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_START_LINE_4"]				= "<PvP对抗赛> 对抗赛的赌注类型为 [%s] 金额为 [%d]";
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_START_LINE_5"]				= "<PvP对抗赛> 对抗赛没有设置赌注。";
    
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_NOT_STARTED_LINE_1"]			= "<PvP对抗赛> 由于报名玩家人数不足，战斗无法开始。";
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_NOT_STARTED_LINE_2"]			= "<PvP对抗赛> 参与比赛的玩家不能离开比赛地图，而且至少需要两名玩家。";
    
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_FINISHED_LINE_1"]				= "<PvP对抗赛> 比赛结束！";
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_FINISHED_LINE_2"]				= "<PvP对抗赛> 感谢各位，比赛结果将在几秒后给出！";
    
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_WINNER_SAME"]					= "<PvP对抗赛> 比赛以平局结束，两队剩余成员数量相同。";
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_WINNER_NOT_EXIST"]			= "<PvP对抗赛> 没有队伍获胜，因为没有成员存活。";
    LC_TRANSLATE["TOURNAMENT_ANNOUNCEMENT_WINNER_SUCCES"]				= "<PvP对抗赛> 恭喜 %s 队，该队以剩余 %d 名成员赢得了比赛。";
    
    LC_TRANSLATE["TOURNAMENT_MEMBER_REMAINING_LIFE"]					= "<PvP对抗赛> 你还有 %d 条命。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_FINISHED_LIFE_LINE_1"]				= "<PvP对抗赛> 你已经没有剩余生命了。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_FINISHED_LIFE_LINE_2"]				= "<PvP对抗赛> 你已被淘汰，下一次请做好准备！";
    LC_TRANSLATE["TOURNAMENT_MEMBER_DIVIDED"]							= "<PvP对抗赛> 你被分配到了 %s 队。";
    
    LC_TRANSLATE["TOURNAMENT_MEMBER_BLOCK_DUEL"]						= "<PvP对抗赛> 你无法在此地图发起决斗。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_BLOCK_PARTY"]						= "<PvP对抗赛> 你无法在此地图使用组队功能。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_BLOCK_RING_MARRIAGE"]				= "<PvP对抗赛> 你无法在此地图使用结婚戒指。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_BLOCK_POLY"]						= "<PvP对抗赛> 你无法在此地图变身。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_BLOCK_CHANGE_PKMODE"]				= "<PvP对抗赛> 你无法在此地图更改PK模式。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_BLOCK_MOUNT"]						= "<PvP对抗赛> 你无法在此地图骑乘坐骑。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_BLOCK_HORSE"]						= "<PvP对抗赛> 你无法在此地图骑马。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_BLOCK_EXIT_OBSERVER_MODE_LINE_1"]	= "<PvP对抗赛> 你无法更改观战模式。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_BLOCK_EXIT_OBSERVER_MODE_LINE_2"]	= "<PvP对抗赛> 如果你想离开地图，请点击左侧任务按钮。";
    
    LC_TRANSLATE["TOURNAMENT_MEMBER_OPEN_REWARD_WRONG_SIZE"]			= "<PvP对抗赛> 无法使用该物品，请确保背包有至少 %d 个空位。";	
    LC_TRANSLATE["TOURNAMENT_MEMBER_CANNOT_USE_ITEM"]					= "<PvP对抗赛> 无法使用物品 %s，该物品在本地图中受限。";	
    LC_TRANSLATE["TOURNAMENT_INSERT_LOG_WINNERS"]						= "名称: %s | 等级: %d | IP: %s | 剩余生命: %d | 队伍: %s";	
    
    #ifdef ENABLE_EXTRA_LIVES_TOURNAMENT
    LC_TRANSLATE["TOURNAMENT_MEMBER_ADD_EXTRA_LIVES_ERROR_LINE_1"]		= "<PvP对抗赛> 你无法在此地图使用该物品，请在进入前使用。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_ADD_EXTRA_LIVES_ERROR_LINE_2"]		= "<PvP对抗赛> 你最近已使用过该物品，不能频繁使用，必须先用完所有额外生命。";
    LC_TRANSLATE["TOURNAMENT_MEMBER_ADD_EXTRA_LIVES_ERROR_LINE_3"]		= "<PvP对抗赛> 当前剩余额外生命：%d / %d。";	
    LC_TRANSLATE["TOURNAMENT_MEMBER_ADD_EXTRA_LIVES_SUCCES"]			= "<PvP对抗赛> 恭喜你，成功增加了 +%d 条命，可用于 %d 次参赛。";	
    #endif

	return LC_TRANSLATE[key];
}

long g_position_tournament[2][2] = /* Configurable */
{
	{ 5014000,	5011900 },	// TEAM_MEMBERS_A
	{ 5008300,	5013000 },	// TEAM_MEMBERS_B
};

long g_observers_position_tournament[3][2] = /* Configurable */
{
	{ 5015200,	5009500 },	// Random 1
	{ 5015400,	5013100 },	// Random 2
	{ 5015000,	5016500 }	// Random 3
};

bool CTournamentPvP::file_is_empty(std::ifstream& file)
{
	return (file.peek() == std::ifstream::traits_type::eof() == true || !file || !file.is_open()) ? true : false;
}

int CTournamentPvP::GetStatus()
{
	int key = 0;
	char szFileName[256];
	snprintf(szFileName, sizeof(szFileName), "%s/%s", LocaleService_GetBasePath().c_str(), FILENAME_STATUS);

	std::ifstream file(szFileName);
		
	if (!file.is_open())
	{
		sys_err("Error %s", szFileName);
		return false;
	}

	file >> key;
	file.close();

	return key;
}

void CTournamentPvP::WriteStatus(int key)
{
	char szFileName[256];
	snprintf(szFileName, sizeof(szFileName), "%s/%s", LocaleService_GetBasePath().c_str(), FILENAME_STATUS);

	FILE* file = NULL;	
	file = fopen(szFileName, "w+");

	if (!file)
	{
		sys_err("Error %s", szFileName);
		return;
	}
	
	fprintf(file, "");
	
	fprintf(file, "%d", key);	
	fclose(file);
}

void RegisterWinners(const char *func, int line, const char *format, ...)
{
	va_list kwargs;
	time_t vKey = time(0);  
	char *time_s = asctime(localtime(&vKey));

	FILE* file = NULL;
	char szBuf[1024 + 2];
	int length;
	
	char szFileName[256];
	snprintf(szFileName, sizeof(szFileName), "%s/%s", LocaleService_GetBasePath().c_str(), FILENAME_LOG_WINNER);
	file = fopen(szFileName, "a+");

	if (!file)
	{
		sys_err("Error %s", szFileName);
		return;
	}

	time_s[strlen(time_s) - 1] = '\0';
	length = snprintf(szBuf, 1024, "Date: %-15.15s | ", time_s + 4);
	szBuf[1025] = '\0';

	if (length < 1024)
	{
		va_start(kwargs, format);
		vsnprintf(szBuf + length, 1024 - length, format, kwargs);
		va_end(kwargs);
	}

	strcat(szBuf, "\n");

	fputs(szBuf, file);
	fflush(file);

	fputs(szBuf, stdout);
	fflush(stdout);
}

void CTournamentPvP::ReadFileItems()
{
	char szFileName[256];
	snprintf(szFileName, sizeof(szFileName), "%s/%s", LocaleService_GetBasePath().c_str(), FILENAME_BLOCK_ITEMS);
	std::ifstream file(szFileName);

	if (!file.is_open())
	{
		sys_err("Error %s", szFileName);
		return;
	}

	std::string line;
	while (getline(file, line))
	{
		if (line.empty())
			continue;
		
		DWORD iVnum = atoi(line.c_str());
		m_listForbidden.push_back(iVnum);
	}
}
	
bool CTournamentPvP::GetIsPermission_Day()
{
    // any time
	return true;
}

bool CTournamentPvP::AnalyzeTimer(int key)
{
	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);

	if (!CTournamentPvP::instance().GetIsPermission_Day())
		return false;

    const int now = get_global_time();
    const int registerTotalTime = CTournamentPvP::instance().GetFlag("register_total_time");
    const int pvpTotalTime = CTournamentPvP::instance().GetFlag("pvp_total_time");
	switch (key)
	{
		case TOURNAMENT_CAN_START:
		    if (registerTotalTime != 0 && now > registerTotalTime){
		        return true;
		    }
			return false;

		case TOURNAMENT_PROCESSING:
		    if (pvpTotalTime != 0 && now > pvpTotalTime){
		        return true;
		    }
			return false;
	}
}

struct FRefreshWindow
{
	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = static_cast<LPCHARACTER>(ent);
			if (ch->IsPC() && !ch->IsObserverMode())
			{
				TPacketGCTournamentAdd p;
				p.header = HEADER_GC_TOURNAMENT_ADD;
				p.membersOnline_A = CTournamentPvP::instance().GetMembersTeamA();
				p.membersOnline_B = CTournamentPvP::instance().GetMembersTeamB();
				p.membersDead_A = (CTournamentPvP::instance().GetMembersTeamA() == TOURNAMENT_MAX_PLAYERS / 2) ? 0 : TOURNAMENT_MAX_PLAYERS / 2 - CTournamentPvP::instance().GetMembersTeamA();
				p.membersDead_B = (CTournamentPvP::instance().GetMembersTeamB() == TOURNAMENT_MAX_PLAYERS / 2) ? 0 : TOURNAMENT_MAX_PLAYERS / 2 - CTournamentPvP::instance().GetMembersTeamB();
				p.memberLives = CTournamentPvP::instance().GetMyLives(ch);
				p.dwTimeRemained = CTournamentPvP::instance().GetCurrentTimer();
				ch->GetDesc()->Packet(&p, sizeof(TPacketGCTournamentAdd));
			}
		}
	}
};

struct FWarpToHome
{
	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = static_cast<LPCHARACTER>(ent);

			if (ch->IsPC())
				ch->GoHome();
		}
	}
};

EVENTINFO(TournamentPvPInfoData)
{
	CTournamentPvP *pTournament;

	TournamentPvPInfoData()
	: pTournament(0)
	{
	}
};

bool CTournamentPvP::IsTournamentMap(LPCHARACTER ch, int key)
{
	if (ch->GetMapIndex() == TOURNAMENT_MAP_INDEX)
	{
		switch(key)
		{
			case TOURNAMENT_BLOCK_DUEL:
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_BLOCK_DUEL")); 
				return true;

			case TOURNAMENT_BLOCK_PARTY:
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_BLOCK_PARTY"));
				return true;

			case TOURNAMENT_BLOCK_RING_MARRIAGE:
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_BLOCK_RING_MARRIAGE"));
				return true;

			case TOURNAMENT_BLOCK_POLY:
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_BLOCK_POLY")); 
				return true;

			case TOURNAMENT_BLOCK_CHANGE_PKMODE:
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_BLOCK_CHANGE_PKMODE"));
				return true;

			case TOURNAMENT_BLOCK_MOUNT:
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_BLOCK_MOUNT"));
				return true;			
			
			case TOURNAMENT_BLOCK_HORSE:
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_BLOCK_HORSE"));
				return true;

			case TOURNAMENT_BLOCK_EXIT_OBSERVER_MODE:
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_BLOCK_EXIT_OBSERVER_MODE_LINE_1"));		
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_BLOCK_EXIT_OBSERVER_MODE_LINE_2"));
				return true;

			default:
				return false;
		}
	}
}

int CTournamentPvP::GetDurationMinutes()
{
	return GetFlag("pvp_time");
}

std::string CTournamentPvP::ConvertTimeToString(int typeInfo)
{
	switch (typeInfo)
	{
		case TOURNAMENT_INFO_START:
			return "14:00";  // 写死开始时间

		case TOURNAMENT_INFO_FINISH:
			return "14:30";  // 写死结束时间

		default:
			return "00:00";  // 未知类型，返回默认时间
	}
}

std::string CTournamentPvP::ConvertTeamToString(DWORD idxTeam)
{
	switch (idxTeam)
	{
		case TEAM_MEMBERS_A:
			return LC_TRANSLATE("TOURNAMENT_TEAM_MEMBER_RED");
		case TEAM_MEMBERS_B:
			return LC_TRANSLATE("TOURNAMENT_TEAM_MEMBER_BLUE");
	}
}

int CTournamentPvP::GetAttackMode(int indexTeam)
{
	switch (indexTeam)
	{
		case TEAM_MEMBERS_A:
			return PK_MODE_TEAM_A;
		case TEAM_MEMBERS_B:
			return PK_MODE_TEAM_B;
	}
}

void RefreshWindow()
{
	LPSECTREE_MAP pSectreeMap = SECTREE_MANAGER::instance().GetMap(TOURNAMENT_MAP_INDEX);
	if (pSectreeMap != NULL)
	{
		FRefreshWindow f;
		pSectreeMap->for_each(f);
	}
}

void CTournamentPvP::LoadingState(int indexType)
{
    // for temp setting 1
    // int key = 1;
	switch (indexType)
	{
		case TOURNAMENT_STATE_STARTED: 
		{
		    sys_err("======================TOURNAMENT_STATE_STARTED1======================");	
			CTournamentPvP::instance().SetCurrentTime(CTournamentPvP::instance().GetDurationMinutes() * 60);
			CTournamentPvP::instance().SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_START_LINE_1"));
			CTournamentPvP::instance().SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_START_LINE_2"), CTournamentPvP::instance().GetDurationMinutes());
			CTournamentPvP::instance().SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_START_LINE_3"));
			
            if (GetFlag("bet_price") != 0)
                CTournamentPvP::instance().SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_START_LINE_4"), betTypeNames[GetFlag("bet_type") - 1], GetFlag("bet_price"));
            else
               CTournamentPvP::instance().SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_START_LINE_5"));
			
			sys_err("======================TOURNAMENT_STATE_STARTED2======================");			
			CTournamentPvP::instance().TransferByCategory();
			RefreshWindow();
		}
		break;
			
		case TOURNAMENT_STATE_NOT_STARTED:
		{
		    sys_err("======================TOURNAMENT_STATE_NOT_STARTED======================");	
			CTournamentPvP::instance().SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_NOT_STARTED_LINE_1"));
			CTournamentPvP::instance().SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_NOT_STARTED_LINE_2"));
		}
		break;
			
		case TOURNAMENT_STATE_FINISHED:
		{
		    sys_err("======================TOURNAMENT_STATE_FINISHED======================");	
			CTournamentPvP::instance().SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_FINISHED_LINE_1"));
			CTournamentPvP::instance().SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_FINISHED_LINE_2"));	
			CTournamentPvP::instance().GetTeamWinner();
		}
		break;			
	}
}

EVENTFUNC(tournament_timer)
{
	if (event == NULL)
		return 0;

	if (event->info == NULL)
		return 0;

	TournamentPvPInfoData* info = dynamic_cast<TournamentPvPInfoData*>(event->info);

	if (info == NULL)
		return 0;
	
	CTournamentPvP& pInstance = CTournamentPvP::Instance();
	
    // process send out first
    const int now = get_global_time();
	const int sendOutTime = pInstance.GetFlag("event_send_out");
	if (sendOutTime != 0 && now > sendOutTime)
	{
		pInstance.SetFlag("event_send_out", 0);
		pInstance.SendOut(false);
		return 0;
	}   
		
	static int vKey = 0;
	// TOURNAMENT_CAN_START = 0,
    if (CTournamentPvP::instance().AnalyzeTimer(TOURNAMENT_CAN_START) == true)
    {
        if (vKey == TOURNAMENT_CAN_START)
        {
            if (pInstance.CheckingStart() == true)
            {
                pInstance.LoadingState(TOURNAMENT_STATE_STARTED);
                vKey++;
                pInstance.WriteStatus(1);
                
                return PASSES_PER_SEC(1);	
            }
            else if (pInstance.CheckingStart() == false)
            {
                pInstance.LoadingState(TOURNAMENT_STATE_NOT_STARTED);
                vKey = TOURNAMENT_CAN_START;
                return PASSES_PER_SEC(1);
            }
        }
    }
    // TOURNAMENT_PROCESSING = 1, pvp time is it over ?
    else if (CTournamentPvP::instance().AnalyzeTimer(TOURNAMENT_PROCESSING) == true)
    {
        if (vKey == TOURNAMENT_PROCESSING)
        {
            pInstance.LoadingState(TOURNAMENT_STATE_FINISHED);
            vKey = TOURNAMENT_CAN_START;
            pInstance.WriteStatus(0);
            return PASSES_PER_SEC(1);
        }
    }
   
	//CTournamentPvP::instance().PrepareAnnouncement();
	return PASSES_PER_SEC(1);
}

void CTournamentPvP::SendOut(bool isStart)
{
	if (isStart)
	{
		SetFlag("event_send_out", time(0) + PVP_TOURNAMENT_SEND_OUT_TIME);
		Notice(LC_TEXT("2003"), PVP_TOURNAMENT_SEND_OUT_TIME);
	}
	else
	{
		DestroyAll();
	}
}

bool CTournamentPvP::Initialize()
{
	ReadFileItems();
	
	if (processEvent != NULL)
	{
		event_cancel(&processEvent);
		processEvent = NULL;
	}
	
	ClearRegisters();
	ClearSTDMap();
	WriteStatus(0);
	SetCurrentTime(0);
	
	return true;
}

void CTournamentPvP::Destroy()
{
	ClearSTDMap();
	ClearRegisters();
	WriteStatus(0);
	SetCurrentTime(0);
	
	if (processEvent != NULL)
	{
		event_cancel(&processEvent);
		processEvent = NULL;
	}
}

void CTournamentPvP::ClearSTDMap()
{
	m_map_team_a.clear();
	m_map_team_b.clear();
	m_map_lives.clear();
}

void CTournamentPvP::ClearRegisters()
{
	m_map_pvp_register_all.clear();	
}

bool CTournamentPvP::CanUseItem(LPCHARACTER ch, LPITEM item)
{
	if (!ch || !item)
		return false;

	switch (item->GetVnum())
	{
#ifdef ENABLE_EXTRA_LIVES_TOURNAMENT	
		case TOURNAMENT_ITEM_EXTRA_LIVES:
		{
			if (ch->GetMapIndex() == TOURNAMENT_MAP_INDEX)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_ADD_EXTRA_LIVES_ERROR_LINE_1"));
				return false;
			}
			
			if (CTournamentPvP::instance().GetExistExtraLives(ch))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_ADD_EXTRA_LIVES_ERROR_LINE_2"));
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_ADD_EXTRA_LIVES_ERROR_LINE_3"), (TOURNAMENT_CAN_USED_MAX_EXTRA_LIVES - CTournamentPvP::instance().GetUsedCountExtraLives(ch)), TOURNAMENT_CAN_USED_MAX_EXTRA_LIVES);
				return false;
			}
			
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_ADD_EXTRA_LIVES_SUCCES"), (TOURNAMENT_EXTRA_LIVES - TOURNAMENT_MAX_LIVES), TOURNAMENT_CAN_USED_MAX_EXTRA_LIVES);
			ch->SetQuestFlag(FLAG_EXTRA_LIVES, 1);
			ch->RemoveSpecifyItem(TOURNAMENT_ITEM_BOX, 1);
		}
		break;
#endif

		case TOURNAMENT_ITEM_BOX:
		{
			const DWORD MAX_LIST_REWARD = 8; /* Change here how many rows have m_dwListRewards. */
			DWORD m_dwListRewards[MAX_LIST_REWARD][2] = /* Configurable */
			{
				{	80014,	1	},
				{	14209,	1	},
				{	16209,	1	},
				{	17109,	1	},
				{	149,	1	},
				{	159,	1	},
				{	20259,	1	},
				{	299,	1	}
			};
			
			if (ch->CountEmptyInventory() < MAX_LIST_REWARD)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_OPEN_REWARD_WRONG_SIZE"), MAX_LIST_REWARD);
				return false;
			}

			for (int i=0; i< _countof(m_dwListRewards); i++)
			{
				ch->AutoGiveItem(m_dwListRewards[i][0], m_dwListRewards[i][1]);
			}
			ch->RemoveSpecifyItem(TOURNAMENT_ITEM_BOX, 1);
		}
		break;
		default:
			return true;
	}
}

void CTournamentPvP::GiveReward(LPCHARACTER ch)
{
	ch->AutoGiveItem(TOURNAMENT_ITEM_BOX, 1);
}

void CTournamentPvP::SendNoticeLine(const char * format, ...)
{
	if (!format)
		return;

	char chatbuf[CHAT_MAX_LEN + 1];
	va_list args;

	va_start(args, format);
	vsnprintf(chatbuf, sizeof(chatbuf), format, args);
	va_end(args);

	/*TPacketGGNotice p;
	p.bHeader = HEADER_GG_NOTICE;
	p.lSize = strlen(chatbuf) + 1;

	TEMP_BUFFER buf;
	buf.write(&p, sizeof(p));
	buf.write(chatbuf, p.lSize);

	P2P_MANAGER::instance().Send(buf.read_peek(), buf.size());*/
	
	SendNotice(chatbuf);
}

#ifdef ENABLE_KILL_COUNTS_FOR_EACH_PLAYER	
void CTournamentPvP::InsertPlayerKillLogs(LPCHARACTER ch)
{
	if (!ch)
		return;

	int points = ch->GetQuestFlag(FLAG_KILL_COUNT);
	DWORD pid = ch->GetPlayerID();
	const char* name = ch->GetName();
	int type = 1;

	char szQuery[512];
	snprintf(szQuery, sizeof(szQuery),
		"INSERT INTO %s (pid, name, type, total_points) VALUES(%u, '%s', %d, %d) "
		"ON DUPLICATE KEY UPDATE total_points = %d",
		MYSQL_DATABASE_RANKING,
		pid, name, type, points,
		points);

	DBManager::instance().DirectQuery(szQuery);
}
#endif

void CTournamentPvP::GetTeamWinner()
{
	if (CTournamentPvP::instance().GetMembersTeamA() == CTournamentPvP::instance().GetMembersTeamB())
	{
		SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_WINNER_SAME"));	
		SendOut();
		return;
	}
	
	if (CTournamentPvP::instance().GetMembersTeamA() == TOURNAMENT_NO_MEMBERS && CTournamentPvP::instance().GetMembersTeamB() == TOURNAMENT_NO_MEMBERS)
	{
		SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_WINNER_NOT_EXIST"));	
		SendOut();
		return;
	}	

	int idxTeamWinner = (CTournamentPvP::instance().GetMembersTeamA() > CTournamentPvP::instance().GetMembersTeamB()) ? TEAM_MEMBERS_A : TEAM_MEMBERS_B;
	int idxSize = (CTournamentPvP::instance().GetMembersTeamA() > CTournamentPvP::instance().GetMembersTeamB()) ? CTournamentPvP::instance().GetMembersTeamA() : CTournamentPvP::instance().GetMembersTeamB();
	
	switch (idxTeamWinner)
	{
		case TEAM_MEMBERS_A:
		{
			itertype(m_map_team_a) it = m_map_team_a.begin();
			LPCHARACTER ch = NULL;
			for (; it != m_map_team_a.end(); ++it)
			{
				ch = CHARACTER_MANAGER::instance().FindByPID(it->second);
				if (ch)
				{
					insert_winners(LC_TRANSLATE("TOURNAMENT_INSERT_LOG_WINNERS"), ch->GetName(), ch->GetLevel(), ch->GetDesc()->GetHostName(), CTournamentPvP::instance().GetMyLives(ch), CTournamentPvP::instance().ConvertTeamToString(idxTeamWinner).c_str());
					GiveReward(ch);
				}
			}
		}
		break;
		
		case TEAM_MEMBERS_B:
		{
			itertype(m_map_team_b) it = m_map_team_b.begin();
			LPCHARACTER ch = NULL;
			for (; it != m_map_team_b.end(); ++it)
			{
				ch = CHARACTER_MANAGER::instance().FindByPID(it->second);
				if (ch)
				{
					insert_winners(LC_TRANSLATE("TOURNAMENT_INSERT_LOG_WINNERS"), ch->GetName(), ch->GetLevel(), ch->GetDesc()->GetHostName(), CTournamentPvP::instance().GetMyLives(ch), CTournamentPvP::instance().ConvertTeamToString(idxTeamWinner).c_str());
					GiveReward(ch);
				}
			}
		}
		break;
	}

	SendNoticeLine(LC_TRANSLATE("TOURNAMENT_ANNOUNCEMENT_WINNER_SUCCES"), CTournamentPvP::instance().ConvertTeamToString(idxTeamWinner).c_str(), idxSize);
	SendOut();
}

bool CTournamentPvP::IsLimitedItem(LPCHARACTER ch, DWORD dwVnum)
{
	if (m_listForbidden.empty())
		return false;
	
	if (ch->GetMapIndex() != TOURNAMENT_MAP_INDEX)
		return false;
	
	if (std::find(m_listForbidden.begin(), m_listForbidden.end(), dwVnum) != m_listForbidden.end())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_CANNOT_USE_ITEM"), ITEM_MANAGER::instance().GetTable(dwVnum)->szLocaleName);
		return true;
	}
	
	return false;
}

bool CTournamentPvP::IsRegister(LPCHARACTER ch)
{
    if (!ch)
        return false;

    return m_map_pvp_register_all.count(ch->GetPlayerID()) > 0;
}

void CTournamentPvP::DeleteRegister(LPCHARACTER ch)
{
    if (!ch)
        return;

    m_map_pvp_register_all.erase(ch->GetPlayerID());
}

int CTournamentPvP::GetIndexTeam(LPCHARACTER ch)
{
    if (!ch)
        return 0;

    DWORD pid = ch->GetPlayerID();

    if (m_map_team_a.find(pid) != m_map_team_a.end())
        return TEAM_MEMBERS_A;

    if (m_map_team_b.find(pid) != m_map_team_b.end())
        return TEAM_MEMBERS_B;

    return 0;
}


int CTournamentPvP::Register(LPCHARACTER ch,int team)
{
	if (!ch->IsPC())
		return 0;//unkown desc!
	else if (1 != GetFlag("event_is_active"))
		return 1;//event not active!
	else if (get_global_time() > GetFlag("register_total_time"))
		return 2;//register done!
	else if (m_map_pvp_register_all.size() >= GetFlag("register_count_max"))
		return 3;//max register
	else if (GetFlag("pvp_for_job") != 0 && !(GetFlag("pvp_for_job") - 1 == ch->GetJob()))
		return 4;//not same job!
	else if (ch->GetLevel() < GetFlag("pvp_min_level"))
		return 5;//lower level
	else if (ch->GetLevel() > GetFlag("pvp_max_level"))
		return 6;//bigger level
	else if (ch->GetMapIndex() != TOURNAMENT_MAP_INDEX)
		return 7;//not on pvp map!
	else if (GetFlag("bet_type") == 2 && ch->GetGold() < GetFlag("bet_price"))
		return 8;//not enought bet money

    const auto it = m_map_pvp_register_all.find(ch->GetPlayerID());
    if (it != m_map_pvp_register_all.end())
    {
        return 9;//already register!
    }

    const int betType = GetFlag("bet_type");
	const int betPrice = GetFlag("bet_price");
	if (GetCoins(ch, betType) < betPrice)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "点卷或礼卷余额不足!");
		return 11;
	}
	
	if (m_map_pvp_register_all.size() == GetFlag("register_count_max"))
		Notice(LC_TEXT("2030"));

	if (betPrice > 0)
	{
	    if (betType==0 || betType==1){
            if (!ChangeCoins(ch, betPrice, betType))
            {
                ch->ChatPacket(CHAT_TYPE_INFO, "交易发生问题，支付失败！");
                return 12;
            }	    
	    }else{
	        ch->PointChange(POINT_GOLD, -betPrice);
	    }
	}
	
	m_map_pvp_register_all.insert(std::make_pair(ch->GetPlayerID(), ch->GetPlayerID()));
	
	return 99;
}

void CTournamentPvP::Respawn(LPCHARACTER ch)
{
	int teamIndex = CTournamentPvP::instance().GetIndexTeam(ch);
    // player restart in pvp map
	if (teamIndex != 0)
	{
		ch->Show(TOURNAMENT_MAP_INDEX, g_position_tournament[teamIndex - 1][0], g_position_tournament[teamIndex - 1][1]);
		ch->Stop();
	}
}

#ifdef ENABLE_EXTRA_LIVES_TOURNAMENT
bool CTournamentPvP::GetExistExtraLives(LPCHARACTER ch)
{
	return (ch->GetQuestFlag(FLAG_EXTRA_LIVES) > 0);
}

int CTournamentPvP::GetUsedCountExtraLives(LPCHARACTER ch)
{
	return ch->GetQuestFlag(FLAG_USED_COUNT_EXTRA_LIVES);
}

void CTournamentPvP::SetUsedCountExtraLives(LPCHARACTER ch, int val)
{
	ch->SetQuestFlag(FLAG_USED_COUNT_EXTRA_LIVES, val);
}
#endif

void CTournamentPvP::AppendLives(LPCHARACTER ch)
{
#ifdef ENABLE_EXTRA_LIVES_TOURNAMENT
	if (CTournamentPvP::instance().GetExistExtraLives(ch))
	{
		m_map_lives.insert(std::make_pair(ch->GetPlayerID(), TOURNAMENT_EXTRA_LIVES));
	}
	else
	{
		m_map_lives.insert(std::make_pair(ch->GetPlayerID(), TOURNAMENT_MAX_LIVES));
	}
#else
	m_map_lives.insert(std::make_pair(ch->GetPlayerID(), TOURNAMENT_MAX_LIVES));
#endif
}

void CTournamentPvP::DestroyAll()
{
	LPSECTREE_MAP sectree = SECTREE_MANAGER::instance().GetMap(TOURNAMENT_MAP_INDEX);

	if ( sectree != NULL )
	{
		struct FWarpToHome f;
		sectree->for_each( f );
	}
	
	ClearSTDMap();
    if (processEvent != NULL)
    {
        event_cancel(&processEvent);
        processEvent = NULL;
    }
}

void CTournamentPvP::OnDisconnect(LPCHARACTER ch)
{
	if (ch->GetMapIndex() == TOURNAMENT_MAP_INDEX)
	{
		m_map_lives.erase(ch->GetPlayerID());
		m_map_team_a.erase(ch->GetPlayerID());
		m_map_team_b.erase(ch->GetPlayerID());
		ch->GoHome();
		RefreshWindow();
	}
}

void CTournamentPvP::OnLogin(LPCHARACTER ch)
{
	if (ch->GetMapIndex() == TOURNAMENT_MAP_INDEX)
		ch->SetObserverMode(true);
}

bool CTournamentPvP::RemoveLives(LPCHARACTER pkDead)	
{
    itertype(m_map_lives) it = m_map_lives.find(pkDead->GetPlayerID());

	if (it == m_map_lives.end())
		return false;
	
	if (it->second == TOURNAMENT_LAST_LIFE)
	{
		pkDead->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_FINISHED_LIFE_LINE_1"));
		pkDead->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_FINISHED_LIFE_LINE_2"));	
		pkDead->GoHome();
		return false;
	}

	it->second -= 1;
	m_map_lives[pkDead->GetPlayerID()] = it->second;
	RefreshWindow();
	return true;
}

void CTournamentPvP::OnKill(LPCHARACTER pkKiller, LPCHARACTER pkDead)
{
	if (!pkKiller->IsPC())
		return;
	
	if (!pkDead->IsPC())
		return;
		
	if (pkKiller->GetMapIndex() != TOURNAMENT_MAP_INDEX)
		return;
	
	if (pkKiller->GetPKMode() == pkDead->GetPKMode())
		return;

#ifdef ENABLE_KILL_COUNTS_FOR_EACH_PLAYER	
	int KillCount = pkKiller->GetQuestFlag(FLAG_KILL_COUNT);
	KillCount += 1;
	pkKiller->SetQuestFlag(FLAG_KILL_COUNT, KillCount);

	CTournamentPvP::instance().InsertPlayerKillLogs(pkKiller);
#endif

	if (CTournamentPvP::instance().RemoveLives(pkDead))
	{
		pkDead->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_REMAINING_LIFE"), m_map_lives[pkDead->GetPlayerID()]);
		CTournamentPvP::instance().Respawn(pkDead);
	}
}

void CTournamentPvP::PushBack(std::map<DWORD, DWORD> m_map_global, std::vector<DWORD>* m_vec)
{
	itertype(m_map_global) iter = m_map_global.begin();
	
	for (; iter != m_map_global.end(); ++iter)
	{
		m_vec->push_back(iter->second);
	}
}

bool CTournamentPvP::CheckingStart()
{
	int playerCount = 0;

	for (const auto& it : m_map_pvp_register_all)
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(it.second);
		if (ch && ch->GetMapIndex() == TOURNAMENT_MAP_INDEX)
			++playerCount;
	}
    // TOURNAMENT_MAX_PLAYERS = 2
	return playerCount >= TOURNAMENT_MAX_PLAYERS;
}

bool CTournamentPvP::TransferByCategory()
{
    sys_err("======================TransferByCategory======================");
	std::vector<DWORD> m_vec_character, m_vec_cache;
	DWORD dwPID;
	LPCHARACTER ch = NULL;
	
	ClearSTDMap();
	sys_err("======================TransferByCategory1======================");
	CTournamentPvP::instance().PushBack(m_map_pvp_register_all, &m_vec_character);
    sys_err("======================TransferByCategory2======================");
	srand(time(0));
    sys_err("======================TransferByCategory3======================");
	while (CTournamentPvP::instance().GetMembersTeamA() < TOURNAMENT_MAX_PLAYERS / 2)
	{
		dwPID = m_vec_character[rand() % m_vec_character.size()];
		while (std::find(m_vec_cache.begin(), m_vec_cache.end(), dwPID) != m_vec_cache.end())
		{
			dwPID = m_vec_character[rand() % m_vec_character.size()];
		}
		
		ch = CHARACTER_MANAGER::instance().FindByPID(dwPID);

		if (ch != NULL)
		{
			m_map_team_a.insert(std::make_pair(dwPID, dwPID));
			m_vec_cache.push_back(dwPID);
			
			m_map_pvp_register_all.erase(dwPID);
		}
	}
	sys_err("======================TransferByCategory4======================");
	srand(time(0));
	
	while (CTournamentPvP::instance().GetMembersTeamB() < TOURNAMENT_MAX_PLAYERS / 2)
	{
		dwPID = m_vec_character[ rand() % m_vec_character.size()];
		while (std::find(m_vec_cache.begin(), m_vec_cache.end(), dwPID) != m_vec_cache.end())
		{
			dwPID = m_vec_character[rand() % m_vec_character.size()];
		}

		ch = CHARACTER_MANAGER::instance().FindByPID(dwPID);

		if (ch != NULL)
		{
			m_map_team_b.insert(std::make_pair(dwPID, dwPID));
			m_vec_cache.push_back(dwPID);
			m_map_pvp_register_all.erase(dwPID);
		}
	}
    sys_err("======================TransferByCategory5======================");
	CTournamentPvP::instance().TeleportMembers(m_map_team_a, TEAM_MEMBERS_A);
	CTournamentPvP::instance().TeleportMembers(m_map_team_b, TEAM_MEMBERS_B);
	sys_err("======================TransferByCategory6======================");
}

void CTournamentPvP::TeleportMembers(std::map<DWORD, DWORD> m_map_global, DWORD index)
{
	itertype(m_map_global) it = m_map_global.begin();
	LPCHARACTER ch = NULL;

	for (; it != m_map_global.end(); ++it)
	{
		ch = CHARACTER_MANAGER::instance().FindByPID(it->second);

		if (ch != NULL)
		{
			if (ch->IsObserverMode())
			{
				ch->SetObserverMode(false);
			}
			
			CTournamentPvP::instance().AppendLives(ch);
			ch->Show(TOURNAMENT_MAP_INDEX, g_position_tournament[index - 1][0], g_position_tournament[index - 1][1]);
			ch->Stop();
			ch->SetPKMode(CTournamentPvP::instance().GetAttackMode(index));
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TRANSLATE("TOURNAMENT_MEMBER_DIVIDED"), CTournamentPvP::instance().ConvertTeamToString(index).c_str());

#ifdef ENABLE_EXTRA_LIVES_TOURNAMENT			
			int usedCount = CTournamentPvP::instance().GetUsedCountExtraLives(ch);

			if (usedCount == TOURNAMENT_CAN_USED_MAX_EXTRA_LIVES - 1)
			{
				CTournamentPvP::instance().SetUsedCountExtraLives(ch, 0);
				ch->SetQuestFlag(FLAG_EXTRA_LIVES, 0);
			}
			else
			{
				CTournamentPvP::instance().SetUsedCountExtraLives(ch, usedCount + 1);
			}
#endif
		}
	}
}

void CTournamentPvP::Warp(LPCHARACTER ch)
{
	int random = number(0, 2);
	ch->WarpSet(g_observers_position_tournament[random][0], g_observers_position_tournament[random][1]);
}

int CTournamentPvP::ClosePvpTournament(LPCHARACTER ch)
{
    if (!ch)
        return 1;
    //event start by player id
    DWORD pid = ch->GetPlayerID();
	if (pid != GetFlag("event_player_id") && !ch->IsGM())
	    return 1;
	    
    if (processEvent)
        event_cancel(&processEvent);
        
    processEvent = event_create(tournament_timer, AllocEventInfo<TournamentPvPInfoData>(), PASSES_PER_SEC(0));
    
	SetFlag("event_is_active", 0);
	SetFlag("event_send_out", time(0) + PVP_TOURNAMENT_SEND_OUT_TIME);
	Notice(LC_TEXT("2003"), PVP_TOURNAMENT_SEND_OUT_TIME);
	m_map_pvp_register_all.clear();		
	
	return 0;
}

void CTournamentPvP::StartPvpTournament(LPCHARACTER ch)
{
    // setting pvp start
	if (!ch)
		return;
    // already started
	if (GetFlag("event_is_active") == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2013"));
		return;
	}
	if (GetFlag("register_max_time") == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2014"));
		return;
	}
	else if (GetFlag("register_count_max") == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2015"));
		return;
	}
	else if (GetFlag("pvp_min_level") == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2016"));
		return;
	}
	else if (GetFlag("pvp_max_level") == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2017"));
		return;
	}
    else if (GetFlag("pvp_time") == 0)
    {
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2018"));
        return;
    }
//    else if (GetFlag("mode_type") < 0)
//    {
//        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2019"));
//        return;
//    }    

	SetFlag("register_total_time", time(0)+GetFlag("register_max_time"));
	SetFlag("pvp_total_time", time(0)+GetFlag("register_max_time")+GetFlag("pvp_time"));
    // start event
	SetFlag("event_is_active", 1);
	SetFlag("event_player_id", ch->GetPlayerID());
	SendInfoNotice(ch);
	
    if (processEvent)
        event_cancel(&processEvent);
        
    processEvent = event_create(tournament_timer, AllocEventInfo<TournamentPvPInfoData>(), PASSES_PER_SEC(0));
}
void CTournamentPvP::SetFlag(const char* flagname, const int value, ...)
{
	char realFlag[90];
	va_list args;
	va_start(args, value);
	vsnprintf(realFlag, sizeof(realFlag), flagname, args);
	va_end(args);
	
	auto it = m_map_pvpFlags.find(realFlag);
	if (it != m_map_pvpFlags.end())
	{
		if (value != 0)
			it->second = value;
		else
			m_map_pvpFlags.erase(it->first);
	}
	else
	{
		if (value != 0)
			m_map_pvpFlags.emplace(realFlag, value);
	}
}
int CTournamentPvP::GetFlag(const char* flagname, ...)
{
	char realFlag[90];
	va_list args;
	va_start(args, flagname);
	vsnprintf(realFlag, sizeof(realFlag), flagname, args);
	va_end(args);

	const auto it = m_map_pvpFlags.find(realFlag);
	if (it != m_map_pvpFlags.end())
		return it->second;
	return 0;
}
void CTournamentPvP::SendInfoNotice(LPCHARACTER ch)
{
	char noticeText[256];
    int registerTime = GetFlag("register_max_time");
    int pvpTime = GetFlag("pvp_time");
    snprintf(noticeText, sizeof(noticeText), LC_TEXT("2004"),ch->GetName(), registerTime, GetFlag("register_count_max"), pvpTime);
    BroadcastNotice( noticeText);

    if (GetFlag("pvp_for_job") != 0 && GetFlag("pvp_for_job") >= 1 && GetFlag("pvp_for_job") <= 4)
        snprintf(noticeText, sizeof(noticeText), LC_TEXT("2005"), jobNamesTournament[GetFlag("pvp_for_job") - 1]);
    else
        snprintf(noticeText, sizeof(noticeText), LC_TEXT("2006"));

    BroadcastNotice( noticeText);
    
    if (GetFlag("pvp_pass_word") != 0){
        snprintf(noticeText, sizeof(noticeText), LC_TEXT("2040"));
        BroadcastNotice( noticeText);
    }
    
    if (GetFlag("bet_price") != 0)
        snprintf(noticeText, sizeof(noticeText), LC_TEXT("2007"), betTypeNames[GetFlag("bet_type") - 1], GetFlag("bet_price"));
    else
        snprintf(noticeText, sizeof(noticeText), LC_TEXT("2008"));
    
    BroadcastNotice( noticeText);

    snprintf(noticeText, sizeof(noticeText), LC_TEXT("2009"));
    BroadcastNotice( noticeText);
}
void CTournamentPvP::Notice(const char* szNotice, ...)
{
	char notice[CHAT_MAX_LEN + 1];
	va_list args;
	va_start(args, szNotice);
	const int len = vsnprintf(notice, sizeof(notice), szNotice, args);
	va_end(args);
	SendNoticeMap(notice, TOURNAMENT_MAP_INDEX, true);
}

uint32_t CTournamentPvP::GetCoins(LPCHARACTER ch, uint8_t type)
{
	std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery("SELECT goldcoins, silvercoins FROM %s.account WHERE id = '%d'", "account", ch->GetDesc()->GetAccountTable().id));

	MYSQL_RES *res = msg->Get()->pSQLResult;
	if (msg->uiSQLErrno != 0 || !res || !msg->Get() || msg->Get()->uiNumRows < 1)
	{
		return 0;
	}
	MYSQL_ROW row = mysql_fetch_row(res);
	
	uint32_t goldCoins = 0;
	uint32_t silverCoins = 0;
	
	str_to_number(goldCoins, row[0]);
	str_to_number(silverCoins, row[1]);

	if (type == 0)
		return goldCoins;
	
	return silverCoins;
}

bool CTournamentPvP::ChangeCoins(LPCHARACTER ch, uint32_t coins, uint8_t type)
{
	const char* coinsName = "";
	if (type == 0)
		coinsName = "goldcoins";
	else
		coinsName = "silvercoins";

	std::unique_ptr<SQLMsg> msg(DBManager::instance().DirectQuery("UPDATE %s.account SET %s = %s - %d WHERE id = '%d'", "account", coinsName, coinsName, coins, ch->GetDesc()->GetAccountTable().id));
	if (msg->uiSQLErrno != 0)
	{
		sys_log(0, "<CTournamentPvP> Error: #0002 %s", ch->GetName());
		return false;
	}
	return true;
}


namespace quest
{
	int tournament_is_map(lua_State* L)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		if (pkChar && pkChar->GetMapIndex() == TOURNAMENT_MAP_INDEX)
			lua_pushboolean(L, true);
		else
			lua_pushboolean(L, false);
		return 1;
	}
	
	int tournament_register(lua_State* L)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		
		int teamIdx = lua_tonumber(L, 1);	
		CTournamentPvP::instance().Register(pkChar,teamIdx);
		return 1;
	}
	
	int tournament_warp(lua_State* L)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		if (pkChar)
			CTournamentPvP::instance().Warp(pkChar);
		return 1;
	}
	
	int tournament_delete_register(lua_State* L)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		if (pkChar)
		{
			if (CTournamentPvP::instance().IsRegister(pkChar))
			    CTournamentPvP::instance().DeleteRegister(pkChar);
		}
		return 1;
	}
	
	int tournament_info_timer(lua_State* L)
	{
		int typeInfo = (int)lua_tonumber(L, 1);
		int typeCat = (int)lua_tonumber(L, 2);
		
		lua_pushstring(L, CTournamentPvP::instance().ConvertTimeToString(typeInfo).c_str());
		return 1;
	}
	
	int tournament_is_running(lua_State* L)
	{
		if (CTournamentPvP::instance().GetStatus() == TOURNAMENT_STARTED)
		{
			lua_pushboolean(L, true);
		}
		else {
			lua_pushboolean(L, false);
		}
		return 1;
	}	
	
	int tournament_observer(lua_State* L)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		pkChar->SetQuestFlag(FLAG_OBSERVER, 1);
		CTournamentPvP::instance().Warp(pkChar);
		return 1;
	}	
	
	int tournament_get_participants(lua_State* L)
	{
		//int categoryIndex = (int)lua_tonumber(L, 1);
		//lua_pushnumber(L, m_map_pvp_register_all.size());
		// un used
		return 1;
	}
	
	int tournament_get_is_register(lua_State* L)
	{
		LPCHARACTER pkChar = CQuestManager::instance().GetCurrentCharacterPtr();
		
		if (CTournamentPvP::instance().IsRegister(pkChar) ) {
			lua_pushboolean(L, true);
		}
		else {
			lua_pushboolean(L, false);
		}
		return 1;
	}
	
	int tournament_get_flag(lua_State* L)
	{
		if (!lua_isstring(L, 1))
			lua_pushstring(L, "");
		else
			lua_pushnumber(L, CTournamentPvP::Instance().GetFlag(lua_tostring(L, 1)));
		return 1;
	}	
	
	int tournament_info_current_timer(lua_State* L)
	{
		time_t currentTime;
		struct tm *localTime;
		time(&currentTime);
		localTime = localtime(&currentTime);
		lua_pushstring(L, asctime(localTime));
		return 1;
	}

	void RegisterTournamentPvPFunctionTable()
	{
		luaL_reg tournament_functions[] =
		{
			{	"get_is_register",	tournament_get_is_register	},
			{	"participants",		tournament_get_participants	},
			{	"is_map",			tournament_is_map			},
			{	"register",			tournament_register			},
			{	"warp",				tournament_warp				},
			{	"is_running",		tournament_is_running		},
			{	"observer",			tournament_observer			},
			{	"info_timer",		tournament_info_timer		},
			{	"info_current_timer", tournament_info_current_timer },
			{	"delete_register",	tournament_delete_register	},
			{	"get_flag",	tournament_get_flag	},
			{	NULL,		NULL					}
		};
		
		CQuestManager::instance().AddLuaFunctionTable("tournament", tournament_functions);
	}
}