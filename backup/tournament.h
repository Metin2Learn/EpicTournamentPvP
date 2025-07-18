/*********************************************************************
* title_name		: Tournament PvP
* date_created		: 2017.02.12
* filename			: tournament.h
* author			: VegaS
* version_actual	: Version 0.0.3
* Created for		: Terabithia.pl
*/
#define ENABLE_KILL_COUNTS_FOR_EACH_PLAYER
#define ENABLE_EXTRA_LIVES_TOURNAMENT

#ifdef ENABLE_KILL_COUNTS_FOR_EACH_PLAYER
	#define FLAG_KILL_COUNT	"tournament.kill_count"
#endif

#ifdef ENABLE_EXTRA_LIVES_TOURNAMENT	
	#define FLAG_EXTRA_LIVES	"tournament.is_extra_lives"
	#define FLAG_USED_COUNT_EXTRA_LIVES	"tournament.used_extra_lives"
#endif

#define FLAG_OBSERVER	"tournament.is_observer_mode"
enum ETournamentT
{
	TEAM_MEMBERS_A = 1,
	TEAM_MEMBERS_B = 2,
	
	TOURNAMENT_NOT_STARTED = 0,
	TOURNAMENT_STARTED = 1,
	
	/*
		Explanation:
		* TOURNAMENT_MAX_PLAYERS = 4
		* That means val / 2 for every team.
		* If you put 20 will be 10 players vs 10 players.
	*/
	TOURNAMENT_MAX_PLAYERS = 2, // Configurable
	TOURNAMENT_MAX_LIVES  = 3, // Configurable

	TOURNAMENT_LAST_LIFE	= 1,
	TOURNAMENT_MAP_INDEX = 241, // Configurable
	TOURNAMENT_ITEM_BOX = 84201,
	
#ifdef ENABLE_EXTRA_LIVES_TOURNAMENT	
	TOURNAMENT_EXTRA_LIVES = TOURNAMENT_MAX_LIVES + 2, // Configurable
	TOURNAMENT_ITEM_EXTRA_LIVES = 84202,
	TOURNAMENT_CAN_USED_MAX_EXTRA_LIVES = 10, // Configurable
#endif
	
	TOURNAMENT_CAN_START = 0,
	TOURNAMENT_PROCESSING = 1,
	
	TOURNAMENT_MAX_CATEGORY = 3,
	
	TOURNAMENT_CATEGORY_LOW = 1,
	TOURNAMENT_CATEGORY_MEDIUM = 2,
	TOURNAMENT_CATEGORY_HARD = 3,
	
	TOURNAMENT_NO_MEMBERS = 0,
	
	TOURNAMENT_BLOCK_DUEL = 0,
	TOURNAMENT_BLOCK_PARTY = 1,
	TOURNAMENT_BLOCK_RING_MARRIAGE = 2,
	TOURNAMENT_BLOCK_POLY = 3,
	TOURNAMENT_BLOCK_CHANGE_PKMODE = 4,
	TOURNAMENT_BLOCK_MOUNT = 5,
	TOURNAMENT_BLOCK_HORSE = 6,
	TOURNAMENT_BLOCK_EXIT_OBSERVER_MODE = 7,
	
	TOURNAMENT_INFO_START = 0,
	TOURNAMENT_INFO_FINISH = 1,
	
	TOURNAMENT_STATE_STARTED = 0,
	TOURNAMENT_STATE_NOT_STARTED = 1,
	TOURNAMENT_STATE_FINISHED = 2,
	
	TOURNAMENT_PASSER_NOTICE = 10,
	
	PVP_TOURNAMENT_SEND_OUT_TIME = 60,
};

constexpr const char* jobNamesTournament[4] = { "猛将", "刺客", "修罗", "法师" };
constexpr const char* betTypeNames[4] = { "点卷", "礼卷", "金币"};
class CTournamentPvP : public singleton<CTournamentPvP>
{
	private :
		std::vector<DWORD> m_listForbidden;	
		std::map<DWORD, DWORD> m_map_team_a;
		std::map<DWORD, DWORD> m_map_team_b;
		std::map<DWORD, DWORD> m_map_lives;	
		int	dwTimeRemained;

	public :
		bool		Initialize();
		void		Destroy();

		void		ClearRegisters();
		bool		IsRegister(LPCHARACTER ch);
		void		DeleteRegister(LPCHARACTER ch);
		void		Respawn(LPCHARACTER ch);
		void		TeleportMembers(std::map<DWORD, DWORD> m_map_global, DWORD index);
		bool		AnalyzeTimer(int key);
		
		void		LoadingState(int idxType);
	
		void		ClearSTDMap();
		
		void		OnKill(LPCHARACTER pkKiller, LPCHARACTER pkDead);
		void		PushBack(std::map<DWORD, DWORD> m_map_global, std::vector<DWORD>* m_vec);
		void		ReadFileItems();

		void		GetTeamWinner();
		void		GiveReward(LPCHARACTER ch);
		void		OnDisconnect(LPCHARACTER ch);
		void		OnLogin(LPCHARACTER ch);
		void		SendNoticeLine(const char * format, ...);
		int			GetIndexTeam(LPCHARACTER ch);
		
#ifdef ENABLE_EXTRA_LIVES_TOURNAMENT
		bool		GetExistExtraLives(LPCHARACTER ch);
		int			GetUsedCountExtraLives(LPCHARACTER ch);
		void		SetUsedCountExtraLives(LPCHARACTER ch, int val);
#endif
		
		void		Warp(LPCHARACTER ch);

		bool		GetIsPermission_Day();

		bool		CheckingStart();
		
		int			GetCurrentTimer() { return dwTimeRemained; }
		void		SetCurrentTime(int val) { dwTimeRemained = val; }
		
		std::string ConvertTimeToString(int typeInfo);
		std::string	ConvertTeamToString(DWORD teamIndex);
		
		int 		GetDurationMinutes();
		
		int			GetAttackMode(int teamIndex);
		
		bool		IsTournamentMap(LPCHARACTER ch, int key);
		
		bool		TransferByCategory();
		
		bool		file_is_empty(std::ifstream& file);
		int			GetStatus();
		void		WriteStatus(int key);

#ifdef ENABLE_KILL_COUNTS_FOR_EACH_PLAYER			
		void		InsertPlayerKillLogs(LPCHARACTER ch);
#endif
		int			GetMyLives(LPCHARACTER ch) { return m_map_lives[ch->GetPlayerID()]; }
		
		void		DestroyAll();
		
		bool		CanUseItem(LPCHARACTER ch, LPITEM item);

		DWORD		GetMembersTeamA() { return m_map_team_a.size(); }
		DWORD		GetMembersTeamB() { return m_map_team_b.size(); }

		bool		IsLimitedItem(LPCHARACTER ch, DWORD dwVnum);
		
		void		AppendLives(LPCHARACTER ch);
		bool		RemoveLives(LPCHARACTER ch);
		int 		Register(LPCHARACTER ch,int team);
		
		int ClosePvpTournament(LPCHARACTER ch);
		void StartPvpTournament(LPCHARACTER ch);
        void SetFlag(const char* flagname, const int value, ...);
        int	GetFlag(const char* flagname, ...);
        void Notice(const char* szNotice, ...);
        void SendOut(bool isStart = true);
        void SendInfoNotice(LPCHARACTER ch);
        
        uint32_t GetCoins(LPCHARACTER ch, uint8_t type);
        bool ChangeCoins(LPCHARACTER ch, uint32_t coins, uint8_t type);
	        
   protected:
        LPEVENT processEvent;
        std::map<DWORD, DWORD> m_map_pvp_register_all;
        std::map<std::string, int>	m_map_pvpFlags; 
};
