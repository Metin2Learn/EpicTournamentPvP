#1.) Search for:
	def UpdatePosition(self):
		self.SetPosition(wndMgr.GetScreenWidth()/2 - self.GetWidth()/2, 10)	
#2.) Delete and replace with:
	def UpdatePosition(self):
		if app.TOURNAMENT_PVP_SYSTEM:
			if player.IsTournamentMap():
				self.SetPosition(wndMgr.GetScreenWidth()/2 - self.GetWidth()/2, 114)
			else:
				self.SetPosition(wndMgr.GetScreenWidth()/2 - self.GetWidth()/2, 10)
		else:
			self.SetPosition(wndMgr.GetScreenWidth()/2 - self.GetWidth()/2, 10)