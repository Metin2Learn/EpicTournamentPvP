//1.) Search for:
void CPythonNetworkStream::__TEST_SetSkillGroupFake(int iIndex)
{
	m_dwMainActorSkillGroup = DWORD(iIndex);

	CPythonPlayer::Instance().NEW_ClearSkillData();
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshCharacter", Py_BuildValue("()"));
}
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
bool CPythonNetworkStream::RecvTournamentAdd()
{
	TPacketGCTournamentAdd dwPacket;
	if (!Recv(sizeof(dwPacket), &dwPacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_Tournament_OnRecvData", Py_BuildValue("(iiiiii)", dwPacket.dwTimeRemained, dwPacket.membersOnline_A, dwPacket.membersOnline_B, dwPacket.membersDead_A, dwPacket.membersDead_B, dwPacket.memberLives));
	return true;
}
#endif

//1.) Search for:
			case HEADER_GC_PARTY_INVITE:
				ret = RecvPartyInvite();
				break;
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
			case HEADER_GC_TOURNAMENT_ADD:
				ret = RecvTournamentAdd();
				break;
#endif