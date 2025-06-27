#1.) Search:
		self.Show()
#2.) Delete and replace with:
		if app.TOURNAMENT_PVP_SYSTEM:
			if player.IsTournamentMap():
				self.Hide()
			else:
				self.Show()
		else:
			self.Show()