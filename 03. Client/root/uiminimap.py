#1.) Search:
		self.AtlasWindow.SetMapName(mapName)
#2.) Add after:
		if app.TOURNAMENT_PVP_SYSTEM:
			if player.IsTournamentMap():
				self.canSeeInfo = FALSE
				self.HideMiniMap()
				ui.ScriptWindow.Hide(self)