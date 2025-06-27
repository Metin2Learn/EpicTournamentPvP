//1.) Search:
			Set(HEADER_GC_PARTY_PARAMETER,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPartyParameter), STATIC_SIZE_PACKET));
#ifdef TOURNAMENT_PVP_SYSTEM
			Set(HEADER_GC_TOURNAMENT_ADD,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCTournamentAdd), STATIC_SIZE_PACKET));
#endif