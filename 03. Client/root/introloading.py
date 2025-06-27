#1.) Search for:
		for name, rgb in NAME_COLOR_DICT.items():
			chrmgr.RegisterNameColor(name, rgb[0], rgb[1], rgb[2])
#2.) Add before !!!:
		if app.TOURNAMENT_PVP_SYSTEM:
			NAME_COLOR_DICT.update({chrmgr.NAMECOLOR_TOURNAMENT_TEAM_A : (255, 31, 31),})
			NAME_COLOR_DICT.update({chrmgr.NAMECOLOR_TOURNAMENT_TEAM_B : (10, 135, 245),})