#1.) Search:
	def __PartyRequestDenied(self):
		self.PopupMessage(localeInfo.PARTY_REQUEST_DENIED)
#2.) Add after:
	if app.TOURNAMENT_PVP_SYSTEM:
		def BINARY_Tournament_OnRecvData(self, leftTime, membersOnline_A, membersOnline_B, membersDead_A, membersDead_B, memberLives):
			self.interface.AddTournamentResultWindow(leftTime, membersOnline_A, membersOnline_B, membersDead_A, membersDead_B, memberLives)
			
#1.) Search:
		# QUEST_CONFIRM
		self.confirmDialog = None
		# END_OF_QUEST_CONFIRM
#2.) Add after:
		if app.TOURNAMENT_PVP_SYSTEM:
			constInfo.TOURNAMENT_WINDOW_IS_SHOWED = 0