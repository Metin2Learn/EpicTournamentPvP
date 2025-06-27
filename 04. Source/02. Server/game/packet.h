//1.) Search for:
typedef struct paryt_parameter
{
	BYTE	bHeader;
	BYTE	bDistributeMode;
} TPacketGCPartyParameter;
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
typedef struct packet_tournament_add
{
	BYTE	header;
	int		membersOnline_A;
	int		membersOnline_B;
	int		membersDead_A;
	int		membersDead_B;
	int		memberLives;
	int		dwTimeRemained;
} TPacketGCTournamentAdd;
#endif

//1.) Search for:
	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER3_BGM		= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL	= 138,
	// END_OF_SUPPORT_BGM
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
	HEADER_GC_TOURNAMENT_ADD			= 145,
#endif