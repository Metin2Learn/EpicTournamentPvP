#1.) Search for:
import uiScriptLocale
#2.) Add after:
if app.TOURNAMENT_PVP_SYSTEM:
	import uitournament

#1.) Search for:
		del self.wndHelp
#2.) Add after:
		if app.TOURNAMENT_PVP_SYSTEM:
			del self.wndTournament
			
#1.) Search for:
		self.__MakeCubeResultWindow()
#2.) Add after:
		if app.TOURNAMENT_PVP_SYSTEM:
			self.__MakeTournamentWindow()

#1.) Search for:
	def __MakeCubeResultWindow(self):
		self.wndCubeResult = uiCube.CubeResultWindow()
		self.wndCubeResult.LoadWindow()
		self.wndCubeResult.Hide()
#2.) Add after:
	if app.TOURNAMENT_PVP_SYSTEM:
		def __MakeTournamentWindow(self):
			self.wndTournament = uitournament.Window()
			self.wndTournament.Initialize()
			self.wndTournament.Hide()
			
		def AddTournamentResultWindow(self, leftTime, membersOnline_A, membersOnline_B, membersDead_A, membersDead_B, memberLives):
			self.wndTournament.Append([int(leftTime), str(membersOnline_A), str(membersOnline_B), str(membersDead_A), str(membersDead_B), str(memberLives)])